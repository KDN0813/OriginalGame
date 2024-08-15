#include "ShaderComponent.h"
#include "Shader/Shader.h"
#include "Object/Object.h"

#include "Shader/InstanceModelShader.h"

#include "Component/InstancingModelComponent.h"

InstancingModelShaderComponent::InstancingModelShaderComponent(InstancingModelShader* const shader)
    :shader(shader)
{
}

void InstancingModelShaderComponent::Draw(ID3D11DeviceContext* dc)
{
    this->shader->Draw(dc);
}

bool InstancingModelShaderComponent::SetInstancingResource()
{    
    auto ownr = GetOwner();
    if (auto instancing_model 
        = ownr->GetComponent<InstancingModelComponent>(this->instancing_model_Wptr))
    {
        return this->shader->SetInstancingResource(
            instancing_model->GetModelResource(),
            instancing_model->GetInstancingModelResource()
        );
    }

    return false;
}

void InstancingModelShaderComponent::InstancingAdd()
{
    auto ownr = GetOwner();
    if (auto instancing_model
        = ownr->GetComponent<InstancingModelComponent>(this->instancing_model_Wptr))
    {
        this->shader->InstancingAdd(instancing_model->GetInstanceData());
    }
}

#ifdef _DEBUG

void InstancingModelShaderComponent::DrawDebugGUI()
{
}

#endif _DEBUG