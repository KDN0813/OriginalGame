#pragma once
#include "Component.h"
#include <DirectXMath.h>

class Transform3DComponent : public Component
{
public:
    Transform3DComponent() {};
    ~Transform3DComponent() {};

    void Update(float elapsed_time)override;
    const char* GetName()const override { return "TransformComponent"; }

    const DirectX::XMFLOAT4X4& GetTransform() { return this->transform; }
    const DirectX::XMMATRIX& GetTransformVec() { return DirectX::XMLoadFloat4x4(&this->transform); }
    void SetTransform(const DirectX::XMFLOAT4X4& transform) { this->transform = transform; }

#pragma region set・get parame
    const DirectX::XMVECTOR GetPositionVec()const noexcept { return DirectX::XMLoadFloat3(&position); }
    const DirectX::XMFLOAT3& GetPosition()const noexcept { return position; }
    void SetPosition(const DirectX::XMFLOAT3& pos)noexcept { this->change_value = true; this->position = pos; }
    void SetPosition(const DirectX::XMVECTOR& pos)noexcept { this->change_value = true; DirectX::XMStoreFloat3(&this->position, pos); }
    const DirectX::XMVECTOR AddPositionVec(const DirectX::XMVECTOR& vec);
    const DirectX::XMFLOAT3& AddPosition(const DirectX::XMFLOAT3 vec);

    const DirectX::XMVECTOR GetQuaternionVec()const noexcept { return DirectX::XMLoadFloat4(&rotation); }
    const DirectX::XMFLOAT4& GetQuaternion()const noexcept { return rotation; }
    void SetQuaternion(const DirectX::XMVECTOR& quaternion)noexcept { this->change_value = true; DirectX::XMStoreFloat4(&this->rotation, quaternion); }
    void SetQuaternion(const DirectX::XMFLOAT4& quaternion)noexcept { this->change_value = true; this->rotation = quaternion; }

    const DirectX::XMVECTOR GetScaleVec()const noexcept { return DirectX::XMLoadFloat3(&this->scale); }
    const DirectX::XMFLOAT3& GetScale()const noexcept { return scale; }
    void SetScale(const DirectX::XMVECTOR scale)noexcept { this->change_value = true; DirectX::XMStoreFloat3(&this->scale, scale); }
    void SetScale(const DirectX::XMFLOAT3 scale)noexcept { this->change_value = true; this->scale = scale; }

#pragma endregion set parame
private:
    DirectX::XMFLOAT4X4 transform{};
    DirectX::XMFLOAT3 position{};
    DirectX::XMFLOAT4 rotation{};
    DirectX::XMFLOAT3 scale{};
    bool change_value = false;
#ifdef _DEBUG
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
#endif // _DEBUG
};