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
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()override;
#endif // _DEBUG
};