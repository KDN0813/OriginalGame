#include "Component\TextNumberComponent.h"

void TextNumberComponent::Start()
{
    // �t�H���g�p�X�v���C�g�ǂݍ���
    this->sprite = std::make_unique<Sprite>(this->param.font_name.c_str());
}

void TextNumberComponent::Render(ID3D11DeviceContext* dc)
{
}
