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
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
#endif _DEBUG
};

