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
    // �f�[�^���i�[���邽�߂̍\���́i�e�G���A�̃f�[�^�j
    struct GridData
    {
        std::weak_ptr<Object> contained_object;     // ���̃G���A�Ɍ��ݓo�^����Ă���I�u�W�F�N�g
    };

public:
    static constexpr float CELL_SIZE = 2.0f;        // �e�O���b�h�Z���̃T�C�Y
    static constexpr float HALF_CELL_SIZE = CELL_SIZE * 0.5f; // �O���b�h�Z���̔����̃T�C�Y
    float push_out_force = 2.0f;
    float stage_size{};                              // �X�e�[�W�S�̂̃T�C�Y
    int max_cells_per_row{};                         // �X�e�[�W��1�s������̃Z����

    // TODO �ϐ����ύX����
    const int BLOCK_GRID_INDEX_Z1 = 5;
    const int BLOCK_GRID_INDEX_Z2 = 195;
    const int BLOCK_GRID_INDEX_X1 = 5;
    const int BLOCK_GRID_INDEX_X2 = 193;
public:
    GridObjectManager();
    ~GridObjectManager() override{}

    // �I�u�W�F�N�g���O���b�h�O���b�h�ɓo�^����֐�
    // �o�^�Ɏ��s�A�܂��͂��łɓo�^�ς݂̏ꍇ��false��Ԃ�
    bool RegisterObject(const std::shared_ptr<Object>&, DirectX::XMFLOAT3 position);
    bool RegisterObject(const std::shared_ptr<Object>&, const int cell_index);

    // �O���b�h�ɃI�u�W�F�N�g�����݂��邩�ǂ������m�F����
    bool IsObjectInGrid(const int cell_index);
    // �O���b�h�ɓ���̃I�u�W�F�N�g�����݂��邩
    bool IsObjectInGrid(const std::shared_ptr<Object>& target_object, int cell_index);

    // �O���b�h���N���s�ł��邩
    bool IsGridBlocked(const int cell_index);

    // �O���b�h�Ԃ̋������v�Z����֐�
    int GetDistanceInGrid(const int cell_index_a, const int cell_index_b);
    int GetDistanceInGrid(const DirectX::XMFLOAT3 positon_a, const DirectX::XMFLOAT3 positon_b);

    // �O���b�h�f�[�^���擾����
    const GridData& GetGridDataAtPosition(const DirectX::XMFLOAT3 position);
    const GridData& GetGridDataAtPosition(const int cell_index);

    // �O���b�h�C���f�b�N�X���擾����
    // �擾�Ɏ��s�����-1���Ԃ��Ă���
    const int GetGridIndex(const DirectX::XMFLOAT3 position);

    // �O���b�h�̒��S���擾
    const DirectX::XMFLOAT3 GetGridCenter(const DirectX::XMFLOAT3 position);
    const DirectX::XMFLOAT3 GetGridCenter(int cell_index);

    // �N���b�h�Z���ɓo�^����Ă���I�u�W�F�N�g�����Z�b�g����
    void ClearGridCell();

    // �I�u�W�F�N�g���O���b�h�Z�������������֐�
    void ReleaseObject(const std::shared_ptr<Object>&, DirectX::XMFLOAT3 position);
    void ReleaseObject(const std::shared_ptr<Object>&, const int cell_index);

    // �C���f�b�N�X�̃G���[�`�F�b�N
    // �l���ُ�Ȃ�true��Ԃ�
    bool IndexErrorCheck(const int cell_index);
private:
    DirectX::XMFLOAT3 grid_min_position;   // �X�e�[�W�S�̂̍���̃��[���h���W
    DirectX::XMFLOAT3 grid_max_position;   // �X�e�[�W�S�̂̉E���̃��[���h���W
    std::vector<GridData> grid_cells;      // �O���b�h�Z���̃f�[�^��ێ�����v�[��

    std::shared_ptr<Object> dummy_object;   // �N���s�G���A�ɐݒ肷��_�~�[�I�u�W�F�N�g
#ifdef _DEBUG
public:
    void DrawDebugGUI();      // �f�o�b�O�p��GUI�`��
    void DrawDebugPrimitive(); // �f�o�b�O�p�̃v���~�e�B�u�`��

private:
    bool is_draw_debug_primitive = false;
    int visible_cell_range = 24;    // �`�悷��O���b�h�̐�
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;

#endif // _DEBUG
};
