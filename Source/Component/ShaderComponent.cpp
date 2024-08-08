#include "ShaderComponent.h"
#include "Shader/Shader.h"

void ShaderComponent::Draw(ID3D11DeviceContext* dc)
{
    this->shader->Draw(dc);
}
