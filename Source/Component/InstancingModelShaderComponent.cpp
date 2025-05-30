#include <imgui.h>
#include "InstancingModelShaderComponent.h"
#include "Shader/Shader.h"
#include "Object/Object.h"

#include "Shader/InstanceModelShader.h"
#include "Camera\CameraManager.h"

#include "Component/InstancedModelWithAnimationComponent.h"
#include "Component/TransformComponent.h"

InstancingModelShaderComponent::InstancingModelShaderComponent(InstancingModelShader* const shader)
    :shader(shader)
{
}

void InstancingModelShaderComponent::Start()
{
    this->shader->AddShaderComponent(this);
}

void InstancingModelShaderComponent::End()
{
    this->shader->RemoveShaderComponent(this);
}

void InstancingModelShaderComponent::InstancingStart()
{
    this->shader->InstancingStart();
}

void InstancingModelShaderComponent::InstancingAdd()
{
#ifdef _DEBUG
    if (!this->is_active) return;
#endif // _DEBUG
    if (!this->GetOwner()->GetIsActive()) return;

    auto owner = GetOwner();
    if (!owner) return;
    auto instancing_model
        = owner->GetComponent<InstancedModelWithAnimationComponent>(this->instancing_model_Wptr);
    auto transform
        = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);

    {
        CameraManager::Instance camera_manager = CameraManager::GetInstance();
        const bool is_off_screen = (!camera_manager->IsAnyMeshAABBVisible(instancing_model->GetBoundingBoxs()));
        owner->SetIsOffScreen(is_off_screen);
        if (is_off_screen) return;
    }

    if (instancing_model && transform)
    {
        this->shader->InstancingAdd(instancing_model.get(), transform.get());
    }
}

void InstancingModelShaderComponent::InstancingEnd(ID3D11DeviceContext* dc)
{
    auto owner = GetOwner();
    if (!owner) return;
    if (auto instancing_model
        = owner->GetComponent<InstancedModelWithAnimationComponent>(this->instancing_model_Wptr))
    {
        this->shader->InstancingEnd(dc, instancing_model.get());
    }
}

bool InstancingModelShaderComponent::IsShaderValid()
{
    auto owner = GetOwner();
    if (!owner) return false;
    auto instancing_model
        = owner->GetComponent<InstancedModelWithAnimationComponent>(this->instancing_model_Wptr);
    auto transform
        = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);

    return (instancing_model && transform);
}

int InstancingModelShaderComponent::GetModelId()
{
    auto owner = GetOwner();
    if (!owner) return -1;
    auto instancing_model
        = owner->GetComponent<InstancedModelWithAnimationComponent>(this->instancing_model_Wptr);
    if (!instancing_model) return-1;
    return instancing_model->GetModelId();
}

#ifdef _DEBUG

void InstancingModelShaderComponent::DrawDebugGUI()
{
    std::string shader_name = this->shader->GetName();
    std::string text = "SetShader:" + shader_name;
    ImGui::Text(text.c_str());
}

#endif _DEBUG