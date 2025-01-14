#pragma once

#include <memory>

class Object;

class UIConstant
{
public:
    // FadeObject用
    static constexpr float DEFAULT_FEAD_DURATION = 2.0f;            // 通常のフェードにかかる時間
    static constexpr float PLAYER_DEFEAT_FEAD_DURATION = 3.0f;      // 敗北時のフェードにかかる時間
public:
    static const std::shared_ptr<Object>& CreateScoreUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateGameTimerUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePause(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateFadeObject(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePlayerHpBarUI(const std::shared_ptr<Object>&);
};