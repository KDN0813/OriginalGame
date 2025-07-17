﻿#include "PlayerStateDerived.h"
#include "Input/Input.h"
#include "Object/Object.h"
#include "Object\Constant\PlayerConstant.h"
#include "Audio\Audio.h"
#include "System\GameData.h"
#include "Effekseer\EffekseerSystem.h"

#include "Object\Constant\PlayerConstant.h"

#include "Component/StateMachineComponent.h"
#include "Component/ModelAnimationControlComponent.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/TransformComponent.h"

PlayerIdleState::PlayerIdleState()
{
}

void PlayerIdleState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::IDLE, true, 0.2f);
}

void PlayerIdleState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wptr);
    if (!player) return;

    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (movement->IsMoveXZAxis())
    {
        state_machine->ChangeState("MoveState");
        return;
    }

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // 死亡判定
        if (!character->IsAlive())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState("DeadState");
            return;
        }

        // 被ダメ判定
#if 0
        if (character->IsDamage())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState(this->change_damage_state);
            return;
        }
#endif // 0
    }

    // 入力受付
    {
        Input::Instance input = Input::GetInstance();
        GamePad& pad = input->GetGamePad();
        // Xボタン
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            // 攻撃ステートへ遷移
            state_machine->ChangeState("AttackState");
        }
        // Yボタン
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            // 回転攻撃ステートに遷移
            if (player->IsUseSpecialGage(player->GetSpinAttackUseGageCount()))
            {
                state_machine->ChangeState("SpinAttackStartState");
                return;
            }
        }
    }
}

PlayerMoveState::PlayerMoveState()
{
}

void PlayerMoveState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true, 0.2f);
}

void PlayerMoveState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wptr);
    if (!player) return;

    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (!movement->IsMoveXZAxis())
    {
        state_machine->ChangeState("IdleState");
        return;
    }

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // 死亡判定
        if (!character->IsAlive())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState("DeadState");
            return;
        }

        // 被ダメ判定
#if 0
        if (character->IsDamage())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState(this->change_damage_state);
            return;
        }
#endif // 0
    }

    // 入力受付
    {
        Input::Instance input = Input::GetInstance();
        GamePad& pad = input->GetGamePad();
        // Xボタン
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            // 攻撃ステートへ遷移
            state_machine->ChangeState("AttackState");
            return;
        }
        // Yボタン
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            // 回転攻撃ステートに遷移
            if (player->IsUseSpecialGage(player->GetSpinAttackUseGageCount()))
            {
                state_machine->ChangeState("SpinAttackStartState");
                return;
            }
        }
    }
}

PlayerAttackState::PlayerAttackState()
{
}

void PlayerAttackState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // アニメーションの再生
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::ATTACK01, false, 0.2f);

    // SE再生
    {
        Audio::Instance audio = Audio::GetInstance();
        AudioParam param{};
        param.volume = 0.5f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash01.wav";
        audio->Play(param);
    }

    // 攻撃判定オブジェクトを有効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK01_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // コリジョンを有効にする

    collision->EvaluateCollision();

    // 攻撃エフェクト再生
    if (const auto& transform = owner->GetComponent(this->transform_Wptr))
    {
        if (const auto& attack_object_transform = attack_object->GetComponent(attack_object_transform_Wptr))
        {
            const DirectX::XMFLOAT3 pos = attack_object_transform->GetWorldPosition();
            const float effect_scale = collision->GetRadius();
            DirectX::XMFLOAT3 rotation{};
            rotation.y = transform->GetLocalAngle().y;
            EffekseerSystem::GetInstance()->PlayEffect("Data/Effect/Effekseer/Attack1.efk", pos, effect_scale, rotation);
        }
    }

    // 無敵状態に設定
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);

    const auto& player = owner->GetComponent(this->player_Wprt);
    if (!player) return;
    // 移動速度の倍率変更
    player->SetMoveRate(player->GetAttackMoveRate());
}

