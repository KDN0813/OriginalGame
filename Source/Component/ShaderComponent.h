#pragma once
#include <D3D11.h>
#include "Component.h"

class Shader;

class ShaderComponent : public Component
{
public:
    ShaderComponent(Shader* const shader);

    const char* GetName()const override { return "ShaderComponent"; }

    void Draw(ID3D11DeviceContext* dc);
private:
    Shader* const shader;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()override;
#endif _DEBUG
};

