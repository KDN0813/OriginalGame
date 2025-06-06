#include "DebugParticle.h"
#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#include "Debug\DebugManager.h"
#endif // _DEBUG
#include "Shader\ParticleSystem.h"
#include "Object\Object.h"
#include "System\MyMath\MyMathf.h"
#include "../Hlsl/ParticleDisp.h"
#include "Camera\CameraManager.h"

#include "Component\TransformComponent.h"
#include "Component\CameraComponent.h"

DebugParticle::DebugParticle()
{
#ifdef _DEBUG
    this->debug_cylinder_effect_area = CylinderParam(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), this->effect_area_radius,this->effect_area_height);
#endif // _DEBUG
}

void DebugParticle::Update(float elapsed_time)
{
#ifdef _DEBUG
    this->debug_cylinder_effect_area.SetHeight(this->effect_area_height);
    this->debug_cylinder_effect_area.SetRadius(this->effect_area_radius);
#endif // _DEBUG

    if (const auto& owner = GetOwner())
    {
        if (const auto& transform = owner->GetComponent(this->transform_Wptr))
        {
            this->area_pos = transform->GetWorldPosition();
#ifdef _DEBUG
            this->debug_cylinder_effect_area.SetPosition(this->area_pos);
#endif // DEBUG
        }
    }

    if (this->effect_looping)
    {
        // エフェクトを連続再生させる
        for (int i = 0; i < loop_count; ++i)
        {
            PlayEffect(this->draw_effect_type);
        }
    }
}

void DebugParticle::PlayEffect(int type)
{
    DirectX::XMFLOAT3 direction{};
    {
        CameraManager::Instance camera_manager = CameraManager::GetInstance();
        if (const auto& camera = camera_manager->GetCurrentCamera())
        {
            direction = camera->GetForward();
        }
    }

    {
        ParticleSystem::Instance particle_system = ParticleSystem::GetInstance();
        const float theta = MyMath::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
        const float range = MyMath::RandomRange(0.0f, this->effect_area_radius);
        const float height = MyMath::RandomRange(0.0f, this->effect_area_height);

        float rot{};
        DirectX::XMFLOAT3 pos{ this->area_pos.x,this->area_pos.y ,this->area_pos.z };
        if (this->is_random_spawn_position)
        {
            pos.x += sinf(theta) * range;
            pos.y += height;
            pos.z += cosf(theta) * range;

            rot = theta;
        }

        particle_system->PlayEffect(type, pos, rot, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), direction);
    }
}

#ifdef _DEBUG

void DebugParticle::DrawDebugGUI()
{
    ImGui::DragFloat("Area Radius", &this->effect_area_radius, 0.1f);
    ImGui::DragFloat("Area Height", &this->effect_area_height, 0.1f);

    ImGui::Checkbox("Is Random Spawn Position", &this->is_random_spawn_position);

    ImGui::SliderInt("Effect Type", &this->draw_effect_type, 0, 2);
    ImGui::Checkbox("Effect Looping", &this->effect_looping);
    if (this->effect_looping)
    {
        ImGui::DragInt("Loop Count", &this->loop_count, 0, 300);
    }

    if(ImGui::Button("Effect Play"))
    {
        PlayEffect(this->draw_effect_type);
    }
    if (ImGui::Button("Play 10 Effects"))
    {
        for (size_t i = 0; i < 10; ++i)
        {
            PlayEffect(this->draw_effect_type);
        }
    }
    if (ImGui::Button("Play 100 Effects"))
    {
        for (size_t i = 0; i < 100; ++i)
        {
            PlayEffect(this->draw_effect_type);
        }
    }
    if (ImGui::Button("Play 1000 Effects"))
    {
        for (size_t i = 0; i < 1000; ++i)
        {
            PlayEffect(this->draw_effect_type);
        }
    }
}

void DebugParticle::DrawDebugPrimitive()
{
    {
        DebugManager::Instance debug_manager = DebugManager::GetInstance();
        if (DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer())
        {
            debug_render->DrawCylinder(this->debug_cylinder_effect_area);
        }
    }
}

void DebugParticle::DrawDebugPrimitiveGUI()
{
    this->debug_cylinder_effect_area.DrawDebugGUI("Effect Area");
}

#endif // DEBUG