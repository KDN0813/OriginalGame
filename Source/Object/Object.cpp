#include <imgui.h>
#include "Object.h"
#include <algorithm>
#include <string>
#include "Component/Component.h"

void Object::Update(float elapsedTime)
{
    // Hack コンポーネント追加・削除があったときだけ実行するようにしたら軽くなるかも
    sortComponentsByPriority();
    
#ifdef _DEBUG
    if (!this->is_active) return;
#endif // _DEBUG

    for (auto& component : component_vector)
    {
#ifdef _DEBUG
        if (!component->GetIsActive()) continue;
#endif // _DEBUG

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

    int priority = 0;
    for (std::shared_ptr<Component>& component : component_vector)
    {
        bool component_is_active = component->GetIsActive();

        // (非)アクティブ設定
        std::string label = "##" + std::to_string(component->GetComponentID());
        if (ImGui::Checkbox(label.c_str(), &component_is_active))
        {
            component->SetIsActive(component_is_active);
        }

        ImGui::SameLine();

        // 非アクティブのオブジェクトは灰色に表示させる
        if (!component_is_active) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色
        
        if (ImGui::CollapsingHeader(component->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            label = "priority:" + std::to_string(priority++);
            ImGui::Text(label.c_str());
            label = "priority Level:" + std::to_string(component->GetPriority());
            ImGui::Text(label.c_str());

            component->DrawDebugGUI();
        }

        if (!component_is_active) ImGui::PopStyleColor();
    }

    // 削除ボタン
    if (ImGui::Button("Remove"))
    {
        this->is_remove = true;
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

void ObjectManager::Update(float elapsedTime)
{
    for (std::shared_ptr<Object>& object : this->object_vector)
    {
        object->Update(elapsedTime);
        if (object->GetIsRemove()) Remove(object);
    }

    for (const std::shared_ptr<Object>& object : this->remove_object_vector)
    {
        std::vector<std::shared_ptr<Object>>::iterator it = std::find(this->object_vector.begin(), this->object_vector.end(), object);
        if (it != object_vector.end())
        {
            this->object_vector.erase(it);
        }
#ifdef _DEBUG
        std::set<std::shared_ptr<Object>>::iterator selection_it = std::find(this->selection_object_vector.begin(), this->selection_object_vector.end(), object);
        if (selection_it != selection_object_vector.end())
        {
            this->selection_object_vector.erase(selection_it);
        }
#endif // _DEBUG
    }
    this->remove_object_vector.clear();
}

void ObjectManager::Remove(std::shared_ptr<Object> object)
{
    this->remove_object_vector.insert(object);
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
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

    ImGui::Begin("Object Detail", nullptr, ImGuiWindowFlags_None);

    std::shared_ptr<Object> last_selected = selection_object_vector.empty() ? nullptr : *selection_object_vector.rbegin();
    if (last_selected != nullptr)
    {
        last_selected->DrawDebugGUI();
    }

    ImGui::End();
}

#endif // _DEBUG