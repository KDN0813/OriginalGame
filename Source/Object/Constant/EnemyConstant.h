#pragma once

#include <memory>
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
    static constexpr float REMOVE_IDLE_TIME = 2.0f; // 死亡アニメーション再生終了から削除されるまでの時間
public:
    static const std::shared_ptr<Object>& CreateEnemy(const std::shared_ptr<Object>&);
};