#include "FadeControllerComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <cmath>
#include "System\MyMath\MyMathf.h"

#include "Object\Object.h"
#include "Component\BaseSpriteComponent.h"

void FadeControllerComponent::Start()
{
    SetIsActive(false);
}

void FadeControllerComponent::Update(float elapsed_time)
{
    switch (this->param.state)
    {
    case FADE_STATE::START:
        this->param.fade_time = 0.0f;
        this->param.state = FADE_STATE::RUN;
        break;
    case FADE_STATE::RUN:
        this->param.fade_time += elapsed_time;

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

        if (this->param.fade_duration <= this->param.fade_time)
        {
            this->param.state = FADE_STATE::END;
            this->SetIsActive(false);
        }
        break;
    default:break;
    }
}

void FadeControllerComponent::FeadStart(FEAD_TYPE type, float fade_duration)
{
    this->param.fead_type = type;
    this->param.fade_duration = fade_duration;
    this->SetIsActive(true);
    this->param.fade_time = this->param.fade_duration;
    this->param.state = FADE_STATE::START;
}

void FadeControllerComponent::ClearFade()
{
    this->SetIsActive(false);
    if (const auto& owner = GetOwner())
    {
        if (const auto& sprite = owner->GetComponent(this->sprite_Wptr))
        {
            sprite->SetAlpha(0.0f);
        }
    }
}

void FadeControllerComponent::FeadIn()
{
    if (const auto& owner = GetOwner())
    {
        if (const auto& sprite = owner->GetComponent(this->sprite_Wptr))
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
        if (const auto& sprite = owner->GetComponent(this->sprite_Wptr))
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
        FeadStart(FEAD_TYPE::FEAD_IN, 0.2f);
    }
    if (ImGui::Button("FeadOut"))
    {
        FeadStart(FEAD_TYPE::FEAD_OUT, 0.2f);
    }

    ImGui::InputFloat("Fade Duration", &this->param.fade_duration);
    ImGui::Text(("Fead Time:" + std::to_string(this->param.fade_time)).c_str());
}

#endif // _DEBUG