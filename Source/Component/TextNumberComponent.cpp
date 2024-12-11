#include "Component\TextNumberComponent.h"
#include "Graphics\Graphics.h"

#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#include <magic_enum.hpp>
#endif // DEBUG

void TextNumberComponent::Start()
{
    // �t�H���g�p�X�v���C�g�ǂݍ���
    this->sprite = std::make_unique<Sprite>(this->param.font_name.c_str());

    // �t�H���g�T�C�Y�ݒ�
    {
        const float texture_width = this->sprite->GetTextureWidth();
        const float texture_height = this->sprite->GetTextureHeight();
        
        float screen_width = {};
        float screen_height = {};
        if (Graphics::Instance graphics = Graphics::GetInstance(); graphics.Get())
        {
            screen_width = graphics->GetScreenWidth();
            screen_height = graphics->GetScreenHeight();
        }

        // �`��p�T�C�Y�ݒ�
        this->font_draw_size = { (texture_width / screen_width) / NUMBER_COUNT ,(texture_height / screen_height) };
    }

    // �؂蔲���T�C�Y�ݒ�
    this->clip_size = { FONT_WIGTH,1.0f };

#ifdef _DEBUG
    // CENTER_TYPE �񋓌^�̊e�l�ɑΉ����閼�O���擾���Ċi�[����
    for (size_t i = 0; i < static_cast<size_t>(Sprite::CENTER_TYPE::MAX); ++i)
    {
        this->center_type_name_pool.emplace_back(magic_enum::enum_name(static_cast<Sprite::CENTER_TYPE>(i)));
    }
#endif // _DEBUG
}

void TextNumberComponent::Render(ID3D11DeviceContext* dc)
{
    // �����擾
    std::string numeral_str = std::to_string(this->param.value);
    int Digits; // ����
    Digits = static_cast<int>(numeral_str.size());

    DirectX::XMFLOAT2 pos = this->param.pos;

    // �������`����s��
    for (int i = 0; i < Digits; ++i)
    {        
        // �`��T�C�Y�X�V
        this->display_size = { this->font_draw_size.x * this->param.scale,this->font_draw_size.y * this->param.scale };

        int n = std::stoi(numeral_str.substr(i, 1));
        this->clip_pos = { FONT_WIGTH * static_cast<float>(n),0.0f};

        this->sprite->Render(dc,
            pos,
            this->display_size,
            this->clip_pos,
            this->clip_size,
            this->param.angle,
            this->param.color,
            this->param.center_type
        );

        // �`��ʒu�X�V
        pos.x += this->display_size.x;
    }
}

void TextNumberComponent::DrawDebugGUI()
{
    ImGui::InputTextString("Font Name", this->param.font_name);
    ImGui::SliderFloat2("Pos", &this->param.pos.x, -1.0f, 2.0f);
    ImGui::DragFloat("Scale", &this->param.scale, 0.05f, 0.0f, 100.0f);
    ImGui::InputInt("Value", &this->param.value);
    ImGui::SliderFloat("Angle", &this->param.angle, 0.0f, 360.0f);
    ImGui::ColorEdit4("Sprite Color", &this->param.color.x);

    // ���S�ʒu�̐ݒ�
    int center_type_index = static_cast<int>(this->param.center_type);
    std::string now_name{};
    now_name = magic_enum::enum_name(this->param.center_type);
    this->param.center_type;
    if (ImGui::ComboUI("CenterType", now_name, this->center_type_name_pool, center_type_index))
    {
        this->param.center_type = static_cast<Sprite::CENTER_TYPE>(center_type_index);
    }
}
