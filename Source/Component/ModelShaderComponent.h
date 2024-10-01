#pragma once
#include <memory>
#include <d3d11.h>
#include "Component.h"

class ModelShader;

class ModelComponent;

class ModelShaderComponent : public Component
{
public:
    ModelShaderComponent(ModelShader* const shader);

    void Start() override;
    void End() override;
    const char* GetName()const override { return "ModelShaderComponent"; }

    void Draw(ID3D11DeviceContext* dc);

private:
    ModelShader* shader;

private:
    std::weak_ptr<ModelComponent> model_Wptr;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
#endif _DEBUG
};

