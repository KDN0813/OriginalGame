#pragma once

#include <memory>
#include <string>
#include "System\MyHash.h"
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

    static const MyHash MODEL_FILENAME;
    static const MyHash PLAYER_CAMERA_NAME;
    static const MyHash DEATH_CAMERA_NAME;
    static const MyHash ATTACK_OBJECT_NAME;
    static const MyHash SPIN_ATTACK_OBJECT_NAME;
public:
    static const std::shared_ptr<Object>& CreatePlayer(const std::shared_ptr<Object>& object);
};