#pragma once

#include <memory>

class Object;

class StageConstant
{
public:
    static constexpr float STAGE_SIZE = 400.0f;    // ステージサイズ
    static constexpr float STAGE_FLOOR_Y = 0.5f;   // ステージの床のY座標
public:
    static const std::shared_ptr<Object>& CreateStageFloor(const std::shared_ptr<Object>& object);
    static const std::shared_ptr<Object>& CreateStageWall(const std::shared_ptr<Object>& object);
};
