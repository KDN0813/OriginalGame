#pragma once
#include "Component.h"
#include <DirectXMath.h>

class TransformComponent : public Component
{
public:
    TransformComponent() :transform() {}
    ~TransformComponent() {};

    const char* GetName()const override { return "TransformComponent"; }

    const DirectX::XMFLOAT4& GetTransform() { return this->transform; }
    void SetTransform(const DirectX::XMFLOAT4& transform) { this->transform = transform; }
private:
    DirectX::XMFLOAT4 transform;

#ifdef _DEBUG
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
#endif // _DEBUG
};