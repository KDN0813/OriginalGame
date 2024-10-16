#pragma once
#include <DirectXMath.h>
#include <memory>

class Object;

using CollisionType = size_t;
using TargetType = size_t;

// �R���C�_�[�̎�ނ��`����񋓌^
// �I�u�W�F�N�g���Փ˂̍ۂɂǂ̖������ʂ��������w�肷��
enum class COLLISION_TYPE : CollisionType
{
    ATTACKER = 0,    // �Ԃ��鑤�i�A�N�e�B�u�ɏՓ˂������N�����I�u�W�F�N�g�j
    DEFENDER,        // �Ԃ����鑤�i�Փ˂����I�u�W�F�N�g�j
    MAX,
    NONE,
};

// �ΏۂƂȂ�^�[�Q�b�g�̎�ނ��`����񋓌^
// ATTACKER�i�U���ҁj����� DEFENDER�i�h��ҁj�Ŏg�p���A
// ���ꂼ��ǂ̃^�C�v�̑�����^�[�Q�b�g�Ƃ��邩���w�肷��
enum class TARGET_TYPE : TargetType
{
    PLAYER = 0,         // �v���C���[���^�[�Q�b�g�Ƃ���
    ENEMY,              // �G���^�[�Q�b�g�Ƃ���
    MAX,
    NONE,
};

struct CircleParam
{
    DirectX::XMFLOAT2 center;
    float radius;
};

struct CircleHitResult
{
    DirectX::XMFLOAT2 out_position;
    std::weak_ptr<Object> hit_object_Wptr;
};

/**
* ���C�̃q�b�g����
*
* .\param position ���C�Ƃƃ|���S���̌�_
* .\param normal �Փ˂����|���S���̖@���x�N�g��
* .\param rotation ��]��
* .\param distance ���C�̎n�_�����_�܂ł̋���
* .\param material_index �Փ˂����|���S���̃}�e���A���ԍ�
*/
struct RayHitResult
{
    DirectX::XMFLOAT3   position = {};
    DirectX::XMFLOAT3   normal = {};
    DirectX::XMFLOAT3   rotation = {};
    float               distance = 0.0f;
    int                 material_index = -1;
};
