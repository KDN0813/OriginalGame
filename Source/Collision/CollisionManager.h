#pragma once
#include <memory>
#include "System/ClassBase/Singleton.h"
#include "Collision/CircleCollider.h"

class CollisionManager : public Singleton<CollisionManager>
{
public:
    CollisionManager();
    ~CollisionManager() {};

    // �X�V�֐�
    void Update();

    // �e��擾�֐�
    CircleCollider* GetCircleCollider() const { return this->circle_collider.get(); }
private:
    std::unique_ptr<CircleCollider> circle_collider;
#ifdef _DEBUG
public:
    void DrawDebugGUI();
#endif // _DEBUG
};

