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
        float roll_speedY = DirectX::XMConvertToRadians(90.0f);
        float roll_speedX = DirectX::XMConvertToRadians(90.0f);
        float max_angleX = DirectX::XMConvertToRadians(45.0f);
        float min_angleX = DirectX::XMConvertToRadians(45.0f);
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
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // �e��擾�E�ݒ�֐�
    float GetRollSpeed()const { return this->param.roll_speedY; }
    void SetRollSpeed(float speed) { this->param.roll_speedY = speed; }

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

// �f�X�J�����X�V�N���X
class CameraControllerDeathComponent : public Component
{
public:
    struct CameraControllerParam
    {
        float rotation_speed = DirectX::XMConvertToRadians(5.0f);
        float range_move_speed = 1.0f;
    };
public:
    CameraControllerDeathComponent(CameraControllerParam param) : param(param) {};
    ~CameraControllerDeathComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "CameraControllerDeathComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

private:
    CameraControllerParam param;
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
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitive�����݂��邩
#endif // DEBUG
};

#if defined(_DEBUG) || defined(RELEASE_DEBUG)

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
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

private:
    std::weak_ptr<CameraComponent> camera_Wptr;

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

#endif // _DEBUG
