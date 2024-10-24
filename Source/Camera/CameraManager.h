#pragma once
#include <vector>
#include <memory>
#include "Camera/CameraParam.h"
#include "System/ClassBase/Singleton.h"
#include "System/MyMath/MYMATRIX.h"
#ifdef _DEBUG
#include <string>   // �f�o�b�O�̂ݎg�p
#endif // DEBUG

class CameraComponent;

class CameraManager : public Singleton<CameraManager>
{
public:
    CameraManager();
    ~CameraManager();

    // �g�p����J�����̐ݒ�
    void SetCurrentCamera(CAMERA_TYPE type);

    // �J�����̎擾
    std::shared_ptr<CameraComponent> GetCamera(CAMERA_TYPE type);
    // ���݃J�����̎擾
    const std::shared_ptr<CameraComponent>& GetCurrentCamera() { return this->current_camera; }

    // �X�V����
    void Update(float elapsed_time);

    // �J�����^�C�v���ُ�ł��邩
    bool IsErrorType(CAMERA_TYPE type);

    // �J�����̃��Z�b�g
    void Reset();

    // �r���[�v���W�F�N�V�����s����擾
    DirectX::XMFLOAT4X4 GetViewProjection();
    // �r���[�v���W�F�N�V�����s��(MYMATRIX)���擾
    MYMATRIX GetViewProjectionMatrix();
private:
    std::vector<std::shared_ptr<CameraComponent>> camera_pool;  // �J�����ꗗ
    std::shared_ptr<CameraComponent> current_camera;            // ���ݎg�p���Ă���J����

#ifdef _DEBUG
public:
    void DrawDebugGUI();
private:
    std::vector<std::string> camera_name_pool;
    int camera_index = 0;    // ���݂̃J�����C���f�b�N�X
#endif // _DEBUG
};

