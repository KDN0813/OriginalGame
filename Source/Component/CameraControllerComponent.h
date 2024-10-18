#pragma once
#include "Component/Component.h"

class CameraComponent;
class Transform3DComponent;

// �Q�[���p�b�h�̓��͂���ɃJ�����𐧌�
class CameraControllerGamepad : public Component
{
public:
    struct CameraControllerParam
    {
        float roll_speed = DirectX::XMConvertToRadians(90.0f);
    };

public:
    CameraControllerGamepad(CameraControllerParam param);

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "CameraControllerGamepad"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };

    // �e��擾�E�ݒ�֐�
    float GetRollSpeed()const { return this->param.roll_speed; }
    void SetRollSpeed(float speed) { this->param.roll_speed = speed; }

private:
    CameraControllerParam param;
    CameraControllerParam default_param;

private:
    std::weak_ptr<CameraComponent> camera_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override ;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() { return false; }   // DebugPrimitive�����݂��邩
#endif // _DEBUG
};

#ifdef _DEBUG

// �f�o�b�O�p�J�����R���g���[���[
class CameraControllerDebug : public Component
{
public:
    CameraControllerDebug() {}

    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;
    // ���O�擾
    const char* GetName() const override { return "CameraControllerDebug"; };

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };

private:
    std::weak_ptr<CameraComponent> camera_Wptr;

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
};

#endif // _DEBUG
