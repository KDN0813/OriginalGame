#include "Component\TextNumberComponent.h"

#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#include <magic_enum.hpp>
#endif // DEBUG


void TextNumberComponent::Start()
{
    // フォント用スプライト読み込み
    this->sprite = std::make_unique<Sprite>(this->param.font_name.c_str());

    // フォントサイズ設定
    {
        const float texture_width = this->sprite->GetTextureWidth();
        const float texture_height = this->sprite->GetTextureHeight();
        const float number_count = 10;// 数字の数(0～9までの整数)
        this->font_size = { texture_width / number_count,texture_height };
    }

#ifdef _DEBUG
    // CENTER_TYPE 列挙型の各値に対応する名前を取得して格納する
    for (size_t i = 0; i < static_cast<size_t>(Sprite::CENTER_TYPE::MAX); ++i)
    {
        this->center_type_name_pool.emplace_back(magic_enum::enum_name(static_cast<Sprite::CENTER_TYPE>(i)));
    }
#endif // _DEBUG
}

void TextNumberComponent::Render(ID3D11DeviceContext* dc)
{
    this->sprite->Render(dc,
        this->param.pos,
        this->display_size,
        this->clip_pos,
        this->font_size,
        this->param.angle,
        this->param.color,
        this->param.center_type
    );
}

void TextNumberComponent::DrawDebugGUI()
{
    ImGui::InputTextString("Font Name", this->param.font_name);
    ImGui::SliderFloat2("Pos", &this->param.pos.x, -1.0f, 2.0f);
    ImGui::InputInt("Value", &this->param.value);
    ImGui::SliderFloat("Angle", &this->param.angle, 0.0f, 360.0f);
    ImGui::ColorEdit4("Sprite Color", &this->param.color.x);

    // 中心位置の設定
    int center_type_index = static_cast<int>(this->param.center_type);
    std::string now_name{};
    now_name = magic_enum::enum_name(this->param.center_type);
    this->param.center_type;
    if (ImGui::ComboUI("CenterType", now_name, this->center_type_name_pool, center_type_index))
    {
        this->param.center_type = static_cast<Sprite::CENTER_TYPE>(center_type_index);
    }
}
