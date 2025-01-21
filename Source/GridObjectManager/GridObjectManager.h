#pragma once

#include "System\ClassBase\Singleton.h"
#include <vector>
#include <memory>
#include <DirectXMath.h>

class Object;

class GridObjectManager : public Singleton<GridObjectManager>
{
public:
    // エリア毎に格納するデータ
    struct GridData
    {
        std::weak_ptr<Object> object;   // 現在このエリアに入っているオブジェクト
    };
public:
    static constexpr float AREA_RAGE = 5.0f;    // エリアサイズ
    float STAGE_RAGE{};  // ステージのサイズ
    int CELL_MAX{};
public:
    GridObjectManager();
    ~GridObjectManager() {};

private:
    DirectX::XMFLOAT3 min_erea_pos;   // 全体の左上の座標
    DirectX::XMFLOAT3 max_erea_pos;   // 全体の右下の座標
    std::vector<GridData> grid_data_pool;

#ifdef _DEBUG
public:
    void DrawDebugGUI();
    void DrawDebugPrimitive();

#endif // _DEBUG
};
