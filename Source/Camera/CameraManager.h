#pragma once
#include "System/ClassBase/Singleton.h"
#include <vector>

class CameraComponent;

class CameraManager : public Singleton<CameraManager>
{
public:
    CameraManager() :Singleton(this) {};

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
private:
    std::vector<CameraComponent*> camera_vector;
    CameraComponent* main_camera = nullptr; // ���C���J����
#ifdef _DEBUG
public:
    void DrawDebugGUI() {}

#endif // _DEBUG
};
