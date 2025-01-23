#pragma once

#include <memory>

class Object;

class StageConstant
{
public:
    static constexpr float STAGE_SIZE = 400.0f;    // ステージサイズ
public:
    static const std::shared_ptr<Object>& CreateStageFloor(const std::shared_ptr<Object>& object);
    static const std::shared_ptr<Object>& CreateStageWall(const std::shared_ptr<Object>& object);
};
