#include "ShaderComponent.h"
#include "Shader/Shader.h"

ShaderComponent::ShaderComponent(Shader* const shader)
    :shader(shader)
{
}

void ShaderComponent::Draw(ID3D11DeviceContext* dc)
{
    this->shader->Draw(dc);
}

#ifdef _DEBUG

void ShaderComponent::DrawDebugGUI()
{
}

#endif _DEBUG