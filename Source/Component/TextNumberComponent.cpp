#include "TextNumberComponent.h.h"

void TextNumber::Start()
{
    // �t�H���g�p�X�v���C�g�ǂݍ���
    this->sprite = std::make_unique<Sprite>(this->param.font_name);
}
