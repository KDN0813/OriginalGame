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
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::MEDIUM; };

    DirectX::XMFLOAT4X4 GetTransform() { return this->transform; }
    void SetTransform(DirectX::XMFLOAT4X4 transform) { this->transform = transform; }

#pragma region set・get parame
    DirectX::XMFLOAT3 GetPosition()const noexcept { return this->position; }
    void SetPosition(DirectX::XMFLOAT3 pos)noexcept { this->change_value = true; this->position = pos; }
    DirectX::XMFLOAT3 AddPosition(DirectX::XMFLOAT3 vec);

    DirectX::XMFLOAT3 GetAngle()const noexcept { return this->angle; }
    void SetAngle(DirectX::XMFLOAT3 angle)noexcept { this->change_value = true; this->angle = angle; }
    
    DirectX::XMFLOAT3 GetScale()const noexcept { return this->scale; }
    void SetScale(DirectX::XMFLOAT3 scale)noexcept { this->change_value = true; this->scale = scale; }
    
#pragma endregion set parame
private:
    DirectX::XMFLOAT4X4 transform{};
    DirectX::XMFLOAT3 position{};
    DirectX::XMFLOAT3 angle{};
    DirectX::XMFLOAT3 scale{};
    bool change_value = false;
#ifdef _DEBUG
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
#endif // _DEBUG
};