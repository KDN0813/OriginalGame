#pragma once
#include <D3D11.h>
#include "Component.h"

class Shader;

class ShaderComponent : public Component
{
public:
    ShaderComponent(Shader* shader) :shader(shader) {};

    void Draw(ID3D11DeviceContext* dc);
private:
    Shader* shader;
};

