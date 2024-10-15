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
    const DirectX::XMFLOAT4X4& GetWolrdTransform() { return this->world_transform; }
    const DirectX::XMFLOAT4X4& GetLocalTransform() { return this->local_transform; }
    void SetTransform(DirectX::XMFLOAT4X4 transform) { this->local_transform = transform; }

#pragma region set・get parame
    DirectX::XMFLOAT3 GetPosition()const noexcept { return this->position; }
    void SetPosition(DirectX::XMFLOAT3 pos)noexcept { this->change_value = true; this->position = pos; }
    DirectX::XMFLOAT3 AddPosition(DirectX::XMFLOAT3 vec);

    DirectX::XMFLOAT3 GetAngle()const noexcept { return this->angle; }
    void SetAngle(DirectX::XMFLOAT3 angle)noexcept { this->change_value = true; this->angle = angle; }
    
    DirectX::XMFLOAT3 GetScale()const noexcept { return this->scale; }
    void SetScale(DirectX::XMFLOAT3 scale)noexcept { this->change_value = true; this->scale = scale; }
    
    bool GetChangeValue();
#pragma endregion set parame
private:
    DirectX::XMFLOAT4X4 local_transform{};
    DirectX::XMFLOAT4X4 world_transform{};
    DirectX::XMFLOAT3 position{};
    DirectX::XMFLOAT3 angle{};
    DirectX::XMFLOAT3 scale{};
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