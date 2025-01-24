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
        std::weak_ptr<Object> contained_object;     // このエリアに現在登録されているオブジェクト
    };

public:
    static constexpr float CELL_SIZE = 2.0f;        // 各グリッドセルのサイズ
    static constexpr float HALF_CELL_SIZE = CELL_SIZE * 0.5f; // グリッドセルの半分のサイズ
    float push_out_force = 2.0f;
    float stage_size{};                              // ステージ全体のサイズ
    int max_cells_per_row{};                         // ステージの1行あたりのセル数

public:
    GridObjectManager();
    ~GridObjectManager() {}

    // オブジェクトをグリッドセルに登録する関数
    // 登録に失敗、またはすでに登録済みの場合はfalseを返す
    bool RegisterObject(const std::shared_ptr<Object>&, DirectX::XMFLOAT3 position);
    bool RegisterObject(const std::shared_ptr<Object>&, const int cell_index);

    // セルにオブジェクトが存在するかどうかを確認する
    bool IsObjectInCell(const int cell_index);
    // セルに特定のオブジェクトが存在するか
    bool IsObjectInCell(const std::shared_ptr<Object>& target_object, int cell_index);

    // セルデータを取得する
    const GridCell& GetCellDataAtPosition(const DirectX::XMFLOAT3 position);
    const GridCell& GetCellDataAtPosition(const int cell_index);

    // セルインデックスを取得する
    // 取得に失敗すると-1が返ってくる
    const int GetCellIndex(const DirectX::XMFLOAT3 position);

    // セルの中心を取得
    const DirectX::XMFLOAT3 GetCellCenter(const DirectX::XMFLOAT3 position);
    const DirectX::XMFLOAT3 GetCellCenter(int cell_index);

    // クリッドセルに登録されているオブジェクトをリセットする
    void ClearGridCell();

    // オブジェクトをグリッドセルから解除する関数
    void ReleaseObject(const std::shared_ptr<Object>&, DirectX::XMFLOAT3 position);
    void ReleaseObject(const std::shared_ptr<Object>&, const int cell_index);

    // インデックスのエラーチェック
    // 値が異常ならtrueを返す
    bool IndexErrorCheck(const int cell_index);
private:
    DirectX::XMFLOAT3 grid_min_position;   // ステージ全体の左上のワールド座標
    DirectX::XMFLOAT3 grid_max_position;   // ステージ全体の右下のワールド座標
    std::vector<GridCell> grid_cells;      // グリッドセルのデータを保持するプール

#ifdef _DEBUG
public:
    void DrawDebugGUI();      // デバッグ用のGUI描画
    void DrawDebugPrimitive(); // デバッグ用のプリミティブ描画

private:
    bool is_draw_debug_primitive = false;

#endif // _DEBUG
};
