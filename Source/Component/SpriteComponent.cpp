#include "SpriteComponent.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


SpriteComponent::SpriteComponent(const SpriteParam&)
    :param(param),default_param(param)
{
    this->sprite = std::make_unique<Sprite>(this->param.filename);
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
    ImGui::SliderFloat2("Display Pos", &this->param.display_pos.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("Display Size", &this->param.display_size.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("Clip Pos", &this->param.clip_pos.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("Clip Size", &this->param.clip_size.x, 0.0f, 1.0f);
}

#endif // DEBUG