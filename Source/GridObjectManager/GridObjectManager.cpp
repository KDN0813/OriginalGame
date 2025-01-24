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
#endif // _DEBUG


GridObjectManager::GridObjectManager()
    : Singleton(this)
{
    // �X�e�[�W�̃T�C�Y�ݒ�i�X�e�[�W�͐����`�j
    this->stage_size = StageConstant::STAGE_SIZE;

    // 1�s������̃Z�������v�Z
    this->max_cells_per_row = static_cast<int>(this->stage_size / CELL_SIZE);

    // �G���A���Ƃ̃f�[�^���i�[����R���e�i�̃T�C�Y���m��
    this->grid_cells.resize(this->max_cells_per_row * this->max_cells_per_row);

    // �X�e�[�W�̔����̃T�C�Y
    float half_stage_size = this->stage_size * 0.5f;

    // ����ƉE���̍��W���v�Z
    this->grid_min_position = { -half_stage_size, 0.0f, -half_stage_size };
    this->grid_max_position = { half_stage_size, 0.0f, half_stage_size };
}

bool GridObjectManager::RegisterObject(const std::shared_ptr<Object>& object, DirectX::XMFLOAT3 position)
{
    // �I�u�W�F�N�g�܂��̓g�����X�t�H�[���������ȏꍇ�͓o�^���s
    if (!object) return false;

    const int cell_index = GetCellIndex(position);
    if (IndexErrorCheck(cell_index)) return false;

    // �C���f�b�N�X���͈͊O�̏ꍇ�͓o�^���s
    if (cell_index >= static_cast<int>(this->grid_cells.size())) return false;

    if (this->grid_cells[cell_index].contained_object.expired()) return false;

    // �Y���Z���ɃI�u�W�F�N�g��o�^
    this->grid_cells[cell_index].contained_object = object;
    return true;
}

bool GridObjectManager::RegisterObject(const std::shared_ptr<Object>& object, int cell_index)
{
    // �C���f�b�N�X���͈͊O�̏ꍇ�͓o�^���s
    if (IndexErrorCheck(cell_index)) return false;

    // �Y���Z���ɃI�u�W�F�N�g��o�^
    this->grid_cells[cell_index].contained_object = object;
    return true;
}

bool GridObjectManager::IsObjectInCell(const int cell_index)
{
    if (IndexErrorCheck(cell_index)) return false;

    return (!this->grid_cells[cell_index].contained_object.expired());
}

bool GridObjectManager::IsObjectInCell(const std::shared_ptr<Object>& object, int cell_index)
{
    if (IndexErrorCheck(cell_index)) return false;

    return this->grid_cells[cell_index].contained_object.lock() == object;
}

const GridObjectManager::GridCell& GridObjectManager::GetCellDataAtPosition(const DirectX::XMFLOAT3 position)
{
    _ASSERT_EXPR_W(!this->grid_cells.size(), L"grid_cells�̃T�C�Y��0�ł�");

    const int cell_index = GetCellIndex(position);
    if (cell_index < 0) return this->grid_cells[0];

    // �C���f�b�N�X���͈͊O�̏ꍇ�͓o�^���s
    _ASSERT_EXPR_W(cell_index <= static_cast<int>(this->grid_cells.size()), L"cell_index��grid_cells�̃T�C�Y�𒴂��Ă��܂�");
    if (cell_index >= static_cast<int>(this->grid_cells.size())) return this->grid_cells[0];

    // �Y���Z���ɕԂ�
    return this->grid_cells[cell_index];
}

const GridObjectManager::GridCell& GridObjectManager::GetCellDataAtPosition(const int cell_index)
{
    _ASSERT_EXPR_W(!IndexErrorCheck(cell_index), L"cell_index�̒l���ُ�ł�");
    if (IndexErrorCheck(cell_index)) return this->grid_cells[0];

    // �Y���Z���ɕԂ�
    return this->grid_cells[cell_index];
}

const int GridObjectManager::GetCellIndex(const DirectX::XMFLOAT3 position)
{
    // �O���b�h�̍������ɂ������W�v�Z
    const MYVECTOR3 grid_origin = this->grid_min_position;   // �O���b�h�̍�����W
    const MYVECTOR3 world_position = position; // �I�u�W�F�N�g�̃��[���h���W
    const MYVECTOR3 local_position = world_position - grid_origin;              // �O���b�h��ł̃��[�J�����W

    DirectX::XMFLOAT3 position_float{};
    local_position.GetFlaot3(position_float);

    // XZ���ʓ��ŃO���b�h�͈͊O�̏ꍇ�͓o�^���s
    if (position_float.x < 0.0f || position_float.z < 0.0f) return -1;

    // ���[�J�����W����Z���C���f�b�N�X���v�Z
    int cell_x = static_cast<int>(position_float.x / CELL_SIZE);
    int cell_z = static_cast<int>(position_float.z / CELL_SIZE);

    const int cell_index = cell_z * this->max_cells_per_row + cell_x;
    
    _ASSERT_EXPR_W(!IndexErrorCheck(cell_index), L"cell_index�̒l���ُ�ł�");
    if (IndexErrorCheck(cell_index)) return 0;

    return cell_index;
}

const DirectX::XMFLOAT3 GridObjectManager::GetCellCenter(const DirectX::XMFLOAT3 position)
{
    return GetCellCenter(GetCellIndex(position));
}

const DirectX::XMFLOAT3 GridObjectManager::GetCellCenter(int cell_index)
{
    _ASSERT_EXPR_W(!IndexErrorCheck(cell_index), L"cell_index�̒l���ُ�ł�");
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
    for (auto& grid_cell : this->grid_cells)
    {
        grid_cell.contained_object.reset();
    }
}

void GridObjectManager::ReleaseObject(const std::shared_ptr<Object>& object, DirectX::XMFLOAT3 position)
{
    ReleaseObject(object, GetCellIndex(position));
}

void GridObjectManager::ReleaseObject(const std::shared_ptr<Object>& object, const int cell_index)
{
    _ASSERT_EXPR_W(object, L"object��nullptr�ł�");
    if (!object) return;
    _ASSERT_EXPR_W(!IndexErrorCheck(cell_index), L"cell_index�̒l���ُ�ł�");
    if (IndexErrorCheck(cell_index)) return;

    if (this->grid_cells[cell_index].contained_object.lock() != object) return;
    this->grid_cells[cell_index].contained_object.reset();  // �I�u�W�F�N�g�̓o�^����������
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
    }
    ImGui::End();
}

void GridObjectManager::DrawDebugPrimitive()
{
    if (!this->is_draw_debug_primitive) return;

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

        // �G���A���ɃI�u�W�F�N�g�����݂��邩
        bool is_object = (this->grid_cells[i].contained_object.lock() != nullptr);
        DirectX::XMFLOAT4 color = is_object ? DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
        debug_primitive_render->DrawBox(position, {}, { HALF_CELL_SIZE, HALF_CELL_SIZE, HALF_CELL_SIZE }, color);
    }
}

#endif // _DEBUG
