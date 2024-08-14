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

void ShaderComponent::SetInstancingResource()
{
    // TODO(08/14)ŠÖ”‰»‚µ‚½‚¢
    //if (bool expired = this->instancing_model_Wptr.expired(); expired == true)
    //{
    //    assert(expired);
    //
    //    int a = 0;
    //}
    //
    //this->shader->SetInstancingResource();
}

void ShaderComponent::InstancingAdd()
{
    //this->shader->InstancingAdd();
}

void ShaderComponent::GetWeakComponent()
{
    //auto owner = GetOwner();
    //this->instancing_model_Wptr = owner->GetComponent<InstancingModelComponent>();
}

#ifdef _DEBUG

void ShaderComponent::DrawDebugGUI()
{
}

#endif _DEBUG