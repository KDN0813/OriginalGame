#include "SpriteComponent.h"
#include "System/MyMath/MYVECTOR2.h"
#include "Object\Object.h"
#include "Graphics\Graphics.h"

#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#include <magic_enum.hpp>
#include "Shader\SpriteShader.h"
#endif // _DEBUG

#include "Component\Transform2DComponent.h"

SpriteComponent::SpriteComponent(const BaseSpriteComponent::SpriteParam& sprite_param)
    :BaseSpriteComponent(sprite_param)
{
    this->sprite = std::make_unique<Sprite>(sprite_param.filename.size() ? this->sprite_param.filename.c_str() : nullptr);

#ifdef _DEBUG
    // CENTER_TYPE 列挙型の各値に対応する名前を取得して格納する
    for (size_t i = 0; i < static_cast<size_t>(Sprite::CENTER_TYPE::MAX); ++i)
    {
        this->center_type_name_pool.emplace_back(magic_enum::enum_name(static_cast<Sprite::CENTER_TYPE>(i)));
    }
#endif // _DEBUG
}

void SpriteComponent::ReStart()
{
    this->sprite_param = this->default_sprite_param;
}

void SpriteComponent::Render(ID3D11DeviceContext* dc)
{
    DirectX::XMFLOAT2 display_pos{};
    DirectX::XMFLOAT2 display_size{};
    float angle{};
    DirectX::XMFLOAT2 scale{};
    if (const auto& owner = GetOwner())
    {
        if (const auto& transform = owner->GetComponent(this->transform_Wptr))
        {
            display_pos = transform->GetWorldPosition();
            angle = transform->GetWorldAngle();
            scale = transform->GetWorldScale();
        }
    }

    display_size = { this->sprite_param.display_size.x * scale.x ,this->sprite_param.display_size.y * scale.y };

    this->sprite->Render(dc,
        display_pos,
        display_size,
        this->sprite_param.clip_pos,
        this->sprite_param.clip_size,
        angle,
        this->sprite_param.color,
        this->sprite_param.center_type
    );
}

void SpriteComponent::AdjustDisplaySizeToSprite()
{
    // スプライトサイズ取得
    const float SPRITE_WIDTH = this->sprite->GetTextureWidth();
    const float SPRITE_HEIGHT = this->sprite->GetTextureHeight();

    // スクリーンサイズ取得
    Graphics::Instance graphics = Graphics::GetInstance();
    const float SCREEN_WIDTH = graphics->GetScreenWidth();
    const float SCREEN_HEIGHT = graphics->GetScreenHeight();

    // 表示サイズ設定
    this->sprite_param.display_size.x = SPRITE_WIDTH / SCREEN_WIDTH;
    this->sprite_param.display_size.y = SPRITE_HEIGHT / SCREEN_HEIGHT;

    this->sprite_param.display_size = this->sprite_param.display_size;
}

#ifdef _DEBUG

void SpriteComponent::DrawDebugGUI()
{
    ImGui::InputTextString("Sprite Name", this->sprite_param.filename);

    int priority = static_cast<int>(this->sprite_param.draw_priority);
    if (ImGui::InputInt("Draw Priority", &priority))
    {
        this->sprite_param.draw_priority = static_cast<PRIORITY>((priority < 0) ? 0 : priority);
        {
            SpriteShader::Instance manager = SpriteShader::GetInstance();
            manager->SetShouldSort(true);
        }
    }

    ImGui::SliderFloat2("Display Size", &this->sprite_param.display_size.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("Clip Pos", &this->sprite_param.clip_pos.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("Clip Size", &this->sprite_param.clip_size.x, 0.0f, 1.0f);
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
    if (ImGui::Button("AdjustDisplaySizeToSprite"))
    {
        AdjustDisplaySizeToSprite();
    }
}

#endif // DEBUG