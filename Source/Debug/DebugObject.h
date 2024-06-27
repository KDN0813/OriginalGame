#pragma once

#include <DirectXMath.h>
#include "Model/Model.h"
#include "Graphics/Shader/Shader.h"

// �f�o�b�O�p�I�u�W�F�N�g�N���X
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