void PlayerAttackState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player) return;

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // 死亡判定
        if (!character->IsAlive())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState("DeadState");
            return;
        }
    }

    // 入力受付
    //{
    //    Input::Instance input = Input::GetInstance();
    //    GamePad& pad = input->GetGamePad();
    //    // Xボタン
    //    if (pad.GetButtonDown() & GamePad::BTN_X)
    //    {
    //        // コンボ攻撃ステートに遷移
    //        state_machine->ChangeState("AttackComboState");
    //        return;
    //    }
    //}

    // アニメーション再生待ち
    if (player->GetAttackEndPoint() <= animation->GetMainPartsCurrentAnimationSeconds())
    {
        // 待機ステートに遷移
        state_machine->ChangeState("AttackHoldState");
        return;
    }
}

void PlayerAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // 攻撃判定オブジェクトを無効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK01_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // コリジョンを無効にする

    // 無敵状態を解除
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}

PlayerAttackComboState::PlayerAttackComboState()
{
}

void PlayerAttackComboState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // アニメーションの再生
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::ATTACK02, false, 0.2f);

    // SE再生
    {
        Audio::Instance audio = Audio::GetInstance();
        AudioParam param{};
        param.volume = 0.5f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash01.wav";
        audio->Play(param);
    }

    // 攻撃判定オブジェクトを有効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK02_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // コリジョンを有効にする

    collision->EvaluateCollision();

    // 攻撃エフェクト再生
    if (const auto& transform = owner->GetComponent(this->transform_Wptr))
    {
        if (const auto& attack_object_transform = attack_object->GetComponent(attack_object_transform_Wptr))
        {
            const DirectX::XMFLOAT3 pos = attack_object_transform->GetWorldPosition();
            const float effect_scale = collision->GetRadius();
            DirectX::XMFLOAT3 rotation{};
            rotation.y = transform->GetLocalAngle().y;
            EffekseerSystem::GetInstance()->PlayEffect("Data/Effect/Effekseer/Attack2.efk", pos, effect_scale, rotation);
        }
    }

    // 無敵状態に設定
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);

    const auto& player = owner->GetComponent(this->player_Wprt);
    if (!player) return;
    // 移動速度の倍率変更
    player->SetMoveRate(player->GetAttackMoveRate());
}

void PlayerAttackComboState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player) return;

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // 死亡判定
        if (!character->IsAlive())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState("DeadState");
            return;
        }
    }

    // 入力受付
    {
        Input::Instance input = Input::GetInstance();
        GamePad& pad = input->GetGamePad();
        // Xボタン
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            // 攻撃ステートへ遷移
            state_machine->ChangeState("AttackState");
            return;
        }
    }

    // アニメーション再生待ち
    if (player->GetAttackCombo2EndPoint() <= animation->GetMainPartsCurrentAnimationSeconds())
    {
        // 待機ステートに遷移
        state_machine->ChangeState("AttackHoldState");
        return;
    }
}

void PlayerAttackComboState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // 攻撃判定オブジェクトを無効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK02_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // コリジョンを無効にする

    // 無敵状態を解除
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}

PlayerAttackHoldState::PlayerAttackHoldState()
{
}

void PlayerAttackHoldState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wptr);
    if (!player) return;

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // 死亡判定
        if (!character->IsAlive())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState("DeadState");
            return;
        }
    }

    // 入力受付
    {
        Input::Instance input = Input::GetInstance();
        GamePad& pad = input->GetGamePad();
        // Yボタン
        if (pad.GetButton() & GamePad::BTN_Y)
        {
            // 回転攻撃ステートに遷移
            if (player->IsUseSpecialGage(player->GetSpinAttackUseGageCount()))
            {
                state_machine->ChangeState("SpinAttackStartState");
                return;
            }
        }
    }

    // 移動判定
    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (movement->IsMoveXZAxis())
    {
        state_machine->ChangeState("MoveState");
        return;
    }

    // アニメーション再生待ち
    if (!animation->IsPlayMainPartsAnimation())
    {
        // 待機ステートに遷移
        state_machine->ChangeState("IdleState");
        return;
    }
}

void PlayerAttackHoldState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& player = owner->GetComponent(this->player_Wprt);
    if (!player) return;
    // 移動速度の倍率を元に戻す
    player->SetMoveRate(1.0f);
}

