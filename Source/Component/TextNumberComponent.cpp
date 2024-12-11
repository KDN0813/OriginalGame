#include "Component\TextNumberComponent.h"

void TextNumberComponent::Start()
{
    // フォント用スプライト読み込み
    this->sprite = std::make_unique<Sprite>(this->param.font_name.c_str());

    // フォントサイズ設定
    {
        DirectX::XMFLOAT2 sprite_size{};    //　画像サイズ
        const float number_count = 10;      // 数字の数(0～9までの整数)
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
