#include <imgui.h>
#ifdef _DEBUG
#include <magic_enum.hpp>
#endif // _DEBUG

#include "Object.h"
#include <algorithm>
#include <string>
#include "System/Misc.h"
#include "Component/Component.h"

std::shared_ptr<Object> Object::CreateChildObject()
{
    std::string name = GetName() + "_chilled" + std::to_string(this->children.size());
    return CreateChildObject(name.c_str());
}

std::shared_ptr<Object> Object::CreateChildObject(const char* name)
{
    std::shared_ptr<Object> object = std::make_unique<Object>();
    object->SetParent(shared_from_this());
    object->SetName(name);
    this->children.emplace_back(object);
    return object;
}

std::shared_ptr<Object> Object::FindChildObject(MyHash name)
{
    // 子オブジェクトを名前検索する
    for (const auto& child : this->children)
    {
        if (child->GetHash().PerfectEqual(name))
        {
            return child;
        }
    }
    _ASSERT_EXPR_W(false, L"子オブジェクトが存在しません");
    return nullptr;
}

void Object::Update(float elapsedTime)
{
    sortComponentsByPriority();
    
#ifdef _DEBUG
    if (!this->is_active) return;
#endif // _DEBUG

    for (auto& component : this->component_vec)
    {
        if (!component->GetIsActive()) continue;
#ifdef RELEASE_DEBUG
        component->DebugCheat();
#endif // _DEBUG

        component->Update(elapsedTime);
    }

    // 子オブジェクト更新
    for (auto chilled : this->children)
    {
        chilled->Update(elapsedTime);
    }
}

void Object::Start()
{
    for (std::shared_ptr<Component>& component : this->component_vec)
    {
        component->Start();
        // 子オブジェクト更新
        for (auto chilled : this->children)
        {
            chilled->Start();
        }
    }
}

void Object::End()
{
    for (std::shared_ptr<Component>& component : this->component_vec)
    {
        component->End();
        // 子オブジェクト更新
        for (auto chilled : this->children)
        {
            chilled->End();
        }
    }
}

void Object::ReStart()
{
    for (std::shared_ptr<Component>& component : this->component_vec)
    {
        component->SetIsActive(true);
        component->ReStart();
    }
}

const bool Object::GetIsActive()
{
    if (const auto& parent = this->parent_Wptr.lock())
    {
        if (!parent->GetIsActive()) return false;
    }
    return this->is_active;
}
void Object::sortComponentsByPriority()
{
    if (!this->is_sort_pending) return;

    auto sort_func = [](std::shared_ptr<Component>& lhs, std::shared_ptr<Component>& rhs) -> bool
        {
            return lhs->GetPriority() > rhs->GetPriority();
        };

    std::sort(component_vec.begin(), component_vec.end(), sort_func);

    this->is_sort_pending = false;
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

    ImGui::Separator();

    // Componentの名前検索

    if (ImGui::Button("clear##:component_search_name Reset"))
    {
        this->component_search_name.clear();
    }
    ImGui::SameLine();
    // 入力バッファ
    char buffer2[1024];
    ::strncpy_s(buffer2, sizeof(buffer2), this->component_search_name.c_str(), sizeof(buffer2));
    if (ImGui::InputText("Search Component", buffer2, sizeof(buffer2), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        this->component_search_name = buffer2;
    }

    // スペースをあける
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    int priority = 0;
    for (const std::shared_ptr<Component>& component : component_vec)
    {
        if (!this->component_search_name.empty())
        {
            if (!std::string(component->GetName()).starts_with(this->component_search_name)) 
            {
                continue;
            }
        }

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

            label = "ReStart##restart" + std::to_string(component->GetComponentID());
            if (ImGui::Button(label.c_str()))
            {
                component->ReStart();
            }

            if (component->IsDebugPrimitive())
            {
                ImGui::Indent(30.0f);

                bool is_debug_primitive = component->GetIsDebugPrimitive();
                label = "##" + std::to_string(component->GetComponentID()) + "debug_primitive";
                if (ImGui::Checkbox(label.c_str(), &is_debug_primitive))
                {
                    component->SetIsDebugPrimitive(is_debug_primitive);
                }
                ImGui::SameLine();
                
                if (!is_debug_primitive) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色

                label = "PrimitiveGUI##" + std::to_string(component->GetComponentID());
                if (ImGui::CollapsingHeader(label.c_str()))
                {
                    component->DrawDebugPrimitiveGUI();
                }
                ImGui::Unindent(30.0f);
                
                if (!is_debug_primitive) ImGui::PopStyleColor();
            }
        }
        ImGui::Separator();
        if (!component_is_active) ImGui::PopStyleColor();
    }

    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.5f, 0.5f, 1.0f));  // 緑色のボタン
    
    // is_off_screen
    ImGui::Checkbox("Is Off Screen", &this->is_off_screen);

    // リスタート
    if (ImGui::Button("All ReStart"))
    {
        ReStart();
    }
    // 削除ボタン
    if (ImGui::Button("Remove"))
    {
        this->is_remove = true;
    }
    ImGui::PopStyleColor();     // 色を元に戻す

    if (!this->is_active) ImGui::PopStyleColor();
}

