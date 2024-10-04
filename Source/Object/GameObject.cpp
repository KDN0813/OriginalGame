#include "GameObject.h"

GameObject::GameObject()
    :Singleton(this)
{
    this->game_object_pool.resize(static_cast<size_t>(OBJECT_TYPE::MAX));
}

std::shared_ptr<Object> GameObject::GetGameObject(OBJECT_TYPE type)
{
    return this->game_object_pool[static_cast<size_t>(type)].lock();
}

void GameObject::SetGameObject(OBJECT_TYPE type, std::shared_ptr<Object> object)
{
    this->game_object_pool[static_cast<size_t>(type)] = object;
}
