#pragma once
#include <d3d11.h>
#include "Component\Component.h"

// 2D�X�v���C�g�R���|�[�l���g�̊��N���X
class BaseSpriteComponent : public Component
{
public:
    // �`�揈��
    virtual void Render(ID3D11DeviceContext* dc) = 0;
};