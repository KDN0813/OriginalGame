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
    // データを格納するための構造体（各エリアのデータ）
    struct GridCell
    {
        std::weak_ptr<Object> contained_object;   // このエリアに現在登録されているオブジェクト
    };

public:
    static constexpr float CELL_SIZE = 2.0f;        // 各グリッドセルのサイズ
    static constexpr float HALF_CELL_SIZE = CELL_SIZE * 0.5f; // グリッドセルの半分のサイズ
    float stage_size{};                              // ステージ全体のサイズ
    int max_cells_per_row{};                         // ステージの1行あたりのセル数

public:
    GridObjectManager();
    ~GridObjectManager() {}

    // オブジェクトをグリッドセルに登録する関数
    // 登録に失敗、またはすでに登録済みの場合はfalseを返す
    bool RegisterObject(std::shared_ptr<Object> object, std::shared_ptr<Transform3DComponent> transform);

private:
    DirectX::XMFLOAT3 grid_min_position;   // ステージ全体の左上のワールド座標
    DirectX::XMFLOAT3 grid_max_position;   // ステージ全体の右下のワールド座標
    std::vector<GridCell> grid_cells;      // グリッドセルのデータを保持するプール

#ifdef _DEBUG
public:
    void DrawDebugGUI();      // デバッグ用のGUI描画
    void DrawDebugPrimitive(); // デバッグ用のプリミティブ描画

#endif // _DEBUG
};
