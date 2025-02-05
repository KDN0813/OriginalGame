#include "EnemyStateDerived.h"
#include <cmath>
#include "Object\Object.h"
#include "Object\GameObject.h"
#include "Object\Constant\EnemyConstant.h"
#include "System\MyMath\MyMathf.h"
#include "System\MyMath\MYVECTOR3.h"
#include "System\GameData.h"

#include "Component\StateMachineComponent.h"
#include "Component\EnemyComponent.h"
#include "Component\InstancedModelWithAnimationComponent.h"
#include "Component\TransformComponent.h"
#include "Component\CircleCollisionComponent.h"
#include "Component\CharacterComponent.h"
#include "Component\PlayerComponent.h"

const MyHash EnemyIdleState::STATE_NAME = MyHash("EnemyIdleState");
EnemyIdleState::EnemyIdleState()
    :State(STATE_NAME)
{
    this->change_wandering_state.change_state_name = EnemyWanderingState::STATE_NAME;
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
    this->change_attack_state.change_state_name = EnemyAttackState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyIdleState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // アニメーション再生
    animation->PlayAnimation(EnemyConstant::IDLE_NORMAL, true);

    // 待機時間設定
    SetRandomIdleTime();
}

void EnemyIdleState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    if (this->idle_timer > 0.0f)
    {
        // 待機タイマー更新
        this->idle_timer -= elapsed_time;
    }
    else
    {
        // 待機時間が終了したら移動状態に遷移
        state_machine->ChangeState(this->change_wandering_state);
        return;
    }

    // 移動範囲にプレイヤーが存在するか判定
    if (enemy->IsPlayerInMovementArea())
    {
        // 範囲内に存在すれば接近ステートに遷移
        state_machine->ChangeState(this->change_chase_state);
        return;
    }

    // ダメージを受けたか判定
    if (character->IsDamage())
    {
        // ダメージを受けたならダメージステートに遷移
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

void EnemyIdleState::SetRandomIdleTime()
{
    this->idle_timer = MyMath::RandomRange(EnemyConstant::MIN_IDLE_TIME, EnemyConstant::MAX_IDLE_TIME);
}

const MyHash EnemyWanderingState::STATE_NAME = MyHash("EnemyWanderingState");
EnemyWanderingState::EnemyWanderingState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
    this->change_attack_state.change_state_name = EnemyAttackState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyWanderingState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // アニメーション再生
    animation->PlayAnimation(EnemyConstant::MOVE_FWD, true);

    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // 移動有効に設定
    enemy->SetMoveValidityFlag(true);
    // ターゲット設定
    enemy->SetRandomTargetPosition();
}

