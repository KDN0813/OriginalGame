#pragma once
#include "CameraController.h"

// TODO �������폜����
// �f�t�H���g�̃R���g���[���[(�V�K�쐬���̃R�s�[�p)
class DefaultUpdateBehavior : public CameraControllerBase
{
public:
    DefaultUpdateBehavior(OwnerPtr owner) :CameraControllerBase(owner) {}

    // �X�V�֐�
    virtual void Update(float elapsed_time) {};
    // ���O�擾
    virtual const char* GetName() const { return ""; };

#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

// �Q�[���p�b�h�̓��͂���ɃJ�����𐧌�
class GamepadCameraController : public CameraControllerBase
{
public:
    GamepadCameraController(OwnerPtr owner) :CameraControllerBase(owner) {}

    // �X�V�֐�
    virtual void Update(float elapsed_time) {};
    // ���O�擾
    virtual const char* GetName() const { return "GamepadCameraController"; };

#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

