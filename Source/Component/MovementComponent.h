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

private:
    std::weak_ptr<Transform3DComponent> transform3D_Wptr;
    DirectX::XMFLOAT3 velocity{};

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI();

#endif _DEBUG
};

