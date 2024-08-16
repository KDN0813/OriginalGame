#include "ModelShaderComponent.h"
#include "Shader/ModelShader.h"
#include "Object/Object.h"

#include "Component/ModelComponent.h"

ModelShaderComponent::ModelShaderComponent(ModelShader* const shader)
    :shader(shader)
{
}

void ModelShaderComponent::Draw(ID3D11DeviceContext* dc)
{
    auto owner = GetOwner();

    if (auto model = owner->GetComponent<ModelComponent>(this->model_Wptr))
    {
        shader->Draw(dc, model.get());
    }
}

#ifdef _DEBUG

void ModelShaderComponent::DrawDebugGUI()
{
}

#endif _DEBUG