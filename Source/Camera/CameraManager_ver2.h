#pragma once
#include <vector>
#include <memory>
#include "Camera/CameraParam.h"
#include "System/ClassBase/Singleton.h"
#include "System/MyMath/MYMATRIX.h"
#ifdef _DEBUG
#include <string>   // デバッグのみ使用
#endif // DEBUG

class CameraComponent_ver2;

class CameraManager_ver2 : public Singleton<CameraManager_ver2>
{
public:
    CameraManager_ver2();
    ~CameraManager_ver2();

    void SetMainCamera(CAMERA_TYPE type);

    // 更新処理
    void Update(float elapsed_time);

    // カメラタイプが異常であるか
    bool IsErrorType(CAMERA_TYPE type);

    // ビュープロジェクション行列を取得
    DirectX::XMFLOAT4X4 GetViewProjection();
    // ビュープロジェクション行列(MYMATRIX)を取得
    MYMATRIX GetViewProjectionMatrix();
private:
    std::vector<std::shared_ptr<CameraComponent_ver2>> camera_pool;  // メインカメラ
    std::shared_ptr<CameraComponent_ver2> main_camera;  // メインカメラ

#ifdef _DEBUG
public:
    void DrawDebugGUI();
private:
    std::vector<std::string> camera_name_pool;
    int camera_index = 0;    // 現在のカメラインデックス
#endif // _DEBUG
};

