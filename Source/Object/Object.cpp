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

// TODO(08/13)������������
// �Ƃ肠����ImGui�\�����邽�߂�GameObjectManager���쐬����
// ���O�����̂�Object�N���X�ɕύX����
// ������Render�N���X�������āA�`�悵����object����Draw�֐��ĂԂƂ��ł�����

void ObjectManager::Update(float elapsedTime)
{
    for (std::shared_ptr<Object>& object : object_vector)
    {
        object->Update(elapsedTime);
    }
}

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

        if (selection_objects.find(object) != selection_objects.end())
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
            nodeFlags |= ImGuiTreeNodeFlags_Bullet;
        }

        // ��A�N�e�B�u�̃I�u�W�F�N�g�͊D�F�ɕ\��������
        bool is_active = (!object->GetIsActive());
        if (is_active) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// �D�F

        ImGui::TreeNodeEx(object.get(), nodeFlags, object->GetNameCStr());

        if (is_active) ImGui::PopStyleColor();

        // �I��
        if (ImGui::IsItemClicked())
        {
            // �P��I�������Ή����Ă���
            ImGuiIO& io = ImGui::GetIO();
            selection_objects.clear();
            selection_objects.insert(object);
        }
        // (��)�A�N�e�B�u��
        if (ImGui::IsItemClicked(1))
        {
            object->SetIsActive(!object->GetIsActive());
        }

        ImGui::TreePop();
    }
}

void ObjectManager::DrawDetail()
{
}
