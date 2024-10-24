#pragma once
#include <vector>
#include <memory>
#include "Camera/CameraParam.h"
#include "System/ClassBase/Singleton.h"
#include "System/MyMath/MYMATRIX.h"
#ifdef _DEBUG
#include <string>   // デバッグのみ使用
#endif // DEBUG

class CameraComponent;

class CameraManager : public Singleton<CameraManager>
{
public:
    CameraManager();
    ~CameraManager();

    // 使用するカメラの設定
    void SetCurrentCamera(CAMERA_TYPE type);

    // カメラの取得
    std::shared_ptr<CameraComponent> GetCamera(CAMERA_TYPE type);
    // 現在カメラの取得
    const std::shared_ptr<CameraComponent>& GetCurrentCamera() { return this->current_camera; }

    // 更新処理
    void Update(float elapsed_time);

    // カメラタイプが異常であるか
    bool IsErrorType(CAMERA_TYPE type);

    // カメラのリセット
    void Reset();

    // ビュープロジェクション行列を取得
    DirectX::XMFLOAT4X4 GetViewProjection();
    // ビュープロジェクション行列(MYMATRIX)を取得
    MYMATRIX GetViewProjectionMatrix();
private:
    std::vector<std::shared_ptr<CameraComponent>> camera_pool;  // カメラ一覧
    std::shared_ptr<CameraComponent> current_camera;            // 現在使用しているカメラ

#ifdef _DEBUG
public:
    void DrawDebugGUI();
private:
    std::vector<std::string> camera_name_pool;
    int camera_index = 0;    // 現在のカメラインデックス
#endif // _DEBUG
};

