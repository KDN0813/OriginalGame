#pragma once

#include <memory>
#include <DirectXMath.h>
#include "Model/ModelCommonData.h"

#include "Object\Object.h"

class EnemyConstant
{
public:
    enum ANIMATION : AnimeIndex
    {
        IDLE_CHEST = 0,
        IDLE_NORMAL,
        IDLE_BATTLE,
        ATTACK01,
        ATTACK02,
        TAUNTING,
        VICTORY,
        MOVE_FWD,
        MOVE_BWD,
        MOVE_LFT,
        MOVE_RGT,
        RUN,
        SENSESOMETHINGST,
        SENSESOMETHINGRPT,
        GETHIT,
        DIZZY,
        DIE,

        MAX,
    };

public:
    static const MyHash ATTACK_OBJECT_NAME;

    static constexpr float MIN_IDLE_TIME = 0.5f;
    static constexpr float MAX_IDLE_TIME = 5.0f;
    static constexpr float REMOVE_IDLE_TIME = 1.0f; // ���S�A�j���[�V�����Đ��I������폜�����܂ł̎���

#ifdef _DEBUG
    static constexpr size_t ENEMY_MAX = 200;
    static constexpr float DEFAULT_TERRITORY_RENGR = 185.0f;        // �G�̈ړ��͈�
#else
    static constexpr size_t ENEMY_MAX = 3000;
    static constexpr float DEFAULT_TERRITORY_RENGR = 185.0f;    // �G�̈ړ��͈�
#endif // _DEBUG
    static constexpr float NEAR_PLAYER_TERRITORY_RENGR = 100.0f;     // �v���C���[�t�߂ɐ������鎞�͈̔�
public:
    static const std::shared_ptr<Object>& CreateEnemy(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateEnemy(const DirectX::XMFLOAT3 spawn_point, const std::shared_ptr<Object>&);
};