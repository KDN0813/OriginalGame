#pragma once
#include <d3d11.h>
#include "Component\Component.h"
#include <String>
#include "Sprite/Sprite.h"

// 2D�X�v���C�g�R���|�[�l���g�̊��N���X
class BaseSpriteComponent : public Component , public std::enable_shared_from_this<BaseSpriteComponent>
{
public:
    struct SpriteParam
    {
        std::string filename = {};
        DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
        DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
        DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
        DirectX::XMFLOAT2 offset = { 0.0f,0.0f };
        PRIORITY draw_priority = PRIORITY::DEFAULT;
        Sprite::CENTER_TYPE center_type = Sprite::CENTER_TYPE::TOP_LEFT;
    };
public:
    BaseSpriteComponent(const SpriteParam& param) :sprite_param(param), default_sprite_param(param){};
public:
    // �J�n�֐�
    virtual void Start() override;

    // �`�揈��
    virtual void Render(ID3D11DeviceContext* dc) = 0;

    PRIORITY GetDrawPriority() { return this->sprite_param.draw_priority; }

    void SetColor(const DirectX::XMFLOAT4 color) { this->sprite_param.color = color; this->default_sprite_param.color = color;};
    void SetAlpha(const float a) { this->sprite_param.color.w = a; this->default_sprite_param.color.w = a; };
    void SetOffset(const DirectX::XMFLOAT2 offset) { this->sprite_param.offset = offset; this->default_sprite_param.offset = offset; }

    // �������ꂽ�ʒu�̌v�Z
    DirectX::XMFLOAT2 CalcModifiedPos(const DirectX::XMFLOAT2 display_pos);

protected:
    SpriteParam sprite_param;
    SpriteParam default_sprite_param;
};