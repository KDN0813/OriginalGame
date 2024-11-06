#include "LegacyStateDerived.h"
#include "Object/Object.h"

#include "Component/EnemyComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/InstancedModelWithStateAnimationComponent.h"

void Legacy_WanderState::Start()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    enemy->SetRandomTargetPosition();
}

void Legacy_WanderState::End()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
}

void Legacy_IdelState::Start()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;

    enemy->SetRandomIdleTime();
}

void Legacy_IdelState::End()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;

    enemy->SetIdleTime(0.0f);
}

void Legacy_AttackState::Start()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto circle_collision = owner->EnsureComponentValid<CircleCollisionComponent>(this->circle_collision_Wptr);
    if (!circle_collision) return;

    circle_collision->SetIsActive(true);
}

void Legacy_AttackState::Update(float elapsed_time)
{
}

void Legacy_AttackState::End()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto circle_collision = owner->EnsureComponentValid<CircleCollisionComponent>(this->circle_collision_Wptr);
    if (!circle_collision) return;

    circle_collision->SetIsActive(false);
}

void Legacy_DamageState::Start()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    enemy->SetMoveValidityFlag(false);  // ˆÚ“®•s‰Â‚ÉÝ’è
}

void Legacy_DamageState::Update(float elapsed_time)
{
    int a = 0;
}

void Legacy_DamageState::End()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    enemy->SetMoveValidityFlag(true);  // ˆÚ“®‰Â‚ÉÝ’è
}

bool Legacy_DamageState::IsTransitionReady()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return true;
    const auto& model = owner->EnsureComponentValid<InstancedModelWithStateAnimationComponent>(this->model_Wptr);
    if (!model) return true;

    // ƒ‚ƒfƒ‹‚Ì‘JˆÚ€”õ‚ª‚Å‚«‚Ä‚¢‚é‚©
    return model->IsTransitionReady();
}
