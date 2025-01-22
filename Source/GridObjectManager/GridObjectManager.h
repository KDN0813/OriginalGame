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
    // �G���A���Ɋi�[����f�[�^
    struct GridData
    {
        std::weak_ptr<Object> object;   // ���݂��̃G���A�ɓ����Ă���I�u�W�F�N�g
    };
public:
    static constexpr float AREA_RAGE = 2.0f;    // �G���A�T�C�Y
    static constexpr float HALF_AREA_RAGE = AREA_RAGE * 0.5f;    // �G���A�T�C�Y
    float STAGE_RAGE{};  // �X�e�[�W�̃T�C�Y
    int CELL_MAX{};
public:
    GridObjectManager();
    ~GridObjectManager() {};

    // ���g���G���A�ɓo�^����
    // �o�^�Ɏ��s�A���ɓo�^����Ă���ꍇ��false��Ԃ�
    bool Check(std::shared_ptr<Object> object, std::shared_ptr<Transform3DComponent> transform);

private:
    DirectX::XMFLOAT3 min_erea_pos;   // �S�̂̍���̍��W
    DirectX::XMFLOAT3 max_erea_pos;   // �S�̂̉E���̍��W
    std::vector<GridData> grid_data_pool;

#ifdef _DEBUG
public:
    void DrawDebugGUI();
    void DrawDebugPrimitive();

#endif // _DEBUG
};