void Object::DrawDebugPrimitive()
{
    if (!this->is_active) return;

    for (auto& component : this->component_vec)
    {
        if (!component->GetIsActive())continue;
        if (!component->GetIsDebugPrimitive())continue;
        component->DrawDebugPrimitive();

        // 子オブジェクト更新
        for (auto chilled : this->children)
        {
            chilled->DrawDebugPrimitive();
        }
    }
}

#endif _DEBUG


std::shared_ptr<Object> ObjectManager::Create()
{
    static int id = 0;
    char name[256];
    ::sprintf_s(name, sizeof(name), "object%d", id++);
    return Create(name);
}

std::shared_ptr<Object> ObjectManager::Create(const char* name)
{
    std::shared_ptr<Object> object = std::make_shared<Object>();
    {
        object->SetName(name);
    }
    this->start_object_vec.emplace_back(object);
    return object;
}

void ObjectManager::Start()
{
    for (std::shared_ptr<Object>& object : this->start_object_vec)
    {
        object->Start();
        this->update_object_vec.emplace_back(object);
    }
    this->start_object_vec.clear();
}

void ObjectManager::Update(float elapsedTime)
{
    Start();

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

        object->End();
    }
    this->remove_object_vec.clear();
}

void ObjectManager::Remove(std::shared_ptr<Object> object)
{
    this->remove_object_vec.insert(object);
}

void ObjectManager::ReStart()
{
    for (std::shared_ptr<Object>& object : this->update_object_vec)
    {
        object->SetIsActive(true);
        object->ReStart();
    }
}

#ifdef _DEBUG

void ObjectManager::DrawDebugGUI()
{
    DrawLister();
    DrawDetail();
}

void ObjectManager::DrawDebugPrimitive()
{
    for (std::shared_ptr<Object>& object : this->update_object_vec)
    {
        object->DrawDebugPrimitive();
    }
}

void ObjectManager::DrawLister()
{
    if (ImGui::Begin("Object"))
    {
        for (std::shared_ptr<Object>& object : update_object_vec)
        {
            DrawDebugNode(object);
        }
    }
    ImGui::End();
}

void ObjectManager::DrawDetail()
{
    ImGui::Begin("Object Detail", nullptr, ImGuiWindowFlags_None);

    std::shared_ptr<Object> last_selected = selection_object_vec.empty() ? nullptr : *selection_object_vec.rbegin();
    if (last_selected != nullptr)
    {
        last_selected->DrawDebugGUI();
    }

    ImGui::End();
}

void ObjectManager::DrawDebugNode(const std::shared_ptr<Object>& object)
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

    // 子オブジェクト描画
    const std::vector<std::shared_ptr<Object>>& children = object->GetChildren();
    if (children.size())
    {
        ImGui::Indent(10.0f);
        for (const std::shared_ptr<Object>& chilled : children)
        {
            DrawDebugNode(chilled);
        }
        ImGui::Unindent(10.0f);
    }
    return;
};

#endif // _DEBUG