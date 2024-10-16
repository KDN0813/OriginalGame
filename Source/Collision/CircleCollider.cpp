#include "CircleCollider.h"

#include "Component/CircleComponent.h"

void CircleCollider::Update()
{
    CheckCollision();
}

void CircleCollider::AddCircle(std::shared_ptr<CircleComponent>& circle)
{
    if (!circle) return;
    
    if (circle->GetCollisionType() & COLLISION_TYPE::ATTACKER)
    {

    }
    else if (circle->GetCollisionType() & COLLISION_TYPE::DEFENDER)
    {

    }
}

void CircleCollider::CheckCollision()
{
}

void CircleCollider::RemoveDeletedCircle()
{
}
