#pragma once
#include "Component.h"

class PlayerComponent;
class SpriteComponent;

class SpecialGageComponent : public Component
{
public:
    SpecialGageComponent() {};
    ~SpecialGageComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override { this->flash_timer = 0.0f; };      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "SpecialGageComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

private:
    float CalculateSpecialPointWidth();

private:
    float flash_timer = 0.0f;

private:
    std::weak_ptr<SpriteComponent> sprite_Wptr;
    std::weak_ptr<PlayerComponent> player_Wptr;
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

