#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Component.h"

class Transform3DComponent;

class MovementComponent : public Component
{
public:
    // �X�V�֐�
    void Update(float elapsed_time)override;
    // ���O�擾
    const char* GetName()const override { return "MovementComponent"; };
    // �D��x
    const unsigned int GetPriority()const noexcept override { return 1; }

private:
    DirectX::XMFLOAT3 velocity{};

private:
    std::weak_ptr<Transform3DComponent> transform3D_Wptr;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI();

#endif _DEBUG
};
