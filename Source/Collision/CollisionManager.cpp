#include "CollisionManager.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


CollisionManager::CollisionManager()
    : Singleton(this)
{
    this->circle_collider = std::make_unique<CircleCollider>();
}

void CollisionManager::Update()
{
    this->circle_collider->Update();
}

#ifdef _DEBUG

void CollisionManager::DrawDebugGUI()
{
    this->circle_collider->DrawDebugGUI();
}

#endif // DEBUG