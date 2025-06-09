#include "EnemyStateDerived.h"
#include <cmath>
#include "Object\Object.h"
#include "Object\GameObjectRegistry.h"
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

EnemyIdleState::EnemyIdleState()
{
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
        state_machine->ChangeState("WanderState");
        return;
    }

    // 移動範囲にプレイヤーが存在するか判定
    if (enemy->IsPlayerInMovementArea())
    {
        // 範囲内に存在すれば接近ステートに遷移
        state_machine->ChangeState("ChaseState");
        return;
    }

    // ダメージを受けたか判定
    if (character->IsDamage())
    {
        // ダメージを受けたならダメージステートに遷移
        state_machine->ChangeState("DamageState");
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState("DeadState");
        return;
    }
}

void EnemyIdleState::SetRandomIdleTime()
{
    this->idle_timer = MyMath::RandomRange(EnemyConstant::MIN_IDLE_TIME, EnemyConstant::MAX_IDLE_TIME);
}

EnemyWanderState::EnemyWanderState()
{
}

void EnemyWanderState::Start()
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

void EnemyWanderState::Update(float elapsed_time)
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
        state_machine->ChangeState("IdleState");
        return;
    }

    // 移動範囲にプレイヤーが存在するか判定
    if (enemy->IsPlayerInMovementArea())
    {
        // 範囲内に存在すれば接近ステートに遷移
        state_machine->ChangeState("ChaseState");
        return;
    }

    // ダメージを受けたか判定
    if (character->IsDamage())
    {
        // ダメージを受けたならダメージステートに遷移
        state_machine->ChangeState("DamageState");
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState("DeadState");
        return;
    }
}

void EnemyWanderState::End()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // 移動無効に設定
    enemy->SetMoveValidityFlag(false);
}

EnemyChaseState::EnemyChaseState()
{
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
#if 1   // 敵の重なりを防ぐ処理のため一時的に処理をスキップしてます
    if (enemy->IsPlayerInAttackArea())
    {
        // 範囲内にいるなら攻撃ステートに遷移
        state_machine->ChangeState("AttackState");
        return;
    }
#endif // 0

    // 移動範囲にプレイヤーが存在するか判定
    if (!enemy->IsPlayerInMovementArea())
    {
        // 範囲内にいないなら待機ステートに遷移
        state_machine->ChangeState("IdleState");
        return;
    }

    // ダメージを受けたか判定
    if (character->IsDamage())
    {
        // ダメージを受けたならダメージステートに遷移
        state_machine->ChangeState("DamageState");
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState("DeadState");
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

EnemyAttackState::EnemyAttackState()
{
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
            state_machine->ChangeState("ChaseState");
            return;
        }
        else
        {
            // 範囲内にいないなら待機ステートに遷移
            state_machine->ChangeState("IdleState");
            return;
        }
    }

    // ダメージを受けたか判定
    if (character->IsDamage())
    {
        // ダメージを受けたならダメージステートに遷移
        state_machine->ChangeState("DamageState");
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState("DeadState");
        return;
    }
}

void EnemyAttackState::End()
{
    // 攻撃判定オブジェクトを無効にする
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& attack_object = owner->FindChildObject(EnemyConstant::ATTACK_OBJECT_NAME);  // 子オブジェクト(攻撃用オブジェクト)取得
    if (!attack_object) return;
    const auto& collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(false);  // コリジョンを有効にする
}

EnemyDamageState::EnemyDamageState()
{
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
        state_machine->ChangeState("IdleState");
        return;
    }

    // 生存してるか判定
    if (!character->IsAlive())
    {
        // 生存していないなら死亡ステートに遷移
        state_machine->ChangeState("DeadState");
        return;
    }
}

EnemyDeadState::EnemyDeadState()
{
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

    // 削除までのタイムリミット設定
    this->remove_timer = EnemyConstant::REMOVE_IDLE_TIME;
}

void EnemyDeadState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& model = owner->GetComponent(animation_Wprt);
    if (!model)return;

    if (this->remove_timer > 0.0f)
    {
        // 削除タイマー更新
        this->remove_timer -= elapsed_time;
    }
    else
    {
        model->SetAlpha(0.0f);
        owner->SetIsRemove(true);   // 削除する
    }

    const float alpha = std::lerp(1.0f, 0.0f, this->remove_timer / EnemyConstant::REMOVE_IDLE_TIME);
    model->SetAlpha(1.0f - alpha);
}

EnemyDeadIdleState::EnemyDeadIdleState()
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
    const auto& model = owner->GetComponent(animation_Wprt);
    if (!model)return;

    if (this->remove_timer > 0.0f)
    {
        // 削除タイマー更新
        this->remove_timer -= elapsed_time;
    }
    else
    {
        model->SetAlpha(0.0f);
        owner->SetIsRemove(true);   // 削除する
    }

    const float alpha = std::lerp(1.0f, 0.0f, this->remove_timer / EnemyConstant::REMOVE_IDLE_TIME);
    model->SetAlpha(1.0f - alpha);
}

EnemySpawnState::EnemySpawnState()
{
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
        state_machine->ChangeState("ChaseState");
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