PlayerSpinAttackStartState::PlayerSpinAttackStartState()
{
}

void PlayerSpinAttackStartState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    // アニメーションの再生
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation)return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK_START, false, 0.2f);

    const auto& player = owner->GetComponent(this->player_Wprt);
    if (!player) return;

    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;

    // 攻撃エフェクト再生
    if (const auto& transform = owner->GetComponent(this->transform_Wptr))
    {
        if (const auto& attack_object_transform = attack_object->GetComponent(this->attack_object_transform_Wptr))
        {
            const DirectX::XMFLOAT3 pos = attack_object_transform->GetWorldPosition();
            const float effect_scale = 8.0f;
            DirectX::XMFLOAT3 rotation{};
            rotation.y = transform->GetLocalAngle().y;
            //rotation.z = .0f;

            player->SetSpinAttackEffectHandle(EffekseerSystem::GetInstance()->PlayEffect("Data/Effect/Effekseer/SpinAttack.efk", pos, effect_scale, rotation));
        }
    }
}

void PlayerSpinAttackStartState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;


    // アニメーション再生待ち
    if (!animation->IsPlayMainPartsAnimation())
    {
        // 回転
        state_machine->ChangeState("SpinAttackSpinLoopState");
        return;
    }
}

PlayerSpinAttackSpinLoopState::PlayerSpinAttackSpinLoopState()
{
}

void PlayerSpinAttackSpinLoopState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // アニメーションの再生
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK_SPIN_LOOP, true, 0.0f);

    // SE再生
    {
        Audio::Instance audio = Audio::GetInstance();
        AudioParam param{};
        param.volume = 0.4f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash02.wav";
        audio->Play(param);
    }

    // プレイヤーの入力移動を無効にする
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
    {
        player->SetIsSpinAttack(true);  // 回転攻撃フラグを立てる
        player->SetMoveRate(player->GetSpinAttackMoveRate());
        player->SetSpinAttackTimer(player->GetSpinAttackTime());

        // 回転攻撃の残り時間UIをアクティブ化する
        if (const auto& player_owner = player->GetOwner())
        {
            if (const auto& spin_attack_ui_object = player_owner->FindChildObject(PlayerConstant::SPIN_ATTACK_TIME_UI_NAME))
            {
                spin_attack_ui_object->SetIsActive(true);
            }
        }
    }

    // 攻撃判定オブジェクトを有効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (!collision) return;

    collision->SetIsActive(true);  // コリジョンを有効にする
    collision->EvaluateCollision();

    // 無敵状態に設定
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);

    // ポイント消費
    {
        player->UseSpecialGage(player->GetSpinAttackUseGageCount());
    }
}

void PlayerSpinAttackSpinLoopState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wptr);
    if (!player) return;
    const auto& transform = owner->GetComponent(this->transform_Wptr);
    if (!transform) return;

    // 攻撃エフェクトの位置更新
    {
        const DirectX::XMFLOAT3 pos = transform->GetWorldPosition();
        const Effekseer::Handle handle =player->GetSpinAttackEffectHandle();

        EffekseerSystem::GetInstance()->SetPositionEffect(handle, pos);
    }

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // 死亡判定
        if (!character->IsAlive())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState("DeadState");
            return;
        }
    }

    // 攻撃判定オブジェクトを有効にする
    {
        const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
        if (!attack_object) return;
        auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
        if (!collision) return;
        collision->EvaluateCollision();
    }

    float attack_time = player->GetSpinAttackTimer() - elapsed_time;
    if (attack_time <= 0.0f)
    {
        // 攻撃時間が0になったら待機ステートに遷移
        state_machine->ChangeState("IdleState");
    }
    player->SetSpinAttackTimer(attack_time);
}

