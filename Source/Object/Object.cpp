#include <imgui.h>
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

#ifdef _DEBUG

void Object::DrawDebugGUI()
{
    ImGui::Checkbox("##:", &this->is_active);

    ImGui::SameLine();

    // 非アクティブのオブジェクトは灰色に表示させる
    if (!this->is_active) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色

    char buffer[1024];
    ::strncpy_s(buffer, sizeof(buffer), GetNameCStr(), sizeof(buffer));
    if (ImGui::InputText("name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        SetName(buffer);
    }

    for (std::shared_ptr<Component>& component : component_vector)
    {
        if (ImGui::CollapsingHeader(component->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            component->DrawDebugGUI();
        }
    }

    if (!this->is_active) ImGui::PopStyleColor();
}

#endif _DEBUG


std::shared_ptr<Object> ObjectManager::Create()
{
    std::shared_ptr<Object> object = std::make_shared<Object>();
    {
        static int id = 0;
        char name[256];
        ::sprintf_s(name, sizeof(name), "object%d", id++);
        object->SetName(name);
    }
    this->object_vector.emplace_back(object);
    return object;
}

// TODO(08/13)続きここから
// とりあえずImGui表示するためにGameObjectManagerを作成する
// 名前長いのでObjectクラスに変更する
// こいつにRenderクラス持たせて、描画したいobjectだけDraw関数呼ぶとかできそう

void ObjectManager::Update(float elapsedTime)
{
    for (std::shared_ptr<Object>& object : object_vector)
    {
        object->Update(elapsedTime);
    }
}

#ifdef _DEBUG

void ObjectManager::DrawDebugGUI()
{
    DrawLister();
    DrawDetail();
}

void ObjectManager::DrawLister()
{
    for (std::shared_ptr<Object>& object : object_vector)
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf;

        if (selection_object_vector.find(object) != selection_object_vector.end())
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
            nodeFlags |= ImGuiTreeNodeFlags_Bullet;
        }

        // 非アクティブのオブジェクトは灰色に表示させる
        bool is_active = (!object->GetIsActive());
        if (is_active) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色

        ImGui::TreeNodeEx(object.get(), nodeFlags, object->GetNameCStr());

        if (is_active) ImGui::PopStyleColor();

        // 選択
        if (ImGui::IsItemClicked())
        {
            // 単一選択だけ対応しておく
            ImGuiIO& io = ImGui::GetIO();
            selection_object_vector.clear();
            selection_object_vector.insert(object);
        }
        // (非)アクティブ化
        if (ImGui::IsItemClicked(1))
        {
            object->SetIsActive(is_active);
        }

        ImGui::TreePop();
    }
}

void ObjectManager::DrawDetail()
{
    ImGui::SetNextWindowPos(ImVec2(970, 10.0f), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    ImGui::Begin("Object Detail", nullptr, ImGuiWindowFlags_None);
    std::shared_ptr<Object> lastSelected = selection_object_vector.empty() ? nullptr : *selection_object_vector.rbegin();
    if (lastSelected != nullptr)
    {
        lastSelected->DrawDebugGUI();
    }

    ImGui::End();
}

#endif // _DEBUG