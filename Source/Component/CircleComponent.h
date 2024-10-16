#pragma once
#include "Component.h"

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

// �~�̓����蔻��p�R���|�[�l���g
class CircleComponent : public Component
{
public:
    CircleComponent() {};
    virtual ~CircleComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override;
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "CircleComponent"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // �e��擾�E�ݒ�֐�
    CollisionType GetCollisionType() { return this->collision_type; }
    TargetType GetTargetType() { return this->target_type; }
    void SetCollisionType(COLLISION_TYPE type) { this->collision_type = type; }
    void SetTargetType(TARGET_TYPE type) { this->target_type = type; }

private:
    CollisionType collision_type = COLLISION_TYPE::COLLISION_TYPE_DEFAULT;
    TargetType target_type = TARGET_TYPE::TARGET_DEFAULT;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override {};
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() { return false; }   // DebugPrimitive�����݂��邩
#endif // DEBUG
};

