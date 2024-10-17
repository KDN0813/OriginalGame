#pragma once
#include "Component/Component.h"

class CameraComponent;
class Transform3DComponent;

// �Q�[���p�b�h�̓��͂���ɃJ�����𐧌�
class GamepadCameraController : public Component
{
public:
    GamepadCameraController(float roll_speed = 90.0f);

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "GamepadCameraController"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };
private:
    float roll_speed;

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
class Transform3DComponent;

// �f�o�b�O�p�J�����R���g���[���[
class DebugCameraController : public Component
{
public:
    DebugCameraController() {}

    // �X�V�֐�
    void Update(float elapsed_time) override;
    // ���O�擾
    const char* GetName() const override { return "DebugUpdateController"; };

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
