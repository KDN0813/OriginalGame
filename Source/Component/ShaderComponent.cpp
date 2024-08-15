#include "ShaderComponent.h"
#include "Shader/Shader.h"
#include "Object/Object.h"

#include "Component/InstancingModelComponent.h"

ShaderComponent::ShaderComponent(Shader* const shader)
    :shader(shader)
{
}

void ShaderComponent::Draw(ID3D11DeviceContext* dc)
{
    this->shader->Draw(dc);
}

bool ShaderComponent::SetInstancingResource()
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

void ShaderComponent::InstancingAdd()
{
    this->shader->InstancingAdd();
}

#ifdef _DEBUG

void ShaderComponent::DrawDebugGUI()
{
}

#endif _DEBUG