#pragma once
#include <memory>
#include "System/ClassBase/Singleton.h"

class CollisionManager : public Singleton<CollisionManager>
{
public:
    CollisionManager();
    ~CollisionManager() {};

    // �X�V�֐�
    void Update();

    // �e��擾�֐�

private:

};

