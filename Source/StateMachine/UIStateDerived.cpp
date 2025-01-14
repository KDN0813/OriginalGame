#include "UIStateDerived.h"
#include "Object\Object.h"
#include "Object\GameObject.h"
#include "System\GameData.h"

#include "Component\TextNumberComponent.h"
#include "Component\SpriteComponent.h"
#include "Component\CharacterComponent.h"

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
