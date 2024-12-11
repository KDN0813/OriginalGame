#include "TextNumberComponent.h.h"

void TextNumber::Start()
{
    // フォント用スプライト読み込み
    this->sprite = std::make_unique<Sprite>(this->param.font_name);
}
