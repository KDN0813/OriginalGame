#include "DebugParticle.h"
#ifdef _DEBUG
#include <imgui.h>
#include "Debug\DebugManager.h"
#endif // _DEBUG
#include "Shader\ParticleSystem.h"

DebugParticle::DebugParticle()
{
#ifdef _DEBUG
    this->debug_cylinder_effect_area = CylinderParam(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), this->effect_area_radius,this->effect_area_height);

#endif // _DEBUG
}

#ifdef _DEBUG

void DebugParticle::DrawDebugGUI()
{
    ImGui::DragFloat("Area Radius", &this->effect_area_radius, 0.1f);
    ImGui::DragFloat("Area Height", &this->effect_area_height, 0.1f);

    if(ImGui::Button("Effect Play"))
    {
        if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
        {
            DirectX::XMFLOAT3 pos{};
            float angle{};

            particle_system->Play(pos, angle);
        }
    }
}

void DebugParticle::DrawDebugPrimitive()
{
    this->debug_cylinder_effect_area.DrawDebugGUI("Effect Area");
}

void DebugParticle::DrawDebugPrimitiveGUI()
{
    DebugManager::Instance debug_manager = DebugManager::GetInstance();

    DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer();
    debug_render->DrawCylinder(this->debug_cylinder_effect_area);
}

#endif // DEBUG