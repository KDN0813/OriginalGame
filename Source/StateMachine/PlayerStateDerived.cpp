#include "PlayerStateDerived.h"
#include "Input/Input.h"
#include "Object/Object.h"
#include "Object\Constant\PlayerConstant.h"
#include "Audio\Audio.h"

#include "Object\Constant\PlayerConstant.h"

#include "Component/StateMachineComponent.h"
#include "Component/ModelAnimationControlComponent.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CharacterComponent.h"

const MyHash PlayerIdleState::STATE_NAME = MyHash("PlayerIdleState");
PlayerIdleState::PlayerIdleState()
    : State(PlayerIdleState::STATE_NAME)
{
    this->change_move_state.change_state_name = PlayerMoveState::STATE_NAME;
    this->change_attack_state.change_state_name = PlayerAttackState::STATE_NAME;
    this->change_damage_state.change_state_name = PlayerDamageState::STATE_NAME;
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackState::STATE_NAME;
}

void PlayerIdleState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::IDLE, true, 0.2f);
}

void PlayerIdleState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_move_state);
        return;
    }

    // 被ダメ確認
    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character->IsDamage())
    {
        // 被ダメステートに遷移
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // 入力受付
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& pad = input->GetGamePad();
        // Xボタン
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            // 攻撃ステートへ遷移
            state_machine->ChangeState(this->change_attack_state);
        }
        // Yボタン
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            // 回転攻撃ステートに遷移
            state_machine->ChangeState(this->change_spin_attack_state);
            return;
        }
    }

    // TODO 自機死亡処理③
    // 死亡したらダメージステートに遷移
}

const MyHash PlayerMoveState::STATE_NAME = MyHash("PlayerMoveState");
PlayerMoveState::PlayerMoveState()
    : State(PlayerMoveState::STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
    this->change_damage_state.change_state_name = PlayerDamageState::STATE_NAME;
    this->change_attack_state.change_state_name = PlayerAttackState::STATE_NAME;
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackState::STATE_NAME;
}

void PlayerMoveState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true, 0.2f);
}

void PlayerMoveState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (!movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // 被ダメ確認
    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character->IsDamage())
    {
        // 被ダメステートに遷移
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // 入力受付
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& pad = input->GetGamePad();
        // Xボタン
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            // 攻撃ステートへ遷移
            state_machine->ChangeState(this->change_attack_state);
            return;
        }
        // Yボタン
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            // 回転攻撃ステートに遷移
            state_machine->ChangeState(this->change_spin_attack_state);
            return;
        }
    }

    // TODO 自機死亡処理③
    // 死亡したらダメージステートに遷移
}

const MyHash PlayerAttackState::STATE_NAME = MyHash("PlayerAttackState");
PlayerAttackState::PlayerAttackState()
    : State(PlayerAttackState::STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
}

void PlayerAttackState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // アニメーションの再生
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerConstant::ANIMATION::ATTACK01, false, 0.2f);

    // SE再生
    if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
    {
        AudioParam param{};
        param.volume = 0.5f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash01.wav";
        audio->Play(param);
    }

    // プレイヤーの入力移動を無効にする
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(false);

    // 攻撃判定オブジェクトを有効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // コリジョンを有効にする

    collision->EvaluateCollision();

    // 無敵状態に設定
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);
}

void PlayerAttackState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;

    // アニメーション再生待ち
    if (!animation->IsPlayAnimation())
    {
        // 待機ステートに遷移
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // TODO 自機死亡処理③
    // 死亡したらダメージステートに遷移
}

void PlayerAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // プレイヤーの入力移動を有効にする
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(true);

    // 攻撃判定オブジェクトを無効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // コリジョンを無効にする

    // 無敵状態を解除
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}

const MyHash PlayerSpinAttackState::STATE_NAME = MyHash("PlayerSpinAttackState");
PlayerSpinAttackState::PlayerSpinAttackState()
    :State(PlayerSpinAttackState::STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
}

void PlayerSpinAttackState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // アニメーションの再生
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK, false, 0.2f);

    // SE再生
    if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
    {
        AudioParam param{};
        param.volume = 0.4f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash02.wav";
        audio->Play(param);
    }

    // プレイヤーの入力移動を無効にする
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(false);

    // 攻撃判定オブジェクトを有効にする
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
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

    // アニメーション再生待ち
    if (!animation->IsPlayAnimation())
    {
        // 待機ステートに遷移
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // TODO 自機死亡処理③
    // 死亡したらダメージステートに遷移
}

void PlayerSpinAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // プレイヤーの入力移動を有効にする
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(true);

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


const MyHash PlayerDamageState::STATE_NAME = MyHash("PlayerDamagekState");
PlayerDamageState::PlayerDamageState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
    this->change_move_state.change_state_name = PlayerMoveState::STATE_NAME;
    this->change_attack_state.change_state_name = PlayerAttackState::STATE_NAME;
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackState::STATE_NAME;
}

void PlayerDamageState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::DAMAGE, false);

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

    // アニメーション再生待ち
    if (!animation->IsPlayAnimation())
    {
        // 待機ステートに遷移
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // TODO 自機死亡処理③
    // 死亡したらダメージステートに遷移
}

void PlayerDamageState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player)return;
    player->SetInputMoveValidityFlag(true);
}


const MyHash PlayerDeadState::STATE_NAME = MyHash("PlayerDeadState");
PlayerDeadState::PlayerDeadState()
    :State(STATE_NAME)
{
}

void PlayerDeadState::Staet()
{
    // TODO 自機死亡処理①_①
    // アニメーション再生
    // デスカメラに遷移
}

void PlayerDeadState::Update(float elapsed_time)
{
    // TODO 自機死亡処理①_②
    // アニメーション再生終了後死亡待機ステートに遷移

    // TODO 自機死亡処理④ ゲームモードを設定
}

// TODO 自機死亡処理②
// 死亡ステート作成
const MyHash PlayerDeadIdleState::STATE_NAME = MyHash("PlayerDeadIdleState");
PlayerDeadIdleState::PlayerDeadIdleState()
    :State(STATE_NAME)
{
}

void PlayerDeadIdleState::Staet()
{
    // TODO 自機死亡処理②
    // アニメーション再生
}

void PlayerDeadIdleState::Update(float elapsed_time)
{
}
