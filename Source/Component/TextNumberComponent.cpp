#include "Component\TextNumberComponent.h"
#include "Graphics\Graphics.h"
#include "Object\Object.h"

#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#include <magic_enum.hpp>
#include "Shader\SpriteShader.h"
#endif // DEBUG

#include "Component\Transform2DComponent.h"

void TextNumberComponent::Start()
{
    BaseSpriteComponent::Start();

    // フォント用スプライト読み込み
    this->sprite = std::make_unique<Sprite>(this->sprite_param.filename.c_str());

    // フォントサイズ設定
    {
        const float texture_width = this->sprite->GetTextureWidth();
        const float texture_height = this->sprite->GetTextureHeight();
        
        float screen_width = {};
        float screen_height = {};
        {
            Graphics::Instance graphics = Graphics::GetInstance();
            screen_width = graphics->GetScreenWidth();
            screen_height = graphics->GetScreenHeight();
        }

        // 描画用サイズ設定
        this->font_draw_size = { (texture_width / screen_width) / NUMBER_COUNT ,(texture_height / screen_height) };
    }

    // 切り抜きサイズ設定
    this->sprite_param.clip_size = { FONT_WIGTH,1.0f };

#ifdef _DEBUG
    // CENTER_TYPE 列挙型の各値に対応する名前を取得して格納する
    for (size_t i = 0; i < static_cast<size_t>(Sprite::CENTER_TYPE::MAX); ++i)
    {
        this->center_type_name_pool.emplace_back(magic_enum::enum_name(static_cast<Sprite::CENTER_TYPE>(i)));
    }
#endif // _DEBUG
}

void TextNumberComponent::ReStart()
{
    this->sprite_param = this->default_sprite_param;
}

void TextNumberComponent::Render(ID3D11DeviceContext* dc)
{
    DirectX::XMFLOAT2 pos{};
    DirectX::XMFLOAT2 scale{};
    float angle{};
    if (const auto& owner = GetOwner())
    {
        if (const auto& transform = owner->GetComponent(this->transform_Wptr))
        {
            pos = transform->GetWorldPosition();
            scale = transform->GetWorldScale();
            angle = transform->GetWorldAngle();
        }
    }

    // 桁数取得
    std::string numeral_str = std::to_string(this->value);
    int Digits; // 桁数
    Digits = static_cast<int>(numeral_str.size());

    // 描画サイズ更新
    this->sprite_param.display_size = { this->font_draw_size.x * scale.x,this->font_draw_size.y * scale.y };

    // 中心位置へのオフセット値取得
    float rateX, rateY;
    Sprite::GetCenterTypeRate(rateX, rateY, this->sprite_param.center_type);

    // 位置設定
    pos.x -= this->sprite_param.display_size.x * static_cast<float>(Digits - 1) * rateX;

    // 桁数分描画を行う
    for (int i = 0; i < Digits; ++i)
    {        

        int n = std::stoi(numeral_str.substr(i, 1));
        this->sprite_param.clip_pos = { FONT_WIGTH * static_cast<float>(n),0.0f};

        this->sprite->Render(dc,
            pos,
            this->sprite_param.display_size,
            this->sprite_param.clip_pos,
            this->sprite_param.clip_size,
            angle,
            this->sprite_param.color,
            this->sprite_param.center_type
        );

        // 描画位置更新
        pos.x += this->sprite_param.display_size.x;
    }
}

#ifdef _DEBUG

void TextNumberComponent::DrawDebugGUI()
{
    ImGui::InputTextString("Font Name", this->sprite_param.filename);

    int priority = static_cast<int>(this->sprite_param.draw_priority);
    if (ImGui::InputInt("Draw Priority", &priority))
    {
        this->sprite_param.draw_priority = static_cast<PRIORITY>((priority < 0) ? 0 : priority);
        {
            SpriteShader::Instance manager = SpriteShader::GetInstance();
            manager->SetShouldSort(true);
        }
    }

    int value = this->value;
    if (ImGui::InputInt("Value", &value))
    {
        SetDrawValue(value);
    }
    ImGui::ColorEdit4("Sprite Color", &this->sprite_param.color.x);

    // 中心位置の設定
    int center_type_index = static_cast<int>(this->sprite_param.center_type);
    std::string now_name{};
    now_name = magic_enum::enum_name(this->sprite_param.center_type);
    this->sprite_param.center_type;
    if (ImGui::ComboUI("CenterType", now_name, this->center_type_name_pool, center_type_index))
    {
        this->sprite_param.center_type = static_cast<Sprite::CENTER_TYPE>(center_type_index);
    }
}

#endif // DEBUG