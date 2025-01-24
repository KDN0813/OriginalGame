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
    struct GridCell
    {
        std::weak_ptr<Object> contained_object;     // ���̃G���A�Ɍ��ݓo�^����Ă���I�u�W�F�N�g
    };

public:
    static constexpr float CELL_SIZE = 2.0f;        // �e�O���b�h�Z���̃T�C�Y
    static constexpr float HALF_CELL_SIZE = CELL_SIZE * 0.5f; // �O���b�h�Z���̔����̃T�C�Y
    float push_out_force = 2.0f;
    float stage_size{};                              // �X�e�[�W�S�̂̃T�C�Y
    int max_cells_per_row{};                         // �X�e�[�W��1�s������̃Z����

public:
    GridObjectManager();
    ~GridObjectManager() {}

    // �I�u�W�F�N�g���O���b�h�Z���ɓo�^����֐�
    // �o�^�Ɏ��s�A�܂��͂��łɓo�^�ς݂̏ꍇ��false��Ԃ�
    bool RegisterObject(const std::shared_ptr<Object>&, DirectX::XMFLOAT3 position);
    bool RegisterObject(const std::shared_ptr<Object>&, const int cell_index);

    // �Z���ɃI�u�W�F�N�g�����݂��邩�ǂ������m�F����
    bool IsObjectInCell(const int cell_index);
    // �Z���ɓ���̃I�u�W�F�N�g�����݂��邩
    bool IsObjectInCell(const std::shared_ptr<Object>& target_object, int cell_index);

    // �Z���f�[�^���擾����
    const GridCell& GetCellDataAtPosition(const DirectX::XMFLOAT3 position);
    const GridCell& GetCellDataAtPosition(const int cell_index);

    // �Z���C���f�b�N�X���擾����
    // �擾�Ɏ��s�����-1���Ԃ��Ă���
    const int GetCellIndex(const DirectX::XMFLOAT3 position);

    // �Z���̒��S���擾
    const DirectX::XMFLOAT3 GetCellCenter(const DirectX::XMFLOAT3 position);
    const DirectX::XMFLOAT3 GetCellCenter(int cell_index);

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
    std::vector<GridCell> grid_cells;      // �O���b�h�Z���̃f�[�^��ێ�����v�[��

#ifdef _DEBUG
public:
    void DrawDebugGUI();      // �f�o�b�O�p��GUI�`��
    void DrawDebugPrimitive(); // �f�o�b�O�p�̃v���~�e�B�u�`��

private:
    bool is_draw_debug_primitive = false;

#endif // _DEBUG
};
