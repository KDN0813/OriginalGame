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


    virtual void Update(float elapsed_time);
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader);

    // 各種設定・取得関数
    const DirectX::XMFLOAT3& GetPosition() const { return this->position; }
    void SetPosition(DirectX::XMFLOAT3& pos) { this->position = pos; }
    const DirectX::XMFLOAT4X4& GetTransform()const { return this->transform; }

    const Model* GetModel()const { return this->model.get(); }
    virtual void DrawImGUi();

    void UpdateTransform();

private:
    std::unique_ptr<Model> model;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 scale;
    DirectX::XMFLOAT4X4 transform;
};

class ObjectManager
{
public:
    ObjectManager(const char* filename);
    ~ObjectManager() {};

    void Update(float elapsedTime);

    void DrawImGUi();

    // 各種取得関数
    const size_t GetObjeCount()const { return objects.size(); }
    const DebugObject* GetDebugObject(size_t index)const { return this->objects[index].get(); }
    const Model* GetModel() { return this->objects[0]->GetModel(); }
    ID3D11Buffer* GetInputBuffer() const { return this->inputBuffer.Get(); }

public:
    static const size_t OBJECT_MAX = 200;
private:
    std::vector<std::unique_ptr<DebugObject>> objects;

    Microsoft::WRL::ComPtr<ID3D11Buffer> inputBuffer;  // インスタンスごとの行列を保持するバッファ 
    std::vector<DirectX::XMFLOAT4X4> inputData;
};

// デバッグ用プレイヤー
class DebugPlayer : public DebugObject
{
public:
    DebugPlayer(const char* filename):DebugObject(filename){}

    void Update(float elapsedTime)override;
};

