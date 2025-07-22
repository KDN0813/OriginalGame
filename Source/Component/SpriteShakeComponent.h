#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <deque>

class BaseSpriteComponent;

// �X�v���C�g�̐U���A�j���[�V�������s��
class SpriteShakeComponent : public Component
{
public:
    struct ShakeCommand
    {
        DirectX::XMFLOAT2 force = {};           // ���͂����V�F�C�N�̋���
        float end_time = 0.0f;                  
    };
public:
    SpriteShakeComponent() : command_pool() {};
    ~SpriteShakeComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "SpriteShakeComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // ���߂�擪�ɂɒǉ�
    void PushFrontCommand(const ShakeCommand& command);
    void PushFrontCommand(DirectX::XMFLOAT2 force, float shake_time);
    // ���߂��Ō���ɒǉ�
    void PushBackCommand(const ShakeCommand& command);
    void PushBackCommand(DirectX::XMFLOAT2 force, float shake_time);

private:
    std::deque<ShakeCommand> command_pool;
    DirectX::XMFLOAT2 shake_movement = {};           //  �V�F�C�N��
    float tiemr = 0.0f;
private:
    std::weak_ptr<BaseSpriteComponent> sprite_Wptr;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitive�����݂��邩
private:
    // �f�o�b�O���ɃV�F�C�N�����邽�߂̒l
    DirectX::XMFLOAT2 debug_force = {};
    float debug_end_time = 0.0f;

#endif // DEBUG
};

