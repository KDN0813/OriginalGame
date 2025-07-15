#pragma once

#include <memory>
#include <DirectXMath.h>
#include "System\MyHash.h"

class Object;

class UIConstant
{
public:
    struct ScoreUIGroup
    {
        std::shared_ptr<Object> total_ui_object;        // 総スコア()
        std::shared_ptr<Object> chain_pop_ui_object;    // 連鎖スコア出現UI
        std::shared_ptr<Object> chain_move_ui_object;   // 連鎖スコア移動UI
    };
public:
    // FadeObject用
    static constexpr float DEFAULT_FEAD_DURATION = 2.0f;            // 通常のフェードにかかる時間
    static constexpr float PLAYER_DEFEAT_FEAD_DURATION = 3.0f;      // 敗北時のフェードにかかる時間
    
    static constexpr DirectX::XMFLOAT4 DEFAULT_SPIN_ATTACK_DESCRIPTION_UI_COLOR = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);      // 回転攻撃用説明UIの色
    static constexpr DirectX::XMFLOAT4 PLAYER_HP_BAR_COLOR = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);      // 回転攻撃用説明UIの色
    static constexpr DirectX::XMFLOAT4 PLAYER_SPECIAL_BAR_COLOR = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
    static constexpr DirectX::XMFLOAT4 PLAYER_SPECIAL_BAR_FLASH_COLOR = DirectX::XMFLOAT4(0.7f, 0.7f, 0.0f, 1.0f);

    static const MyHash SPIN_SPECIAL_GAGE_OBJECT_NAME;
    
    static constexpr float SPECIAL_POINT_BAR_FLASH_TIME = 0.1f;
public:
    static const std::shared_ptr<Object>& CreateGameTimerUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePause(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateFadeObject(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePlayerHpBarUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePlayerSpecialBar(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePlayerSpecialGage(const std::shared_ptr<Object>&, DirectX::XMFLOAT2 pos);
    static const std::shared_ptr<Object>& CreateDescriptionUI(const std::shared_ptr<Object>&);
    static ScoreUIGroup CreateScoreUIs(const std::shared_ptr<Object>& ,const std::shared_ptr<Object>&, const std::shared_ptr<Object>&);
};