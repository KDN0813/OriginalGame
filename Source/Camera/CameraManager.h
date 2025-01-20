#pragma once
#include <vector>
#include <memory>
#include <DirectXCollision.h>
#include "Camera/CameraParam.h"
#include "System/ClassBase/Singleton.h"
#include "System/MyMath/MYMATRIX.h"
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
#include "Object/Object.h"
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
    const std::shared_ptr<CameraComponent>& GetCurrentCamera();

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
    // ������擾
    DirectX::BoundingFrustum GetBoundingFrustum();

    // �{�b�N�X��1�ł���ʓ��ɂ����truer��Ԃ�
    bool IsAnyMeshAABBVisible(const std::vector<DirectX::BoundingBox>& box_vec);
    // �{�b�N�X����ʓ��ɂ����true��Ԃ�
    bool IsMeshVisible(DirectX::BoundingBox);
private:
    std::vector<std::shared_ptr<CameraComponent>> camera_pool;  // �J�����ꗗ
    std::shared_ptr<CameraComponent> current_camera;            // ���ݎg�p���Ă���J����
    int camera_index = 0;    // ���݂̃J�����C���f�b�N�X

#ifdef _DEBUG
public:
    void DrawDebugGUI();
#endif
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
private:
    void ChegeDebugCamera();
private:
    std::shared_ptr<Object> debug_camera;
    std::vector<std::string> camera_name_pool;
    int debug_camera_index = 0;    // �f�o�b�OImGui�p�̌��݂̃J�����C���f�b�N�X
    int temp_camera_index;   // �f�o�b�O�J�����ɕ�����O�̃J�����̃C���f�b�N�X
#endif // _DEBUG
};

