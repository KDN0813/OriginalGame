#include "GameObject.h"
#include <algorithm>
#include "Component/Component.h"

void GameObject::Update(float elapsedTime)
{
    sortComponentsByPriority();

    for (auto& component : components)
    {
        component->Update(elapsedTime);
    }
}

void GameObject::sortComponentsByPriority()
{
    auto sort_func = [](Component* lhs, Component* rhs) -> bool
        {
            return lhs->GetPriority() > rhs->GetPriority();
        };

    std::sort(components.begin(), components.end(), sort_func);
}
