#pragma once
#include "CameraController.h"

// TODO �������폜����
// �f�t�H���g�̃R���g���[���[(�V�K�쐬���̃R�s�[�p)
class DefaultUpdateController : public CameraControllerBase
{
public:
    DefaultUpdateController(OwnerPtr owner) :CameraControllerBase(owner) {}

    // �X�V�֐�
    void Update(float elapsed_time) {};
    // ���O�擾
    const char* GetName() const { return ""; };

#ifdef _DEBUG
public:
    void DrawDebugGUI() {};
#endif // _DEBUG
};

// �Q�[���p�b�h�̓��͂���ɃJ�����𐧌�
class GamepadCameraController : public CameraControllerBase
{
public:
    GamepadCameraController(OwnerPtr owner) :CameraControllerBase(owner) {}

    // �X�V�֐�
    void Update(float elapsed_time);
    // ���O�擾
    const char* GetName() const { return "GamepadCameraController"; };

private:
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float maxAngleX = DirectX::XMConvertToRadians(45);
    float minAngleX = DirectX::XMConvertToRadians(-45);
#ifdef _DEBUG
public:
    void DrawDebugGUI() {};
#endif // _DEBUG
};

#ifdef _DEBUG
class Transform3DComponent;

// �f�o�b�O�p�J�����R���g���[���[
class DebugCameraController : public CameraControllerBase
{
public:
    DebugCameraController(OwnerPtr owner) :CameraControllerBase(owner) {}

    // �X�V�֐�
    void Update(float elapsed_time);
    // ���O�擾
    const char* GetName() const { return "DebugUpdateController"; };

public:
    void DrawDebugGUI() {};
private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
};

#endif // _DEBUG
