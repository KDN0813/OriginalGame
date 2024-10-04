#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Component.h"

class Transform3DComponent;

class MovementComponent : public Component
{
public:
    // 更新関数
    void Update(float elapsed_time)override;
    // 名前取得
    const char* GetName()const override { return "MovementComponent"; };
    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::VERY_HIGH; }

    // XZ軸方向に移動しているか
    bool IsMoveXZAxis();

    // 各種設定取得・関数
    void SetVelocity(DirectX::XMFLOAT3 move_vec) { this->move_vec = move_vec; }
    void SetVelocityX(float move_vecX) { this->move_vec.x = move_vecX; }
    void SetVelocityY(float move_vecY) { this->move_vec.y = move_vecY; }
    void SetVelocityZ(float move_vecZ) { this->move_vec.z = move_vecZ; }
    void SetMoveSpeed(float speed) { this->speed = speed; }
    const float& GetVelocityX() { return this->move_vec.x; }
    const float& GetVelocityY() { return this->move_vec.y; }
    const float& GetVelocityZ() { return this->move_vec.z; }

private:
    DirectX::XMFLOAT3 move_vec{};   // 移動方向のベクトル
    float speed = 1.0f;             // 移動速度

private:
    std::weak_ptr<Transform3DComponent> transform3D_Wptr;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI();

#endif _DEBUG
};

