#include "UIStateDerived.h"
#include "Object\Object.h"
#include "Object\Constant\UIConstant.h"
#include "Object\GameObject.h"
#include "System\GameData.h"

#include "Component\TextNumberComponent.h"
#include "Component\SpriteComponent.h"
#include "Component\CharacterComponent.h"
#include "Component\PlayerComponent.h"

const MyHash ScoreUIDefaultState::STATE_NAME = MyHash("ScoreUIDefaultState");
ScoreUIDefaultState::ScoreUIDefaultState()
    :State(ScoreUIDefaultState::STATE_NAME)
{
}

void ScoreUIDefaultState::Update(float elapsed_time)
{
    if (const auto onwer = GetOwner())
    {
        if (const auto& text_number = onwer->GetComponent(this->text_number_Wptr))
        {
            if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
            {
                // ゲームデータのスコアを設定する
                text_number->SetDrawValue(game_data->GetScore());
            }
        }
    }
}

const MyHash EndTimerUIDefaultState::STATE_NAME = MyHash("EndTimerUIDefaultState");
EndTimerUIDefaultState::EndTimerUIDefaultState()
    : State(EndTimerUIDefaultState::STATE_NAME)
{
}

void EndTimerUIDefaultState::Update(float elapsed_time)
{
    if (const auto onwer = GetOwner())
    {
        if (const auto& text_number = onwer->GetComponent(this->text_number_Wptr))
        {
            if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
            {
                // ゲームデータのゲーム終了時間を設定する
                text_number->SetDrawValue(static_cast<int>(game_data->GetGameEndTimer()));
            }
        }
    }
}

const MyHash PlayerHPBarUIState::STATE_NAME = MyHash("PlayerHPBarUIState");
PlayerHPBarUIState::PlayerHPBarUIState()
    : State(PlayerHPBarUIState::STATE_NAME)
{
}

void PlayerHPBarUIState::Update(float elapsed_time)
{
    if (const auto onwer = GetOwner())
    {
        if (const auto& sprite = onwer->GetComponent(this->sprite_Wptr))
        {
            sprite->SetDisplaySizeX(CalculateHealthBarWidth());
        }
    }
}

float PlayerHPBarUIState::CalculateHealthBarWidth()
{
    if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
    {
        if (const auto player = game_object->GetPlayer())
        {
            if (const auto& player_health = player->GetComponent(this->player_health_Wptr))
            {
                return player_health->GetHealthPercentage();
            }
        }
    }
    return 1.0f;
}

const MyHash PlayerSpecialPointUIState::STATE_NAME = MyHash("PlayerSpecialPointUIState");
PlayerSpecialPointUIState::PlayerSpecialPointUIState()
    :State(STATE_NAME)
{
}

void PlayerSpecialPointUIState::Update(float elapsed_time)
{
    if (const auto onwer = GetOwner())
    {
        if (const auto& sprite = onwer->GetComponent(this->sprite_Wptr))
        {
            sprite->SetDisplaySizeX(CalculateSpecialPointWidth());
        }
    }
}

float PlayerSpecialPointUIState::CalculateSpecialPointWidth()
{
    if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
    {
        if (const auto player = game_object->GetPlayer())
        {
            if (const auto& player_component = player->GetComponent(this->player_Wptr))
            {
                return player_component->GetSpecialPoint() / player_component->GetSpecialPointMax();
            }
        }
    }
    return 1.0f;
}

const MyHash SpinAttackDescriptionUIState::STATE_NAME = MyHash("SpinAttackDescriptionUIState");
SpinAttackDescriptionUIState::SpinAttackDescriptionUIState()
    :State(STATE_NAME)
{
}

void SpinAttackDescriptionUIState::Update(float elapsed_time)
{
    const auto onwer = GetOwner();
    if (!onwer)return;
    const auto& sprite = onwer->GetComponent(this->sprite_Wptr);
    if (!sprite)return;

    GameObject::Instance game_object = GameObject::GetInstance();
    const auto& player = game_object->GetPlayer();
    if (!player)return;
    const auto& player_component = player->GetComponent(this->player_Wptr);
    if (!player_component)return;

    if(player_component->IsUseSpecialPoint(player_component->GetSpinAttackUsePoint()))
    {
        // スキルが使用できる場合
        sprite->SetColor({ 1.0f,1.0f ,1.0f ,1.0f });
    }
    else
    {
        // スキルが使用できない場合
        sprite->SetColor(UIConstant::DEFAULT_SPIN_ATTACK_DESCRIPTION_UI_COLOR);
    }
}
