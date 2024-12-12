#pragma once

#include <memory>
#include "Model/ModelCommonData.h"

class Object;

class PlayerConstant
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
        SPIN_ATTACK,
    };

public:
    static const std::shared_ptr<Object>& CreatePlayer(const std::shared_ptr<Object>& object);
};