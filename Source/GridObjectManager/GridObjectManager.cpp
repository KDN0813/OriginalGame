#include "GridObjectManager.h"

#include "System\MyMath\MYVECTOR3.h"
#include "Object\Object.h"

#include "Component\EnemyComponent.h"
#include "Component\TransformComponent.h"

#ifdef _DEBUG
#include "Debug\DebugManager.h"
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG


GridObjectManager::GridObjectManager()
    : Singleton(this)
{
    // ステージのサイズ設定（ステージは正方形）
    this->stage_size = EnemyComponent::DEFAULT_TERRITORY_RENGR;

    // 1行あたりのセル数を計算
    this->max_cells_per_row = static_cast<int>(this->stage_size / CELL_SIZE);

    // エリアごとのデータを格納するコンテナのサイズを確保
    this->grid_cells.resize(this->max_cells_per_row * this->max_cells_per_row);

    // ステージの半分のサイズ
    float half_stage_size = this->stage_size * 0.5f;

    // 左上と右下の座標を計算
    this->grid_min_position = { -half_stage_size, 0.0f, -half_stage_size };
    this->grid_max_position = { half_stage_size, 0.0f, half_stage_size };
}

bool GridObjectManager::RegisterObject(std::shared_ptr<Object> object, DirectX::XMFLOAT3 position)
{
    // オブジェクトまたはトランスフォームが無効な場合は登録失敗
    if (!object) return false;

    // グリッドの左上を基準にした座標計算
    const MYVECTOR3 grid_origin = this->grid_min_position;   // グリッドの左上座標
    const MYVECTOR3 world_position = position; // オブジェクトのワールド座標
    const MYVECTOR3 local_position = world_position - grid_origin;              // グリッド基準でのローカル座標

    DirectX::XMFLOAT3 position_float{};
    local_position.GetFlaot3(position_float);

    // XZ平面内でグリッド範囲外の場合は登録失敗
    if (position_float.x < 0.0f || position_float.z < 0.0f) return false;

    // ローカル座標からセルインデックスを計算
    int cell_x = static_cast<int>(position_float.x / CELL_SIZE);
    int cell_z = static_cast<int>(position_float.z / CELL_SIZE);

    int cell_index = cell_z * this->max_cells_per_row + cell_x;

    // インデックスが範囲外の場合は登録失敗
    if (cell_index >= static_cast<int>(this->grid_cells.size())) return false;

    // 該当セルにオブジェクトを登録
    this->grid_cells[cell_index].contained_object = object;
    return true;
}

#ifdef _DEBUG

void GridObjectManager::DrawDebugGUI()
{
    if (ImGui::Begin("GridObjectManager"))
    {
        ImGui::InputFloat3("grid_min_position", &this->grid_min_position.x);
    }
    ImGui::End();
}

void GridObjectManager::DrawDebugPrimitive()
{
    DebugManager::Instance debug_manager = DebugManager::GetInstance();
    const auto& debug_primitive_render = debug_manager->GetDebugPrimitiveRenderer();
    if (!debug_primitive_render) return;

    for (int i = 0; i < this->max_cells_per_row * this->max_cells_per_row; ++i)
    {
        float x = static_cast<float>(i % this->max_cells_per_row);
        float z = static_cast<float>(i / this->max_cells_per_row);
        DirectX::XMFLOAT3 position = {
            this->grid_min_position.x + HALF_CELL_SIZE + x * CELL_SIZE,
            0.0f,
            this->grid_min_position.z + HALF_CELL_SIZE + z * CELL_SIZE
        };

        // エリア内にオブジェクトが存在するか
        bool is_object = (this->grid_cells[i].contained_object.lock() != nullptr);
        DirectX::XMFLOAT4 color = is_object ? DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) : DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        debug_primitive_render->DrawBox(position, {}, { HALF_CELL_SIZE, HALF_CELL_SIZE, HALF_CELL_SIZE }, color);
    }
}

void GridObjectManager::ClearGridObject()
{
    for (auto& grid_cell : this->grid_cells)
    {
        grid_cell.contained_object.reset();
    }
}

#endif // _DEBUG
