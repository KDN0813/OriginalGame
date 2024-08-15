#include "Shader.h"
#include "Component/ShaderComponent.h"

void Shader::AddShaderComponent(const std::shared_ptr<ShaderComponent>& shader_component)
{
    shader_component_Wptr_vector.emplace_back(std::weak_ptr<ShaderComponent>(shader_component));
}
