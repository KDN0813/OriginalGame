#pragma once
#include "Component.h"
#include <DirectXMath.h>

class Transform3DComponent : public Component
{
public:
    Transform3DComponent();
    ~Transform3DComponent() {};

    void Update(float elapsed_time)override;
    const char* GetName()const override { return "TransformComponent"; }
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::MEDIUM; };

    void UpdateTransform();
    const DirectX::XMFLOAT4X4& GetWolrdTransform();
    const DirectX::XMFLOAT4X4& GetLocalTransform() { return this->local_transform; }
    void SetTransform(DirectX::XMFLOAT4X4 transform) { this->local_transform = transform; }

#pragma region set・get parame
    DirectX::XMFLOAT3 GetLocalPosition()const noexcept { return this->local_position; }
    void SetLocalPosition(DirectX::XMFLOAT3 pos)noexcept { this->change_value = true; this->local_position = pos; }
    DirectX::XMFLOAT3 AddLocalPosition(DirectX::XMFLOAT3 vec);

    DirectX::XMFLOAT3 GetLocalAngle()const noexcept { return this->local_angle; }
    void SetLocalAngle(DirectX::XMFLOAT3 angle)noexcept { this->change_value = true; this->local_angle = angle; }
    
    DirectX::XMFLOAT3 GetLocalScale()const noexcept { return this->local_scale; }
    void SetLocalScale(DirectX::XMFLOAT3 scale)noexcept { this->change_value = true; this->local_scale = scale; }
    
    DirectX::XMFLOAT3 GetWorldPosition();
    void SetWorldPosition();
    void SetWorldPosition(std::shared_ptr<Transform3DComponent> parent_transform);

    bool GetChangeValue();
#pragma endregion set parame
private:
    DirectX::XMFLOAT4X4 local_transform{};
    DirectX::XMFLOAT4X4 world_transform{};
    DirectX::XMFLOAT3 local_position{};
    DirectX::XMFLOAT3 local_angle{};
    DirectX::XMFLOAT3 local_scale{};
    DirectX::XMFLOAT3 world_position{};
    bool change_value = false;
private:
    std::weak_ptr<Transform3DComponent> parent_ransform_Wptr;

#ifdef _DEBUG
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
    /**
    * デバックの情報を3D画面上に出力する関数
    */
    void DrawDebugPrimitive();
    // デバッグプリミティブを表示する
    bool IsDebugPrimitive()override { return true; }
#endif // _DEBUG
};