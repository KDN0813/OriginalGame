#pragma once

#include <memory>
#include <DirectXMath.h>
#include "System\MyHash.h"

class Object;

class UIConstant
{
public:
    // FadeObject�p
    static constexpr float DEFAULT_FEAD_DURATION = 2.0f;            // �ʏ�̃t�F�[�h�ɂ����鎞��
    static constexpr float PLAYER_DEFEAT_FEAD_DURATION = 3.0f;      // �s�k���̃t�F�[�h�ɂ����鎞��
    
    static constexpr DirectX::XMFLOAT4 DEFAULT_SPIN_ATTACK_DESCRIPTION_UI_COLOR = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);      // ��]�U���p����UI�̐F
    static constexpr DirectX::XMFLOAT4 PLAYER_HP_BAR_COLOR = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);      // ��]�U���p����UI�̐F
    static constexpr DirectX::XMFLOAT4 PLAYER_SPECIAL_BAR_COLOR = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
    static constexpr DirectX::XMFLOAT4 PLAYER_SPECIAL_BAR_FLASH_COLOR = DirectX::XMFLOAT4(0.7f, 0.7f, 0.0f, 1.0f);

    static const MyHash SPIN_SPECIAL_GAGE_OBJECT_NAME;
    
    static constexpr float SPECIAL_POINT_BAR_FLASH_TIME = 0.1f;
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