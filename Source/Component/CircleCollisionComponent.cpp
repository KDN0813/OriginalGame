#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#include <magic_enum.hpp>
#include "Debug/DebugManager.h"
#endif // _DEBUG
#include "CircleCollisionComponent.h"
#include "Object/Object.h"
#include "Collision/CollisionManager.h"

#include "Component/TransformComponent.h"

CircleCollisionComponent::CircleCollisionComponent(CollisionParam param)
    :param(param), default_param(param)
{
}

void CircleCollisionComponent::Start()
{
#ifdef _DEBUG
    // デバッグプリミティブの設定
    {
        // タイプの値が以上な場合設定しない
        if (this->param.collision_type >= COLLISION_TYPE::MAX) return;
        DirectX::XMFLOAT4 color[2] = { DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) ,DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) };
        circle_collsion_primitive = CylinderParam(color[static_cast<size_t>(this->param.collision_type)],
            this->param.radius, height);
    }
#endif // DEBUG
}

void CircleCollisionComponent::End()
{
}

void CircleCollisionComponent::ReStart()
{
    this->param = this->default_param;
    this->hit_flag = false;
    this->hit_result = {};
}

void CircleCollisionComponent::Update(float elapsed_time)
{
#ifdef _DEBUG
    auto owner = GetOwner();
    if (owner)
    {
        auto transform = owner->GetComponent<Transform3DComponent>();
        if (transform)
        {
            this->circle_collsion_primitive.SetPosition(transform->GetWorldPosition());
            this->circle_collsion_primitive.SetRadius(this->param.radius);
        }
    }

#endif // _DEBUG
}

CircleParam CircleCollisionComponent::GetCircleParam()
{
    CircleParam circle_param{};
    circle_param.radius = this->param.radius;

    auto owner = GetOwner();
    if (!owner) return circle_param;
    auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
    if (!transform) return circle_param;

    DirectX::XMFLOAT3 world_pos = transform->GetWorldPosition();
    circle_param.center = DirectX::XMFLOAT2(world_pos.x, world_pos.z);
    return circle_param;
}

#ifdef _DEBUG

void CircleCollisionComponent::DrawDebugGUI()
{
    // collision_type表示
    {
        std::string text_str;
        auto a = magic_enum::enum_name(this->param.collision_type);
        text_str += magic_enum::enum_name(this->param.collision_type);
        ImGui::InputTextString("COLLISION TYPE", text_str);
    }
    // self_type表示
    {
        std::string text_str;
        text_str += magic_enum::enum_name(this->param.self_type);
        ImGui::InputTextString("SELF TYPE", text_str);
    }
    // target_type表示
    {
        std::string text_str;
        text_str += magic_enum::enum_name(this->param.target_type);
        ImGui::InputTextString("TARGET TYPE", text_str);
    }
    ImGui::DragFloat("radius##CircleCollisionComponent", &param.radius, 0.01f);
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

void CircleCollisionComponent::DrawDebugPrimitive()
{
    if (this->param.collision_type >= COLLISION_TYPE::MAX) return;    // タイプが以上なら処理しない
    DebugPrimitiveRenderer* debug_primitive_renderer = DebugManager::Instance()->GetDebugPrimitiveRenderer();
    debug_primitive_renderer->DrawCylinder(circle_collsion_primitive);
}

void CircleCollisionComponent::DrawDebugPrimitiveGUI()
{
    if (this->param.collision_type >= COLLISION_TYPE::MAX) return;    // タイプが以上なら処理しない
    circle_collsion_primitive.DrawDebugGUI("Circle Collsion");
}

#endif // DEBUG