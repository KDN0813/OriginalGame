#include "Object.h"
#include <algorithm>
#include "Component/Component.h"

void Object::Update(float elapsedTime)
{
    //sortComponentsByPriority();

    for (auto& component : component_vector)
    {
        component->Update(elapsedTime);
    }
}

void Object::sortComponentsByPriority()
{
    auto sort_func = [](std::shared_ptr<Component>& lhs, std::shared_ptr<Component>& rhs) -> bool
        {
            return lhs->GetPriority() > rhs->GetPriority();
        };

    std::sort(component_vector.begin(), component_vector.end(), sort_func);
}


std::shared_ptr<Object> ObjectManager::Create()
{
    std::shared_ptr<Object> object = std::make_shared<Object>();
    {
        static int id = 0;
        char name[256];
        ::sprintf_s(name, sizeof(name), "object%d", id++);
        object->SetName(name);
    }
    this->game_object_vector.emplace_back(object);
    return object;
}

// TODO(08/13)続きここから
// とりあえずImGui表示するためにGameObjectManagerを作成する
// 名前長いのでObjectクラスに変更する
// こいつにRenderクラス持たせて、描画したいobjectだけDraw関数呼ぶとかできそう

void ObjectManager::Update(float elapsedTime)
{
    for (std::shared_ptr<Object>& object : game_object_vector)
    {
        object->Update(elapsedTime);
    }
}

void ObjectManager::DrawImGui()
{
    for (std::shared_ptr<Object>& object : game_object_vector)
    {
        object->DrawDebugGUI();
    }
}
