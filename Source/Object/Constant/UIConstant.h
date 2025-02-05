#pragma once

#include <memory>
#include <DirectXMath.h>

class Object;

class UIConstant
{
public:
    // FadeObject用
    static constexpr float DEFAULT_FEAD_DURATION = 2.0f;            // 通常のフェードにかかる時間
    static constexpr float PLAYER_DEFEAT_FEAD_DURATION = 3.0f;      // 敗北時のフェードにかかる時間
    static constexpr DirectX::XMFLOAT4 DEFAULT_SPIN_ATTACK_DESCRIPTION_UI_COLOR = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);      // 回転攻撃用説明UIの色
    static constexpr DirectX::XMFLOAT4 PLAYER_HP_BAR_COLOR = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);      // 回転攻撃用説明UIの色
public:
    static const std::shared_ptr<Object>& CreateScoreUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateGameTimerUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePause(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateFadeObject(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePlayerHpBarUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePlayerSpecialBar(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePlayerSpecialGage(const std::shared_ptr<Object>&, DirectX::XMFLOAT2 pos);
    static const std::shared_ptr<Object>& CreateDescriptionUI(const std::shared_ptr<Object>&);
};