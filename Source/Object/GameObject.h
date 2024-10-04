#pragma once
#include "System/ClassBase/Singleton.h"
#include "Object/Object.h"

class GameObject : public Singleton<GameObject>
{
public:
    enum class OBJECT_TYPE : size_t
    {
        STAGE = 0,
        PLAYER,

        MAX,
    };

public:
    GameObject();
    ~GameObject() {};

    std::shared_ptr<Object> GetGameObject(OBJECT_TYPE type);
    void SetGameObject(OBJECT_TYPE type,std::shared_ptr<Object> object);
private:
    std::vector<std::weak_ptr<Object>> game_object_pool;
};

