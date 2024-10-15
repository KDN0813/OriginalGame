#pragma once
#include "System/ClassBase/Singleton.h"
#include "Model/ModelCommonData.h"

class PlayerCT
{
public:
    enum ANIMATION : AnimeIndex
    {
        IDLE = 0,
        MOVE_FWD,
        MOVE_BWD,
        MOVE_LFT,
        MOVE_RGT,
        DAMAGE,
        DEAD,
        DEAD_STAY,
        ATTACK01,
        ATTACK02,
        ATTACK03,
    };
};

class EnemyCT
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
};

// 定数マネージャー
class ConstantManager : public Singleton<ConstantManager>
{
public:
    ConstantManager() :Singleton(this) {}
    ~ConstantManager(){}
};