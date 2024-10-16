#pragma once
#include "Component.h"

// �R���C�_�[�̎�ނ��`����񋓌^
// �I�u�W�F�N�g���Փ˂̍ۂɂǂ̖������ʂ��������w�肷��
enum class COLLISION_TYPE
{
    NONE = 0,           // �����蔻��Ȃ�
    ATTACKER = 1 << 0,  // �Ԃ��鑤�i�A�N�e�B�u�ɏՓ˂������N�����I�u�W�F�N�g�j
    DEFENDER = 1 << 1,  // �Ԃ����鑤�i�Փ˂����I�u�W�F�N�g�j
    BOTH = 1 << 2,      // �����ɓ����蔻�肪����ꍇ�i�Ԃ��鑤�ƂԂ����鑤�̗����j
    DEFAULT = BOTH,     // �f�t�H���g�ł͗����ɔ�����s��
};

// �ΏۂƂȂ�^�[�Q�b�g�̎�ނ��`����񋓌^
// ATTACKER�i�U���ҁj����� DEFENDER�i�h��ҁj�Ŏg�p���A
// ���ꂼ��ǂ̃^�C�v�̑�����^�[�Q�b�g�Ƃ��邩���w�肷��
enum class TARGET_TYPE
{
    NONE = 0,           // �^�[�Q�b�g�Ȃ�
    PLAYER = 1 << 0,    // �v���C���[���^�[�Q�b�g�Ƃ���
    ENEMY = 1 << 1,     // �G���^�[�Q�b�g�Ƃ���
    ALL = 1 << 2,       // ���ׂẴ^�[�Q�b�g
    DEFAULT = ALL       // �f�t�H���g�ł͂��ׂẴ^�[�Q�b�g���w��
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

