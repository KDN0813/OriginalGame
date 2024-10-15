#pragma once
#include "System/ClassBase/Singleton.h"

class CollisionManager : public Singleton<CollisionManager>
{
public:
    CollisionManager();
    ~CollisionManager() {};
};

