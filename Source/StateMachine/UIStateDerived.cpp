#include "UIStateDerived.h"
#include "Object\Object.h"
#include "System\GameData.h"

#include "Component\TextNumberComponent.h"

const MyHash ScoreUIDefaultState::STATE_NAME = MyHash("ScoreUIDefaultState");
ScoreUIDefaultState::ScoreUIDefaultState()
    :State(ScoreUIDefaultState::STATE_NAME)
{
}

void ScoreUIDefaultState::Update(float elapsed_time)
{
    if (const auto onwer = GetOwner())
    {
        if (const auto& text_number = onwer->EnsureComponentValid(this->text_number_Wptr))
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
    :State(EndTimerUIDefaultState::STATE_NAME)
{
}

void EndTimerUIDefaultState::Update(float elapsed_time)
{
    if (const auto onwer = GetOwner())
    {
        if (const auto& text_number = onwer->EnsureComponentValid(this->text_number_Wptr))
        {
            if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
            {
                // ゲームデータのゲーム終了時間を設定する
                text_number->SetDrawValue(static_cast<int>(game_data->GetGameEndTimer()));
            }
        }
    }
}
