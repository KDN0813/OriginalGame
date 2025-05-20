#include "GridObjectManager.h"

#include "System\MyMath\MYVECTOR3.h"
#include "Object\Object.h"
#include "System\Misc.h"
#include "Object\Constant\StageConstant.h"

#include "Component\EnemyComponent.h"
#include "Component\TransformComponent.h"

#ifdef _DEBUG
#include "Debug\DebugManager.h"
#include "Debug\ImGuiHelper.h"
#include "Object\GameObjectRegistry.h"
#endif // _DEBUG


GridObjectManager::GridObjectManager()
    : Singleton(this)
{
    // ステージのサイズ設定（ステージは正方形）
    this->stage_size = StageConstant::STAGE_SIZE;

    // 1行あたりのセル数を計算
    this->max_cells_per_row = static_cast<int>(this->stage_size / CELL_SIZE);

    // エリアごとのデータを格納するコンテナのサイズを確保
    this->grid_cells.resize(this->max_cells_per_row * this->max_cells_per_row);

    // ステージの半分のサイズ
    float half_stage_size = this->stage_size * 0.5f;

    // 左上と右下の座標を計算
    this->grid_min_position = { -half_stage_size, 0.0f, -half_stage_size };
    this->grid_max_position = { half_stage_size, 0.0f, half_stage_size };

    // ダミーオブジェクト作製
    this->dummy_object = std::make_shared<Object>();
    this->dummy_object->SetName("Dummy");
}

bool GridObjectManager::RegisterObject(const std::shared_ptr<Object>& object, DirectX::XMFLOAT3 position)
{
    // オブジェクトまたはトランスフォームが無効な場合は登録失敗
    if (!object) return false;

    const int cell_index = GetGridIndex(position);
    return RegisterObject(object, cell_index);
}

bool GridObjectManager::RegisterObject(const std::shared_ptr<Object>& object, int cell_index)
{
    // オブジェクトまたはトランスフォームが無効な場合は登録失敗
    if (!object) return false;

    // インデックスが範囲外の場合は登録失敗
    if (IndexErrorCheck(cell_index)) return false;
    // 侵入不可エリアの場合は登録失敗
    if (IsGridBlocked(cell_index)) return false;

    // 既に登録されていたら失敗
    if (!this->grid_cells[cell_index].contained_object.expired()) return false;

    // 該当セルにオブジェクトを登録
    this->grid_cells[cell_index].contained_object = object;
    return true;
}

bool GridObjectManager::IsObjectInGrid(const int cell_index)
{
    if (IndexErrorCheck(cell_index)) return false;

    return (!this->grid_cells[cell_index].contained_object.expired());
}

bool GridObjectManager::IsObjectInGrid(const std::shared_ptr<Object>& object, int cell_index)
{
    if (IndexErrorCheck(cell_index)) return false;

    return this->grid_cells[cell_index].contained_object.lock() == object;
}

bool GridObjectManager::IsGridBlocked(const int cell_index)
{
    if ((cell_index / this->max_cells_per_row <= BLOCK_GRID_INDEX_Z1)
        || (cell_index / this->max_cells_per_row >= BLOCK_GRID_INDEX_Z2)
        || (cell_index % this->max_cells_per_row <= BLOCK_GRID_INDEX_X1)
        || (cell_index % this->max_cells_per_row >= BLOCK_GRID_INDEX_X2)
        )
    {
        return true;
    }
    return false;
}

int GridObjectManager::GetDistanceInGrid(const int cell_index_a, const int cell_index_b)
{
    // セルの行と列を計算
    int row_a = cell_index_a / this->max_cells_per_row;
    int col_a = cell_index_a % this->max_cells_per_row;

    int row_b = cell_index_b / this->max_cells_per_row;
    int col_b = cell_index_b % this->max_cells_per_row;

    // 行と列の差分を計算
    int row_difference = std::abs(row_a - row_b);
    int col_difference = std::abs(col_a - col_b);

    // チェビシェフ距離を計算（斜め移動が可能な場合の最短距離）
    return (std::max)(row_difference, col_difference);
}

int GridObjectManager::GetDistanceInGrid(const DirectX::XMFLOAT3 positon_a, const DirectX::XMFLOAT3 positon_b)
{
    return GetDistanceInGrid(GetGridIndex(positon_a), GetGridIndex(positon_b));
}

const GridObjectManager::GridData& GridObjectManager::GetGridDataAtPosition(const DirectX::XMFLOAT3 position)
{
    _ASSERT_EXPR_W(!this->grid_cells.size(), L"grid_cellsのサイズが0です");

    const int cell_index = GetGridIndex(position);
    if (cell_index < 0) return this->grid_cells[0];

    // インデックスが範囲外の場合は登録失敗
    _ASSERT_EXPR_W(cell_index <= static_cast<int>(this->grid_cells.size()), L"cell_indexがgrid_cellsのサイズを超えています");
    if (cell_index >= static_cast<int>(this->grid_cells.size())) return this->grid_cells[0];

    // 該当セルに返す
    return this->grid_cells[cell_index];
}

const GridObjectManager::GridData& GridObjectManager::GetGridDataAtPosition(const int cell_index)
{
    _ASSERT_EXPR_W(!IndexErrorCheck(cell_index), L"cell_indexの値が異常です");
    if (IndexErrorCheck(cell_index)) return this->grid_cells[0];

    // 該当セルに返す
    return this->grid_cells[cell_index];
}

