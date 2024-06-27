#pragma once

#include <DirectXMath.h>
#include "Model/Model.h"
#include "Graphics/Shader/Shader.h"

// デバッグ用オブジェクトクラス
class DebugObject
{
public:
    DebugObject(const char* filename, DirectX::XMFLOAT3 position = {});
    virtual ~DebugObject() {};


    virtual void Update(float elapsedTime);
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader);

    // 各種設定・取得関数
    const DirectX::XMFLOAT3& GetPosition() const { return this->position; }
    void SetPosition(DirectX::XMFLOAT3& pos) { this->position = pos; }

    virtual void DrawImGUi();
private:
    void UpdateTransform();

private:
    std::unique_ptr<Model> model;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 scale;
    DirectX::XMFLOAT4X4 transform;
};

// デバッグ用プレイヤー
class DebugPlayer : public DebugObject
{
public:
    DebugPlayer(const char* filename):DebugObject(filename){}

    void Update(float elapsedTime)override;
};

