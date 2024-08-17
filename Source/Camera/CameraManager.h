#pragma once
#include "System/ClassBase/Singleton.h"
#include <vector>

class CameraComponent;

class CameraManager : public Singleton<CameraManager>
{
public:
    CameraManager() : Singleton(this) {};
    ~CameraManager() {}

    CameraComponent* GetCameraComponent() { return this->main_camera; }

    /**
     * \fn AddCamera
     * \brief カメラの追加
     * 
     * \param camera 追加するCameraComponentのポインタ
     */
    void AddCamera(CameraComponent* camera);
    /**
     * \fn AddCamera
     * \brief カメラの削除
     *
     * \param camera 追加するCameraComponentのポインタ
     */
    void RemoveCamera(CameraComponent* camera);
    /**
     * \fn SetMainCamera
     * \brief メインカメラの設定
     * 
     * \param camera 設定するCameraComponentのポインタ
     */
    void SetMainCamera(CameraComponent* camera);
private:
    std::vector<CameraComponent*> camera_vector;
    CameraComponent* main_camera = nullptr; // メインカメラ
#ifdef _DEBUG
public:
    void DrawDebugGUI() {}

#endif // _DEBUG
};

