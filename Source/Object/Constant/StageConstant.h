#pragma once

#include <memory>
#include "Model/ModelCommonData.h"

class Object;

class StageConstant
{
public:
    static const std::shared_ptr<Object>& CreateStage(const std::shared_ptr<Object>& object);
};
