#pragma once
#include <vector>
#include <memory>
#include "Camera/CameraParam.h"
#include "System/ClassBase/Singleton.h"
#include "System/MyMath/MYMATRIX.h"
#ifdef _DEBUG
#include <string>   // �f�o�b�O�̂ݎg�p
#endif // DEBUG

class CameraComponent_ver2;

class CameraManager_ver2 : public Singleton<CameraManager_ver2>
{
public:
    CameraManager_ver2();
    ~CameraManager_ver2();

    void SetMainCamera(CAMERA_TYPE type);

    // �X�V����
    void Update(float elapsed_time);

    // �J�����^�C�v���ُ�ł��邩
    bool IsErrorType(CAMERA_TYPE type);

    // �r���[�v���W�F�N�V�����s����擾
    DirectX::XMFLOAT4X4 GetViewProjection();
    // �r���[�v���W�F�N�V�����s��(MYMATRIX)���擾
    MYMATRIX GetViewProjectionMatrix();
private:
    std::vector<std::shared_ptr<CameraComponent_ver2>> camera_pool;  // ���C���J����
    std::shared_ptr<CameraComponent_ver2> main_camera;  // ���C���J����

#ifdef _DEBUG
public:
    void DrawDebugGUI();
private:
    std::vector<std::string> camera_name_pool;
    int camera_index = 0;    // ���݂̃J�����C���f�b�N�X
#endif // _DEBUG
};

