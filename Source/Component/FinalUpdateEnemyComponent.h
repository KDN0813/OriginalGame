#pragma once
#include "Component.h"

class EnemyComponent;
class PlayerComponent;
class Transform3DComponent;

// �Ō�ɍX�V����A�G�l�~�[�p�̃R���|�[�l���g
class FinalUpdateEnemyComponent : public Component
{
public:
    FinalUpdateEnemyComponent() {};
    ~FinalUpdateEnemyComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "FinalUpdateEnemyComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::LOWEST; };

private:
    std::weak_ptr<EnemyComponent> enemy;
    std::weak_ptr<Transform3DComponent> transform_Wptr;

    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
    std::weak_ptr<PlayerComponent> player_Wptr;
    int collision_range = 1;    // �v���C���[�Ƃ̓����蔻����s���͈�
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
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitive�����݂��邩
#endif // DEBUG
};

