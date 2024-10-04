#include <imgui.h>
#ifdef _DEBUG
#include <magic_enum.hpp>
#endif // _DEBUG

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

    for (auto& component : component_vec)
    {
#ifdef _DEBUG
        if (!component->GetIsActive()) continue;
#endif // _DEBUG

        component->Update(elapsedTime);
    }
}

void Object::Initialize()
{
    for (std::shared_ptr<Component>& component : this->component_vec)
    {
        component->Start();
    }
}

void Object::Finalize()
{
    for (std::shared_ptr<Component>& component : this->component_vec)
    {
        component->End();
    }
}

void Object::sortComponentsByPriority()
{
    auto sort_func = [](std::shared_ptr<Component>& lhs, std::shared_ptr<Component>& rhs) -> bool
        {
            return lhs->GetPriority() > rhs->GetPriority();
        };

    std::sort(component_vec.begin(), component_vec.end(), sort_func);
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
    for (std::shared_ptr<Component>& component : component_vec)
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
            label = "Priority:";
            label += magic_enum::enum_name(component->GetPriority());
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
    this->start_object_vec.emplace_back(object);
    return object;
}

void ObjectManager::Update(float elapsedTime)
{
    for (std::shared_ptr<Object>& object : this->start_object_vec)
    {
        object->Initialize();
        this->update_object_vec.emplace_back(object);
    }
    this->start_object_vec.clear();

    for (std::shared_ptr<Object>& object : this->update_object_vec)
    {
        object->Update(elapsedTime);
        if (object->GetIsRemove()) Remove(object);
    }

    for (const std::shared_ptr<Object>& object : this->remove_object_vec)
    {
        std::vector<std::shared_ptr<Object>>::iterator start_it = std::find(this->start_object_vec.begin(), this->start_object_vec.end(), object);
        if (start_it != this->start_object_vec.end())
        {
            this->start_object_vec.erase(start_it);
        }

        std::vector<std::shared_ptr<Object>>::iterator update_it = std::find(this->update_object_vec.begin(), this->update_object_vec.end(), object);
        if (update_it != this->update_object_vec.end())
        {
            this->update_object_vec.erase(update_it);
        }

#ifdef _DEBUG
        std::set<std::shared_ptr<Object>>::iterator selection_it = std::find(this->selection_object_vec.begin(), this->selection_object_vec.end(), object);
        if (selection_it != this->selection_object_vec.end())
        {
            this->selection_object_vec.erase(selection_it);
        }
#endif // _DEBUG

        object->Finalize();
    }
    this->remove_object_vec.clear();
}

void ObjectManager::Remove(std::shared_ptr<Object> object)
{
    this->remove_object_vec.insert(object);
}

#ifdef _DEBUG

void ObjectManager::DrawDebugGUI()
{
    DrawLister();
    DrawDetail();
}

void ObjectManager::DrawLister()
{
    for (std::shared_ptr<Object>& object : update_object_vec)
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf;

        if (selection_object_vec.find(object) != selection_object_vec.end())
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
            selection_object_vec.clear();
            selection_object_vec.insert(object);
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
    ImGui::SetNextWindowPos(ImVec2(1425, 10.0f), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

    ImGui::Begin("Object Detail", nullptr, ImGuiWindowFlags_None);

    std::shared_ptr<Object> last_selected = selection_object_vec.empty() ? nullptr : *selection_object_vec.rbegin();
    if (last_selected != nullptr)
    {
        last_selected->DrawDebugGUI();
    }

    ImGui::End();
}

#endif // _DEBUG