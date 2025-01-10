#include "FadeControllerComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <cmath>
#include "System\MyMath\MyMathf.h"

#include "Object\Object.h"
#include "Component\SpriteComponent.h"

void FadeControllerComponent::Start()
{
}

void FadeControllerComponent::Update(float elapsed_time)
{
    switch (this->param.state)
    {
    case FADE_STATE::START:
        this->param.fade_time = this->param.fade_duration;
        this->param.state = FADE_STATE::RUN;
        break;
    case FADE_STATE::RUN:
        this->param.fade_time -= elapsed_time;

        switch (this->param.fead_type)
        {
        case FEAD_TYPE::FEAD_IN:
            FeadIn();
            break;
        case FEAD_TYPE::FEAD_OUT:
            FeadOut();
            break;
        default:
            break;
        }

        if (this->param.fade_time <= 0)
        {
            this->param.state = FADE_STATE::END;
            this->SetIsActive(false);
        }
        break;
    default:break;
    }
}

void FadeControllerComponent::SetFead(FEAD_TYPE type, float fade_duration)
{
    this->param.fead_type = type;
    this->param.fade_duration = (std::max)(fade_duration, 0.2f);
}

void FadeControllerComponent::FeadStart()
{
    this->SetIsActive(true);
    this->param.fade_time = this->param.fade_duration;
    this->param.state = FADE_STATE::START;
}

void FadeControllerComponent::ClearFade()
{
    this->SetIsActive(false);
    if (const auto& owner = GetOwner())
    {
        if (const auto& sprite = owner->EnsureComponentValid(this->sprite_Wptr))
        {
            const float t = this->param.fade_time / this->param.fade_duration;
            sprite->SetAlpha(0.0f);
        }
    }
}

void FadeControllerComponent::FeadIn()
{
    if (const auto& owner = GetOwner())
    {
        if (const auto& sprite = owner->EnsureComponentValid(this->sprite_Wptr))
        {
            const float t = this->param.fade_time / this->param.fade_duration;
            sprite->SetAlpha(std::lerp(0.0f, 1.0f, t));
        }
    }
}

void FadeControllerComponent::FeadOut()
{
    if (const auto& owner = GetOwner())
    {
        if (const auto& sprite = owner->EnsureComponentValid(this->sprite_Wptr))
        {
            const float t = this->param.fade_time / this->param.fade_duration;
            sprite->SetAlpha(std::lerp(1.0f, 0.0f, t));
        }
    }
}

#ifdef _DEBUG

void FadeControllerComponent::DrawDebugGUI() 
{
    if (ImGui::Button("FeadIn"))
    {
        this->param.fead_type = FEAD_TYPE::FEAD_IN;
        FeadStart();
    }
    if (ImGui::Button("FeadOut"))
    {
        this->param.fead_type = FEAD_TYPE::FEAD_OUT;
        FeadStart();
    }

    ImGui::InputFloat("Fade Duration", &this->param.fade_duration);
    ImGui::Text(("Fead Time:" + std::to_string(this->param.fade_time)).c_str());
}

#endif // _DEBUG