#include "DebugParticle.h"
#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#include "Debug\DebugManager.h"
#endif // _DEBUG
#include "Shader\ParticleSystem.h"
#include "Object\Object.h"
#include "System\MyMath\MyMathf.h"
#include "../Hlsl/ParticleDisp.h"

#include "Component\TransformComponent.h"

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
        if (const auto& transform = owner->EnsureComponentValid(this->transform_Wptr))
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
        PlayEffect();
    }
}

void DebugParticle::PlayEffect()
{
    if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
    {
        const float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
        const float range = MyMathf::RandomRange(0.0f, this->effect_area_radius);
        const float height = MyMathf::RandomRange(0.0f, this->effect_area_height);

        DirectX::XMFLOAT3 pos
        {
            this->area_pos.x + sinf(theta) * range,
            this->area_pos.y + height,
            this->area_pos.z + cosf(theta) * range ,
        };

        particle_system->PlayEffect(pos, DirectX::XMFLOAT3(1.0f, 0.5f, 1.0f), EFFECT_SLASH, theta);
    }
}

void DebugParticle::PlayGroupEffect(size_t count)
{
    if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
    {
        if (particle_system->GetFreeParticleCount() < count) return;   // 空き数より数が多いならreturnする

        std::vector<ParticleSystem::ParticleParam>  particle_pool;
        particle_pool.resize(count);
        for (ParticleSystem::ParticleParam& particle : particle_pool)
        {
            // 色設定
            particle.color = DirectX::XMFLOAT3(1.0f, 0.5f, 1.0f);

            const float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
            const float range = MyMathf::RandomRange(0.0f, this->effect_area_radius);
            const float height = MyMathf::RandomRange(this->effect_area_height - this->effect_area_height * 0.2, this->effect_area_height);
            // 生成位置設定
            particle.position =
            {
                this->area_pos.x + sinf(theta) * range,
                this->area_pos.y + height,
                this->area_pos.z + cosf(theta) * range ,
            };

            // 角度設定
            particle.rot = theta;
            // タイプ設定
            particle.type = EFFECT_FALL_SLASH;
        }

        // エフェクト再生
        particle_system->PlayGroupEffect(particle_pool);
    }
}

#ifdef _DEBUG

void DebugParticle::DrawDebugGUI()
{
    ImGui::DragFloat("Area Radius", &this->effect_area_radius, 0.1f);
    ImGui::DragFloat("Area Height", &this->effect_area_height, 0.1f);

    ImGui::Checkbox("Effect Looping", &this->effect_looping);

    if(ImGui::Button("Effect Play"))
    {
        PlayEffect();
    }
    if (ImGui::Button("Play 10 Effects"))
    {
        for (size_t i = 0; i < 10; ++i)
        {
            PlayEffect();
        }
    }
    if (ImGui::Button("Play 100 Effects"))
    {
        for (size_t i = 0; i < 100; ++i)
        {
            PlayEffect();
        }
    }
    if (ImGui::Button("Play 1000 Effects"))
    {
        for (size_t i = 0; i < 1000; ++i)
        {
            PlayEffect();
        }
    }
    if (ImGui::TreeNodeEx("Play Group", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputSize_t("Group Count", this->group_count);
        if (ImGui::Button("Play Group Effects"))
        {
            PlayGroupEffect(this->group_count);
        }
        ImGui::TreePop();
    }
}

void DebugParticle::DrawDebugPrimitive()
{
    if (DebugManager::Instance debug_manager = DebugManager::GetInstance(); debug_manager.Get())
    {
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