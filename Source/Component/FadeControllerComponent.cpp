#include "FadeControllerComponent.h"

void FadeControllerComponent::Start()
{
}

void FadeControllerComponent::Update(float elapsed_time)
{
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
