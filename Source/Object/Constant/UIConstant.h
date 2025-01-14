#pragma once

#include <memory>

class Object;

class UIConstant
{
public:
    // FadeObject�p
    static constexpr float DEFAULT_FEAD_DURATION = 2.0f;            // �ʏ�̃t�F�[�h�ɂ����鎞��
    static constexpr float PLAYER_DEFEAT_FEAD_DURATION = 3.0f;      // �s�k���̃t�F�[�h�ɂ����鎞��
public:
    static const std::shared_ptr<Object>& CreateScoreUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateGameTimerUI(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePause(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateFadeObject(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreatePlayerHpBarUI(const std::shared_ptr<Object>&);
};