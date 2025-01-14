#pragma once

#include <memory>
#include <string>
#include "System\MyHash.h"

class Object;

class CameraConstant
{
    //¦ƒƒCƒ“ƒJƒƒ‰‚ÍPlayerObject‚ÅŠÇ—‚µ‚Ä‚¢‚Ü‚·
public:
    static const std::shared_ptr<Object>& CreateDeathCamera(const std::shared_ptr<Object>& object);
};