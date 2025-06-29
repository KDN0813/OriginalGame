#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#include <magic_enum.hpp>
#include "Debug/DebugManager.h"
#include "System/Misc.h"
#endif // _DEBUG
#include "CircleCollisionComponent.h"
#include "Collision/CircleCollisionManager.h"
#include "Object/Object.h"

#include "Component/TransformComponent.h"

CircleCollisionComponent::CircleCollisionComponent(CollisionParam param)
    :param(param), default_param(param)
{
    SetIsActive(this->param.default_active_flag);
}

void CircleCollisionComponent::Start()
{
#ifdef _DEBUG
    // デバッグプリミティブの設定
    {
        // タイプの値が以上な場合設定しない
        if (this->param.collision_type >= COLLISION_OBJECT_TYPE::MAX) return;
        
        DirectX::XMFLOAT4 color = { 0.0f,0.0f ,0.0f ,1.0f };
        switch (this->param.collision_type)
        {
        case COLLISION_OBJECT_TYPE::PLAYER_ATTACK:
            color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case COLLISION_OBJECT_TYPE::PLAYER_DEFENSE:
            color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case COLLISION_OBJECT_TYPE::ENEMY_ATTACK:
            color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case COLLISION_OBJECT_TYPE::ENEMY_DEFENSE:
            color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        default:
            break;
        }
        circle_collsion_primitive = CylinderParam(color,this->param.radius, height);
    }
#endif // DEBUG
}

void CircleCollisionComponent::End()
{
}

void CircleCollisionComponent::ReStart()
{
    this->param = this->default_param;
    this->hit_result = {};
    SetIsActive(this->param.default_active_flag);
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

void CircleCollisionComponent::EvaluateCollision()
{
    CircleCollisionManager::Instance manager = CircleCollisionManager::GetInstance();
    manager->EvaluateCollision(shared_from_this());
}

void CircleCollisionComponent::OnCollision(const std::shared_ptr<Object>& hit_object)
{
    _ASSERT_EXPR_W((hit_object != nullptr), "hit_objectがnullptrです");
    if (!hit_object) return;

    for (const auto& component_Wptr : this->collision_component_Wptr_pool)
    {
        if (const auto& component = component_Wptr.lock())
        {
            component->OnCollision(hit_object);
        }
    }
}

void CircleCollisionComponent::AddCollisionComponent(const std::shared_ptr<Component> component)
{
    this->collision_component_Wptr_pool.emplace_back(component);
}

CircleParam CircleCollisionComponent::GetCircleParam()
{
    CircleParam circle_param{};
    circle_param.radius = this->param.radius;

    auto owner = GetOwner();
    if (!owner) return circle_param;
    auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
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
    ImGui::DragFloat("radius##CircleCollisionComponent", &param.radius, 0.01f);

    // ヒットフラグ表示
    ImGui::Checkbox("Hit Flag", &param.hit_flag);
}

void CircleCollisionComponent::DrawDebugPrimitive()
{
    if (this->param.collision_type >= COLLISION_OBJECT_TYPE::MAX) return;    // タイプが以上なら処理しない
    
    DebugManager::Instance debug_manager = DebugManager::GetInstance();
    if (!debug_manager.Get()) return;
    DebugPrimitiveRenderer* debug_primitive_renderer = debug_manager->GetDebugPrimitiveRenderer();
    if (!debug_primitive_renderer) return;
    debug_primitive_renderer->DrawCylinder(circle_collsion_primitive);
}

void CircleCollisionComponent::DrawDebugPrimitiveGUI()
{
    if (this->param.collision_type >= COLLISION_OBJECT_TYPE::MAX) return;    // タイプが以上なら処理しない
    circle_collsion_primitive.DrawDebugGUI("Circle Collision");
}

#endif // DEBUG