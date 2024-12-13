#pragma once

#include <memory>

class Object;

class StageConstant
{
public:
    static const std::shared_ptr<Object>& CreateStageFloor(const std::shared_ptr<Object>& object);
    static const std::shared_ptr<Object>& CreateStageWall(const std::shared_ptr<Object>& object);
};
