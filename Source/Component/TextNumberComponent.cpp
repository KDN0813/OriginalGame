#include "Component\TextNumberComponent.h"

void TextNumberComponent::Start()
{
    // �t�H���g�p�X�v���C�g�ǂݍ���
    this->sprite = std::make_unique<Sprite>(this->param.font_name.c_str());

    // �t�H���g�T�C�Y�ݒ�
    {
        DirectX::XMFLOAT2 sprite_size{};    //�@�摜�T�C�Y
        const float number_count = 10;      // �����̐�(0�`9�܂ł̐���)
        this->font_size = { sprite_size.x,sprite_size.y / number_count };
    }
}

void TextNumberComponent::Render(ID3D11DeviceContext* dc)
{
    this->sprite->Render(dc,
        this->param.pos,
        this->display_size,
        this->clip_pos,
        this->clip_size,
        this->param.angle,
        this->param.color,
        this->param.center_type
    );
}
