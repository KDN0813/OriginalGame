#pragma once
#include "Component.h"
#include <DirectXMath.h>

class TransformComponent : public Component
{
public:
    TransformComponent() :transform() {}
    ~TransformComponent() {};

    const DirectX::XMFLOAT4& GetTransform() { return this->transform; }
    void SetTransform(const DirectX::XMFLOAT4& transform) { this->transform = transform; }
private:
    DirectX::XMFLOAT4 transform;
};