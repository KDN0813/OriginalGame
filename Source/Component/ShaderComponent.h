#pragma once
#include <D3D11.h>
#include "Component.h"
#include <memory>

class Shader;
class InstancingModelComponent;

class ShaderComponent : public Component
{
public:
    ShaderComponent(Shader* const shader);

    const char* GetName()const override { return "ShaderComponent"; }
    void SetWeakComponent()override;

    void Draw(ID3D11DeviceContext* dc);

    void SetInstancingResource();
    void InstancingAdd();
private:
    Shader* const shader;
    std::weak_ptr<InstancingModelComponent> instancing_model_Wptr;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
#endif _DEBUG
};

