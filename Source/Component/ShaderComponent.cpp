#include "ShaderComponent.h"
#include "Shader/Shader.h"
#include "Object/Object.h"

#include "Shader/InstanceModelShader.h"

#include "Component/InstancingModelComponent.h"

InstancingModelShaderComponent::InstancingModelShaderComponent(InstancingModelShader* const shader)
    :shader(shader)
{
}

void InstancingModelShaderComponent::InstancingStart()
{
    this->shader->InstancingStart();
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

void InstancingModelShaderComponent::InstancingEnd()
{
    this->shader->InstancingEnd();
}

#ifdef _DEBUG

void InstancingModelShaderComponent::DrawDebugGUI()
{
}

#endif _DEBUG