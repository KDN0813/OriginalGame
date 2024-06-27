#pragma once

#include <DirectXMath.h>
#include "Model/Model.h"
#include "Graphics/Shader/Shader.h"

// デバッグ用オブジェクトクラス
class DebugObject
{
public:
    DebugObject(const char* filename);
    ~DebugObject() {};


    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void DrawImGUi();
private:
    void UpdateTransform();

private:
    std::unique_ptr<Model> model;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 scale;
    DirectX::XMFLOAT4X4 transform;
};

