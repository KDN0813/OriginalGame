#pragma once
#include <d3d11.h>
#include "Component\Component.h"

// 2D�X�v���C�g�R���|�[�l���g�̊��N���X
class BaseSpriteComponent : public Component , public std::enable_shared_from_this<BaseSpriteComponent>
{
public:
    BaseSpriteComponent() :draw_priority(PRIORITY::DEFAULT) {};
    BaseSpriteComponent(PRIORITY priority) :draw_priority(priority) {};
public:
    // �J�n�֐�
    virtual void Start() override;

    // �`�揈��
    virtual void Render(ID3D11DeviceContext* dc) = 0;

protected:
    PRIORITY draw_priority;
};