const int GridObjectManager::GetGridIndex(const DirectX::XMFLOAT3 position)
{
    // グリッドの左上を基準にした座標計算
    const MYVECTOR3 grid_origin = this->grid_min_position;   // グリッドの左上座標
    const MYVECTOR3 world_position = position; // オブジェクトのワールド座標
    const MYVECTOR3 local_position = world_position - grid_origin;              // グリッド基準でのローカル座標

    DirectX::XMFLOAT3 position_float{};
    local_position.GetFlaot3(position_float);

    // XZ平面内でグリッド範囲外の場合は登録失敗
    if (position_float.x < 0.0f || position_float.z < 0.0f) return -1;

    // ローカル座標からセルインデックスを計算
    int cell_x = static_cast<int>(position_float.x / CELL_SIZE);
    int cell_z = static_cast<int>(position_float.z / CELL_SIZE);

    const int cell_index = cell_z * this->max_cells_per_row + cell_x;
    
    if (IndexErrorCheck(cell_index)) return -1;

    return cell_index;
}

const DirectX::XMFLOAT3 GridObjectManager::GetGridCenter(const DirectX::XMFLOAT3 position)
{
    return GetGridCenter(GetGridIndex(position));
}

const DirectX::XMFLOAT3 GridObjectManager::GetGridCenter(int cell_index)
{
    _ASSERT_EXPR_W(!IndexErrorCheck(cell_index), L"cell_indexの値が異常です");
    if (IndexErrorCheck(cell_index)) return DirectX::XMFLOAT3();

    float x = static_cast<float>(cell_index % this->max_cells_per_row);
    float z = static_cast<float>(cell_index / this->max_cells_per_row);
    const DirectX::XMFLOAT3 position =
    {
        this->grid_min_position.x + x * CELL_SIZE + HALF_CELL_SIZE,
        0.0f,
        this->grid_min_position.z + z * CELL_SIZE + HALF_CELL_SIZE
    };

    return position;
}

void GridObjectManager::ClearGridCell()
{
    int count = 0;
    for (auto& grid_cell : this->grid_cells)
    {
        // 侵入不可エリアの設定
        if ((count / this->max_cells_per_row == BLOCK_GRID_INDEX_Z1)
            || (count / this->max_cells_per_row == BLOCK_GRID_INDEX_Z2)
            || (count % this->max_cells_per_row == BLOCK_GRID_INDEX_X1)
            || (count % this->max_cells_per_row == BLOCK_GRID_INDEX_X2)
            )
        {
            grid_cell.contained_object = this->dummy_object;
        }
        else
        {
            grid_cell.contained_object.reset();

        }
        ++count;
    }
}

void GridObjectManager::ReleaseObject(const std::shared_ptr<Object>& object, DirectX::XMFLOAT3 position)
{
    ReleaseObject(object, GetGridIndex(position));
}

void GridObjectManager::ReleaseObject(const std::shared_ptr<Object>& object, const int cell_index)
{
    _ASSERT_EXPR_W(object, L"objectがnullptrです");
    if (!object) return;
    _ASSERT_EXPR_W(!IndexErrorCheck(cell_index), L"cell_indexの値が異常です");
    if (IndexErrorCheck(cell_index)) return;

    if (this->grid_cells[cell_index].contained_object.lock() != object) return;
    this->grid_cells[cell_index].contained_object.reset();  // オブジェクトの登録を解除する
}

bool GridObjectManager::IndexErrorCheck(const int cell_index)
{
    return (cell_index >= static_cast<int>(this->grid_cells.size()) ||  cell_index < 0);
}

#ifdef _DEBUG

void GridObjectManager::DrawDebugGUI()
{
    if (ImGui::Begin("GridObjectManager"))
    {
        ImGui::Checkbox("Debug Primitive", &this->is_draw_debug_primitive);

        ImGui::InputFloat3("grid_min_position", &this->grid_min_position.x);
        ImGui::InputFloat("push_out_force", &this->push_out_force);
        ImGui::InputInt("Visible Cell Range", &this->visible_cell_range);
    }
    ImGui::End();
}

void GridObjectManager::DrawDebugPrimitive()
{
    if (!this->is_draw_debug_primitive) return;

    DebugManager::Instance debug_manager = DebugManager::GetInstance();
    const auto& debug_primitive_render = debug_manager->GetDebugPrimitiveRenderer();
    if (!debug_primitive_render) return;

    // プレイヤー取得
    GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
    const auto& player = game_object->GetPlayer();
    if (!player) return;
    const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
    if (!player_transform) return;

    // プレイヤーのいるセルのインデックス
    const int player_grid_index = GetGridIndex(player_transform->GetWorldPosition());

    for (int i = 0; i < this->max_cells_per_row * this->max_cells_per_row; ++i)
    {
        if (this->visible_cell_range < GetDistanceInGrid(i, player_grid_index)) continue;

        float x = static_cast<float>(i % this->max_cells_per_row);
        float z = static_cast<float>(i / this->max_cells_per_row);
        DirectX::XMFLOAT3 position = {
            this->grid_min_position.x + HALF_CELL_SIZE + x * CELL_SIZE,
            0.0f,
            this->grid_min_position.z + HALF_CELL_SIZE + z * CELL_SIZE
        };

        // エリア内にオブジェクトが存在するか
        bool is_object = (this->grid_cells[i].contained_object.lock() != nullptr);
        DirectX::XMFLOAT4 color = is_object ? DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
        debug_primitive_render->DrawBox(position, {}, { HALF_CELL_SIZE, HALF_CELL_SIZE, HALF_CELL_SIZE }, color);
    }
}

#endif // _DEBUG
