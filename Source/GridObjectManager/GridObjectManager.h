#pragma once

#include "System\ClassBase\Singleton.h"
#include <vector>
#include <memory>
#include <DirectXMath.h>

class Object;
class Transform3DComponent;

class GridObjectManager : public Singleton<GridObjectManager>
{
public:
    // エリア毎に格納するデータ
    struct GridData
    {
        std::weak_ptr<Object> object;   // 現在このエリアに入っているオブジェクト
    };
public:
    static constexpr float AREA_RAGE = 2.0f;    // エリアサイズ
    static constexpr float HALF_AREA_RAGE = AREA_RAGE * 0.5f;    // エリアサイズ
    float STAGE_RAGE{};  // ステージのサイズ
    int CELL_MAX{};
public:
    GridObjectManager();
    ~GridObjectManager() {};

    // 自身をエリアに登録する
    // 登録に失敗、既に登録されている場合はfalseを返す
    bool Check(std::shared_ptr<Object> object, std::shared_ptr<Transform3DComponent> transform);

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
