#include "ModelShaderComponent.h"
#include "Shader/ModelShader.h"
#include "Object/Object.h"

#include "Component/ModelComponent.h"

ModelShaderComponent::ModelShaderComponent(ModelShader* const shader)
    :shader(shader)
{
    assert(shader);
}

void ModelShaderComponent::Start()
{
    this->shader->AddShaderComponent(this);
}

void ModelShaderComponent::End()
{
    this->shader->RemoveShaderComponent(this);
}

void ModelShaderComponent::Draw(ID3D11DeviceContext* dc)
{
#ifdef _DEBUG
    if (!this->is_active) return;
    if (!this->GetOwner()->GetIsActive()) return;
#endif // _DEBUG

    auto owner = GetOwner();

    if (auto model = owner->GetComponent<ModelComponent>(this->model_Wptr))
    {
        this->shader->Draw(dc, model.get());
    }
}

#ifdef _DEBUG

void ModelShaderComponent::DrawDebugGUI()
{
}

#endif _DEBUG