void PlayerSpinAttackSpinLoopState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // プレイヤーの入力移動を有効にする
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
    {
        player->SetIsSpinAttack(false);  // 回転攻撃フラグを折る
        player->SetInputMoveValidityFlag(true);
        player->SetMoveRate(1.0f);

        // 回転攻撃の残り時間UIを非アクティブ化する
        if (const auto& player_owner = player->GetOwner())
        {
            if (const auto& spin_attack_ui_object = player_owner->FindChildObject(PlayerConstant::SPIN_ATTACK_TIME_UI_NAME))
            {
                spin_attack_ui_object->SetIsActive(false);
            }
        }
    }

    // 攻撃判定オブジェクトを無効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // コリジョンを無効にする

    // 無敵状態を解除
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);

    // エフェクト停止
    {
        const Effekseer::Handle handle = player->GetSpinAttackEffectHandle();

        EffekseerSystem::GetInstance()->StopEffect(handle);
    }
}

PlayerSpinAttackState::PlayerSpinAttackState()
{
}

void PlayerSpinAttackState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // アニメーションの再生
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK, false, 0.0f);

    // SE再生
    {
        Audio::Instance audio = Audio::GetInstance();
        AudioParam param{};
        param.volume = 0.4f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash02.wav";
        audio->Play(param);
    }

    // プレイヤーの入力移動を無効にする
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
    {
        //player->SetInputMoveValidityFlag(false);
        player->SetMoveRate(player->GetSpinAttackMoveRate());
    }

    // 攻撃判定オブジェクトを有効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (!collision) return;
        
    collision->SetIsActive(true);  // コリジョンを有効にする
    collision->EvaluateCollision();

    // 無敵状態に設定
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);
}

void PlayerSpinAttackState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;

    // 攻撃判定オブジェクトを有効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (!collision) return;
    collision->EvaluateCollision();

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // 死亡判定
        if (!character->IsAlive())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState("DeadState");
            return;
        }
    }

    // アニメーション再生待ち
    if (!animation->IsPlayMainPartsAnimation())
    {
        // 待機ステートに遷移
        state_machine->ChangeState("IdleState");
        return;
    }
}

void PlayerSpinAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // プレイヤーの入力移動を有効にする
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
    {
        player->SetInputMoveValidityFlag(true);
        player->SetMoveRate(1.0f);
    }

    // 攻撃判定オブジェクトを無効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // コリジョンを無効にする

    // 無敵状態を解除
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}

PlayerDamageState::PlayerDamageState()
{
}

void PlayerDamageState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::DAMAGE, false);

    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player)return;
    player->SetInputMoveValidityFlag(false);
}

void PlayerDamageState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // 死亡判定
        if (!character->IsAlive())
        {
            // 被ダメステートに遷移
            state_machine->ChangeState("DeadState");
            return;
        }
    }

    // アニメーション再生待ち
    if (!animation->IsPlayMainPartsAnimation())
    {
        // 待機ステートに遷移
        state_machine->ChangeState("IdleState");
        return;
    }
}

void PlayerDamageState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player)return;
    player->SetInputMoveValidityFlag(true);
}

PlayerDeadState::PlayerDeadState()
{
}

void PlayerDeadState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    // アニメーション再生
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
    {
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::DEAD, false);
        animation->PlaySubPartsAnimation(PlayerConstant::ANIMATION::DEAD, false);
    }
    // 入力受付をしないように設定
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
    {
        player->SetInputMoveValidityFlag(false);
    }

    // 下半身のアニメーション管理を停止させる
    const auto& animated_movement = owner->GetComponent(this->animated_movement_Wprt);
    if (animated_movement)
    {
        animated_movement->SetIsActive(false);
    }
}

void PlayerDeadState::Update(float elapsed_time)
{   
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    
    // アニメーション再生待ち
    if (!animation->IsPlayMainPartsAnimation())
    {
        // 死亡待機ステートに遷移
        state_machine->ChangeState("DeadIdleState");
        return;
    }
}

// 死亡ステート
PlayerDeadIdleState::PlayerDeadIdleState()
{
}

void PlayerDeadIdleState::Start()
{
    // アニメーション再生
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::DEAD_STAY, false);

    // ゲームモードを設定
    {
        GameData::Instance game_data = GameData::GetInstance();
        // 敗北状態に変更
        game_data->SetGameStatus(GameData::GameStatus::DEFEAT);
    }
}
