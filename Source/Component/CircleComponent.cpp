#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#include <magic_enum.hpp>
#endif // _DEBUG
#include "CircleComponent.h"
#include "Object/Object.h"

#include "Component/TransformComponent.h"

void CircleComponent::Start()
{
}

void CircleComponent::End()
{
}

void CircleComponent::Update(float elapsed_time)
{
}

CircleParam CircleComponent::GetCircleParam()
{
    auto owner = GetOwner();
    if (!owner) return CircleParam(DirectX::XMFLOAT2(), this->radius);
    auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
    if(!transform) return CircleParam(DirectX::XMFLOAT2(), this->radius);

    DirectX::XMFLOAT3 world_pos = transform->GetWorldPosition();
    return CircleParam(DirectX::XMFLOAT2(world_pos.x, world_pos.z), this->radius);
}

void CircleComponent::DrawDebugGUI()
{
    // collision_type表示
    {
        std::string text_str;
        auto a = magic_enum::enum_name(this->collision_type);
        text_str += magic_enum::enum_name(this->collision_type);
        ImGui::InputTextString("COLLISION TYPE", text_str);
    }
    // self_type表示
    {
        std::string text_str;
        text_str += magic_enum::enum_name(this->self_type);
        ImGui::InputTextString("SELF TYPE", text_str);
    }
    // target_type表示
    {
        std::string text_str;
        text_str += magic_enum::enum_name(this->target_type);
        ImGui::InputTextString("TARGET TYPE", text_str);
    }
    ImGui::DragFloat("radius##CircleComponent", &radius, 0.01f);
    ImGui::Checkbox("Hit Flag" ,&hit_flag);

    // hit_result表示
    {
        ImGui::Spacing();
        if(ImGui::CollapsingHeader("Circle Hit Result"))
        {
            std::string hit_object_name;
            auto hit_object = hit_result.hit_object_Wptr.lock();
            if (hit_object) hit_object_name = hit_object->GetName();
            else hit_object_name = "null";

            ImGui::InputTextString("Hit Object Name", hit_object_name);
        }
    }
}
