#include "StateDerived.h"
#include "Object/Object.h"
#include "Component/EnemyComponent.h"

void WanderState::Start()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    enemy->SetRandomTargetPosition();
}

void WanderState::End()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
}

void IdelState::Start()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;

    enemy->SetRandomIdleTime();
}

void IdelState::End()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return;
    auto enemy = owner->EnsureComponentValid<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;

    enemy->SetIdleTime(0.0f);
}
