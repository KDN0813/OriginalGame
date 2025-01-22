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
        std::weak_ptr<Object> contained_object;   // ���̃G���A�Ɍ��ݓo�^����Ă���I�u�W�F�N�g
    };

public:
    static constexpr float CELL_SIZE = 2.0f;        // �e�O���b�h�Z���̃T�C�Y
    static constexpr float HALF_CELL_SIZE = CELL_SIZE * 0.5f; // �O���b�h�Z���̔����̃T�C�Y
    float stage_size{};                              // �X�e�[�W�S�̂̃T�C�Y
    int max_cells_per_row{};                         // �X�e�[�W��1�s������̃Z����

public:
    GridObjectManager();
    ~GridObjectManager() {}

    // �I�u�W�F�N�g���O���b�h�Z���ɓo�^����֐�
    // �o�^�Ɏ��s�A�܂��͂��łɓo�^�ς݂̏ꍇ��false��Ԃ�
    bool RegisterObject(std::shared_ptr<Object> object, std::shared_ptr<Transform3DComponent> transform);

private:
    DirectX::XMFLOAT3 grid_min_position;   // �X�e�[�W�S�̂̍���̃��[���h���W
    DirectX::XMFLOAT3 grid_max_position;   // �X�e�[�W�S�̂̉E���̃��[���h���W
    std::vector<GridCell> grid_cells;      // �O���b�h�Z���̃f�[�^��ێ�����v�[��

#ifdef _DEBUG
public:
    void DrawDebugGUI();      // �f�o�b�O�p��GUI�`��
    void DrawDebugPrimitive(); // �f�o�b�O�p�̃v���~�e�B�u�`��

#endif // _DEBUG
};
