#include "SpriteComponent.h"
#include "System/MyMath/MYVECTOR2.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // _DEBUG


SpriteComponent::SpriteComponent(const SpriteParam& param)
    :param(param),default_param(param)
{
    this->sprite = std::make_unique<Sprite>(this->param.filename.c_str());
}

#ifdef _DEBUG

void SpriteComponent::Render(ID3D11DeviceContext* dc)
{
    this->sprite->Render(dc,
        this->param.display_pos,
        this->param.display_size,
        this->param.clip_pos,
        this->param.clip_size,
        this->param.angle,
        this->param.color
    );
}

void SpriteComponent::DrawDebugGUI()
{
    ImGui::InputTextString("Sprite Name", this->param.filename);
    ImGui::SliderFloat2("Display Pos", &this->param.display_pos.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("Display Size", &this->param.display_size.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("Clip Pos", &this->param.clip_pos.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("Clip Size", &this->param.clip_size.x, 0.0f, 1.0f);
    ImGui::SliderFloat("Angle", &this->param.angle, 0.0f, 360.0f);
    ImGui::ColorEdit4("Sprite Color", &this->param.color.x);
}

#endif // DEBUG