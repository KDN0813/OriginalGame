#pragma once

using CollisionType = unsigned int;
using TargetType = unsigned int;

// �R���C�_�[�̎�ނ��`����񋓌^
// �I�u�W�F�N�g���Փ˂̍ۂɂǂ̖������ʂ��������w�肷��
enum COLLISION_TYPE : CollisionType
{
    COLLISION_TYPE_NONE = 0,                                                        // �����蔻��Ȃ�
    COLLISION_TYPE_ATTACKER = 1 << 0,                                               // �Ԃ��鑤�i�A�N�e�B�u�ɏՓ˂������N�����I�u�W�F�N�g�j
    COLLISION_TYPE_DEFENDER = 1 << 1,                                               // �Ԃ����鑤�i�Փ˂����I�u�W�F�N�g�j
    COLLISION_TYPE_BOTH = COLLISION_TYPE_ATTACKER | COLLISION_TYPE_DEFENDER,        // �����ɓ����蔻�肪����ꍇ�i�Ԃ��鑤�ƂԂ����鑤�̗����j
    COLLISION_TYPE_DEFAULT = COLLISION_TYPE_BOTH,                                   // �f�t�H���g�ł͗����ɔ�����s��
};

// �ΏۂƂȂ�^�[�Q�b�g�̎�ނ��`����񋓌^
// ATTACKER�i�U���ҁj����� DEFENDER�i�h��ҁj�Ŏg�p���A
// ���ꂼ��ǂ̃^�C�v�̑�����^�[�Q�b�g�Ƃ��邩���w�肷��
enum TARGET_TYPE : TargetType
{
    TARGET_NONE = 0,                                // �^�[�Q�b�g�Ȃ�
    TARGET_PLAYER = 1 << 0,                         // �v���C���[���^�[�Q�b�g�Ƃ���
    TARGET_ENEMY = 1 << 1,                          // �G���^�[�Q�b�g�Ƃ���
    TARGET_ALL = TARGET_PLAYER | TARGET_ENEMY,      // ���ׂẴ^�[�Q�b�g
    TARGET_DEFAULT = TARGET_ALL                     // �f�t�H���g�ł͂��ׂẴ^�[�Q�b�g���w��
};

class CircleParam
{
    DirectX::XMFLOAT3 center;
    float radius;
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