void EnemyWanderingState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    if (enemy->IsAtTarget())
    {
        // 目的地に到着したら待機状態に遷移
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // 移動範囲にプレイヤーが存在するか判定
    if (enemy->IsPlayerInMovementArea())
    {
        // 範囲内に存在すれば接近ステートに遷移
        state_machine->ChangeState(this->change_chase_state);
        return;
    }

    // ダメージを受けたか判定
    if (character->IsDamage())
    {
        // ダメージを受けたならダメージステートに遷移
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

void EnemyWanderingState::End()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // 移動無効に設定
    enemy->SetMoveValidityFlag(false);
}

const MyHash EnemyChaseState::STATE_NAME = MyHash("EnemyChaseState");
EnemyChaseState::EnemyChaseState()
    :State(STATE_NAME)
{
    this->change_attack_state.change_state_name = EnemyAttackState::STATE_NAME;
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyChaseState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // アニメーション再生
    animation->PlayAnimation(EnemyConstant::MOVE_FWD, true);

    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // 移動有効に設定
    enemy->SetMoveValidityFlag(true);
}

void EnemyChaseState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    // 目的地をプレイヤーに設定
    enemy->SetTargetPositionByPlayer();

    // 攻撃範囲にプレイヤーが存在するか
#if 0   // 敵の重なりを防ぐ処理のため一時的に処理をスキップしてます
    if (enemy->IsPlayerInAttaclArea())
    {
        // 範囲内にいるなら攻撃ステートに遷移
        state_machine->ChangeState(this->change_attack_state);
        return;
    }
#endif // 0

    // 移動範囲にプレイヤーが存在するか判定
    if (!enemy->IsPlayerInMovementArea())
    {
        // 範囲内にいないなら待機ステートに遷移
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // ダメージを受けたか判定
    if (character->IsDamage())
    {
        // ダメージを受けたならダメージステートに遷移
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

void EnemyChaseState::End()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // 移動無効に設定
    enemy->SetMoveValidityFlag(false);
}

const MyHash EnemyAttackState::STATE_NAME = MyHash("EnemyAttackState");
EnemyAttackState::EnemyAttackState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyAttackState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // アニメーション再生
    animation->PlayAnimation(EnemyConstant::ATTACK01, false);

    // 攻撃判定オブジェクトを有効にする
    const auto& attack_object = owner->FindChildObject(EnemyConstant::ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // コリジョンを有効にする

    collision->EvaluateCollision();
}

void EnemyAttackState::Update(float elapsed_time)
// 移動処理
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    // アニメーション再生中であるか
    if (!animation->IsPlayAnime())
    {
        // 再生が終わっていたら

        if (enemy->IsPlayerInMovementArea())
        {
            // 範囲内に存在すれば接近ステートに遷移
            state_machine->ChangeState(this->change_chase_state);
            return;
        }
        else
        {
            // 範囲内にいないなら待機ステートに遷移
            state_machine->ChangeState(this->change_idle_state);
            return;
        }
    }

    // ダメージを受けたか判定
    if (character->IsDamage())
    {
        // ダメージを受けたならダメージステートに遷移
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

void EnemyAttackState::End()
{
}

const MyHash EnemyDamageState::STATE_NAME = MyHash("EnemyDamageState");
EnemyDamageState::EnemyDamageState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyDamageState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // アニメーション再生
    animation->PlayAnimation(EnemyConstant::TAUNTING, false);
}

void EnemyDamageState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    if (!animation->IsPlayAnime())
    {
        // アニメーションが終了待機状態に遷移
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

const MyHash EnemyDeadState::STATE_NAME = MyHash("EnemyDeadState");
EnemyDeadState::EnemyDeadState()
    :State(STATE_NAME)
{
    this->change_deth_idle_state.change_state_name = EnemyDeadIdleState::STATE_NAME;
}

void EnemyDeadState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(enemy_Wptr);
    if (!enemy)return;

    // アニメーション再生
    animation->PlayAnimation(EnemyConstant::DIE, false);

    // 死亡待ちフラグを立てる
    enemy->SetPendingRemovalFlag(true);
}

void EnemyDeadState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    if (!animation->IsPlayAnime())
    {
        // アニメーションが終了待機状態に遷移
        state_machine->ChangeState(this->change_deth_idle_state);
        return;
    }
}

const MyHash EnemyDeadIdleState::STATE_NAME = MyHash("EnemyDeadIdleState");
EnemyDeadIdleState::EnemyDeadIdleState()
    :State(STATE_NAME)
{
}

void EnemyDeadIdleState::Start()
{
    this->remove_timer = EnemyConstant::REMOVE_IDLE_TIME;
}

void EnemyDeadIdleState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;

    if (this->remove_timer > 0.0f)
    {
        // 削除タイマー更新
        this->remove_timer -= elapsed_time;
    }
    else
    {
        owner->SetIsRemove(true);   // 削除する
    }
}

const MyHash EnemySpawnState::STATE_NAME = MyHash("EnemySpawnState");
EnemySpawnState::EnemySpawnState()
    :State(STATE_NAME)
{
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
}

void EnemySpawnState::Start()
{
    this->idle_timer = 0.0f;    // タイマーリセット

    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // アニメーション再生
    animation->PlayAnimation(EnemyConstant::IDLE_NORMAL, true);

    // alpha値を0に設定
    animation->SetAlpha(0.0f);

    // 無敵状態に設定
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);

    // 移動停止
    const auto& enemy = owner->GetComponent(enemy_Wptr);
    if (!enemy) return;
    enemy->SetMoveValidityFlag(false);
}

void EnemySpawnState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;

    // モデルの透明度設定
    const float alpha = std::lerp(0.0f, 1.0f, this->idle_timer / this->IDLE_TIME);

    if (this->idle_timer < this->IDLE_TIME)
    {
        // 待機タイマー更新
        this->idle_timer += elapsed_time;
    }
    else
    {
        // 待機時間が終了したら移動状態に遷移
        state_machine->ChangeState(this->change_chase_state);
        animation->SetAlpha(1.0f);
        return;
    }

    animation->SetAlpha(alpha);
}

void EnemySpawnState::End()
{
    const auto& owner = GetOwner();
    if (!owner)return;

    // 移動許可
    const auto& enemy = owner->GetComponent(enemy_Wptr);
    if (!enemy) return;
    enemy->SetMoveValidityFlag(false);

    // 無敵状態解除
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}
