#include <imgui.h>
#include "InstancingModelShaderComponent.h"
#include "Shader/Shader.h"
#include "Object/Object.h"

#include "Shader/InstanceModelShader.h"

#include "Component/InstancingModelComponent.h"
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
    if (!this->GetOwner()->GetIsActive()) return;
#endif // _DEBUG


    auto ownr = GetOwner();
    auto instancing_model
        = ownr->GetComponent<InstancingModelComponent>(this->instancing_model_Wptr);
    auto transform
        = ownr->GetComponent<Transform3DComponent>(this->transform_Wptr);

    if (instancing_model && transform)
    {
        this->shader->InstancingAdd(instancing_model.get(), transform.get());
    }
}

void InstancingModelShaderComponent::InstancingEnd(ID3D11DeviceContext* dc)
{
    auto ownr = GetOwner();
    if (auto instancing_model
        = ownr->GetComponent<InstancingModelComponent>(this->instancing_model_Wptr))
    {
        this->shader->InstancingEnd(dc, instancing_model.get());
    }
}

bool InstancingModelShaderComponent::IsShaderValid()
{
    auto ownr = GetOwner();
    auto instancing_model
        = ownr->GetComponent<InstancingModelComponent>(this->instancing_model_Wptr);
    auto transform
        = ownr->GetComponent<Transform3DComponent>(this->transform_Wptr);

    return (instancing_model && transform);
}

int InstancingModelShaderComponent::GetModelId()
{
    // Hack idを変数で持ち、返すようにしたら処理軽くなるかテストする
    auto ownr = GetOwner();
    auto instancing_model
        = ownr->GetComponent<InstancingModelComponent>(this->instancing_model_Wptr);
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