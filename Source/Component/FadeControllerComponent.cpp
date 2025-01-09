#include "FadeControllerComponent.h"

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
        }
        break;
    default:break;
    }
}

void FadeControllerComponent::SetFead(FEAD_TYPE type, float fade_duration)
{
    this->param.fead_type = type;
    this->param.fade_duration = fade_duration;
}

void FadeControllerComponent::FeadStart()
{
    this->SetIsActive(true);
    this->param.fade_time = this->param.fade_duration;
    this->param.state = FADE_STATE::START;
}

void FadeControllerComponent::FeadIn()
{

}

void FadeControllerComponent::FeadOut()
{
}
