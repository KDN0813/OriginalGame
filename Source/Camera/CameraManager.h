#pragma once
#include <vector>
#include <memory>
#include <DirectXCollision.h>
#include "Camera/CameraParam.h"
#include "System/ClassBase/Singleton.h"
#include "System/MyMath/MYMATRIX.h"
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
#include "Object/Object.h"
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
    const std::shared_ptr<CameraComponent>& GetCurrentCamera();

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
    // 視錐台取得
    DirectX::BoundingFrustum GetBoundingFrustum();

    // ボックスが1つでも画面内にあればtruerを返す
    bool IsAnyMeshAABBVisible(const std::vector<DirectX::BoundingBox>& box_vec);
    // ボックスが画面内にあればtrueを返す
    bool IsMeshVisible(DirectX::BoundingBox);
private:
    std::vector<std::shared_ptr<CameraComponent>> camera_pool;  // カメラ一覧
    std::shared_ptr<CameraComponent> current_camera;            // 現在使用しているカメラ
    int camera_index = 0;    // 現在のカメラインデックス

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
    int debug_camera_index = 0;    // デバッグImGui用の現在のカメラインデックス
    int temp_camera_index;   // デバッグカメラに分かる前のカメラのインデックス
#endif // _DEBUG
};

