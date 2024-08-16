#include "ShaderComponent.h"
#include "Shader/Shader.h"
#include "Object/Object.h"

#include "Shader/InstanceModelShader.h"

#include "Component/InstancingModelComponent.h"
#include "Component/TransformComponent.h"

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
    // Hack ÅŒã‚Ìobject‚Éƒ‚ƒfƒ‹‚ª‘¶Ý‚µ‚È‚¢‚Æ•`‰æ‚ª‚³‚ê‚È‚¢‚Ì‚Å‰ü‘P‚·‚é

    auto ownr = GetOwner();
    if (auto instancing_model
        = ownr->GetComponent<InstancingModelComponent>(this->instancing_model_Wptr))
    {
        this->shader->InstancingEnd(dc, instancing_model.get());
    }
}

#ifdef _DEBUG

void InstancingModelShaderComponent::DrawDebugGUI()
{
}

#endif _DEBUG