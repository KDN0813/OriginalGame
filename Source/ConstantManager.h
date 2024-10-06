#pragma once
#include "System/ClassBase/Singleton.h"
#include "Model/ModelCommonData.h"

enum PLAYER_ANIMATION : AnimeIndex
{
    IDLE = 0,
    MOVE_FWD,
    MOVE_BWD,
    MOVE_LFT,
    MOVE_RGT,
    JUMP,
    DAMAGE,
    DEAD,
    DEAD_STAY,
    ATTACK01,
    ATTACK02,
    ATTACK03,
};

// �萔�}�l�[�W���[
class ConstantManager : public Singleton<ConstantManager>
{
public:
    ConstantManager() :Singleton(this) {}
    ~ConstantManager(){}
};