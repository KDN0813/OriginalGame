#include "CollisionManager.h"

CollisionManager::CollisionManager()
    : Singleton(this)
{
    circle_collider = std::make_unique<CircleCollider>();
}

void CollisionManager::Update()
{
    circle_collider->Update();
}
