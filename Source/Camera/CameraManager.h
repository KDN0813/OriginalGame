#pragma once
#include "System/ClassBase/Singleton.h"
#include <vector>

class CameraComponent;

class CameraManager : public Singleton<CameraManager>
{
public:
    CameraManager();
    ~CameraManager();

    CameraComponent* GetMainCamera() { return this->main_camera; }

    /**
     * \fn AddCamera
     * \brief �J�����̒ǉ�
     * 
     * \param camera �ǉ�����CameraComponent�̃|�C���^
     */
    void AddCamera(CameraComponent* camera);
    /**
     * \fn AddCamera
     * \brief �J�����̍폜
     *
     * \param camera �ǉ�����CameraComponent�̃|�C���^
     */
    void RemoveCamera(CameraComponent* camera);
    /**
     * \fn SetMainCamera
     * \brief ���C���J�����̐ݒ�
     * 
     * \param camera �ݒ肷��CameraComponent�̃|�C���^
     */
    void SetMainCamera(CameraComponent* camera);

    // �X�V����
    void Update(float elapsed_time);
private:
    std::vector<CameraComponent*> camera_vector;
    CameraComponent* main_camera = nullptr; // ���C���J����
#ifdef _DEBUG
public:
    void DrawDebugGUI();
    void CreateDebugCamera();

    CameraComponent* debug_camera = nullptr;    // �f�o�b�O�p�J����
    CameraComponent* temp_camera = nullptr;     // ���C���J�������ꎞ�ۑ�����
    bool debug_flag = false;;
#endif // _DEBUG
};

