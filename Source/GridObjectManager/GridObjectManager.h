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
    struct GridData
    {
        std::weak_ptr<Object> contained_object;     // このエリアに現在登録されているオブジェクト
    };

public:
    static constexpr float CELL_SIZE = 2.0f;        // 各グリッドセルのサイズ
    static constexpr float HALF_CELL_SIZE = CELL_SIZE * 0.5f; // グリッドセルの半分のサイズ
    float push_out_force = 2.0f;
    float stage_size{};                              // ステージ全体のサイズ
    int max_cells_per_row{};                         // ステージの1行あたりのセル数

    // TODO 変数名変更する
    const int BLOCK_GRID_INDEX_Z1 = 5;
    const int BLOCK_GRID_INDEX_Z2 = 195;
    const int BLOCK_GRID_INDEX_X1 = 5;
    const int BLOCK_GRID_INDEX_X2 = 193;
public:
    GridObjectManager();
    ~GridObjectManager() override{}

    // オブジェクトをグリッドグリッドに登録する関数
    // 登録に失敗、またはすでに登録済みの場合はfalseを返す
    bool RegisterObject(const std::shared_ptr<Object>&, DirectX::XMFLOAT3 position);
    bool RegisterObject(const std::shared_ptr<Object>&, const int cell_index);

    // グリッドにオブジェクトが存在するかどうかを確認する
    bool IsObjectInGrid(const int cell_index);
    // グリッドに特定のオブジェクトが存在するか
    bool IsObjectInGrid(const std::shared_ptr<Object>& target_object, int cell_index);

    // グリッドが侵入不可であるか
    bool IsGridBlocked(const int cell_index);

    // グリッド間の距離を計算する関数
    int GetDistanceInGrid(const int cell_index_a, const int cell_index_b);
    int GetDistanceInGrid(const DirectX::XMFLOAT3 positon_a, const DirectX::XMFLOAT3 positon_b);

    // グリッドデータを取得する
    const GridData& GetGridDataAtPosition(const DirectX::XMFLOAT3 position);
    const GridData& GetGridDataAtPosition(const int cell_index);

    // グリッドインデックスを取得する
    // 取得に失敗すると-1が返ってくる
    const int GetGridIndex(const DirectX::XMFLOAT3 position);

    // グリッドの中心を取得
    const DirectX::XMFLOAT3 GetGridCenter(const DirectX::XMFLOAT3 position);
    const DirectX::XMFLOAT3 GetGridCenter(int cell_index);

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
    std::vector<GridData> grid_cells;      // グリッドセルのデータを保持するプール

    std::shared_ptr<Object> dummy_object;   // 侵入不可エリアに設定するダミーオブジェクト
#ifdef _DEBUG
public:
    void DrawDebugGUI();      // デバッグ用のGUI描画
    void DrawDebugPrimitive(); // デバッグ用のプリミティブ描画

private:
    bool is_draw_debug_primitive = false;
    int visible_cell_range = 24;    // 描画するグリッドの数
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;

#endif // _DEBUG
};
