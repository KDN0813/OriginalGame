#pragma once

#include <memory>
#include <string>
#include "System\MyHash.h"

class Object;

class CameraConstant
{
    //�����C���J������PlayerObject�ŊǗ����Ă��܂�
public:
    static const std::shared_ptr<Object>& CreateDeathCamera(const std::shared_ptr<Object>& object);
};