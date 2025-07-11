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

    // �t�H���g�p�X�v���C�g�ǂݍ���
    this->sprite = std::make_unique<Sprite>(this->sprite_param.filename.c_str());

    // �t�H���g�T�C�Y�ݒ�
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

        // �`��p�T�C�Y�ݒ�
        this->font_draw_size = { (texture_width / screen_width) / NUMBER_COUNT ,(texture_height / screen_height) };
    }

    // �؂蔲���T�C�Y�ݒ�
    this->sprite_param.clip_size = { FONT_WIGTH,1.0f };

#ifdef _DEBUG
    // CENTER_TYPE �񋓌^�̊e�l�ɑΉ����閼�O���擾���Ċi�[����
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

    // �����擾
    std::string numeral_str = std::to_string(this->value);
    int Digits; // ����
    Digits = static_cast<int>(numeral_str.size());

    // �`��T�C�Y�X�V
    this->sprite_param.display_size = { this->font_draw_size.x * scale.x,this->font_draw_size.y * scale.y };

    // ���S�ʒu�ւ̃I�t�Z�b�g�l�擾
    float rateX, rateY;
    Sprite::GetCenterTypeRate(rateX, rateY, this->sprite_param.center_type);

    // �ʒu�ݒ�
    pos.x -= this->sprite_param.display_size.x * static_cast<float>(Digits - 1) * rateX;

    // �������`����s��
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

        // �`��ʒu�X�V
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

    // ���S�ʒu�̐ݒ�
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