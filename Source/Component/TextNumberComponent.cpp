#include "Component\TextNumberComponent.h"

void TextNumberComponent::Start()
{
    // フォント用スプライト読み込み
    this->sprite = std::make_unique<Sprite>(this->param.font_name.c_str());
}

void TextNumberComponent::Render(ID3D11DeviceContext* dc)
{
}
