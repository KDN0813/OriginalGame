#include "CircleCollider.h"

#include "Component/CircleComponent.h"

void CircleCollider::Update()
{
    CheckCollision();
}

void CircleCollider::AddCircle(std::shared_ptr<CircleComponent> circle)
{
}

void CircleCollider::CheckCollision()
{
}

void CircleCollider::RemoveDeletedCircle()
{
}
