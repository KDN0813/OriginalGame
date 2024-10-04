#pragma once
#include "CameraController.h"

// TODO 完成時削除する
// デフォルトのコントローラー(新規作成時のコピー用)
class DefaultUpdateController : public CameraControllerBase
{
public:
    DefaultUpdateController(OwnerPtr owner) :CameraControllerBase(owner) {}

    // 更新関数
    void Update(float elapsed_time) {};
    // 名前取得
    const char* GetName() const { return ""; };

#ifdef _DEBUG
public:
    void DrawDebugGUI() {};
#endif // _DEBUG
};

// ゲームパッドの入力を基にカメラを制御
class GamepadCameraController : public CameraControllerBase
{
public:
    GamepadCameraController(OwnerPtr owner) :CameraControllerBase(owner) {}

    // 更新関数
    void Update(float elapsed_time);
    // 名前取得
    const char* GetName() const { return "GamepadCameraController"; };

private:
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float maxAngleX = DirectX::XMConvertToRadians(45);
    float minAngleX = DirectX::XMConvertToRadians(-45);
#ifdef _DEBUG
public:
    void DrawDebugGUI() {};
#endif // _DEBUG
};

#ifdef _DEBUG
class Transform3DComponent;

// デバッグ用カメラコントローラー
class DebugCameraController : public CameraControllerBase
{
public:
    DebugCameraController(OwnerPtr owner) :CameraControllerBase(owner) {}

    // 更新関数
    void Update(float elapsed_time);
    // 名前取得
    const char* GetName() const { return "DebugUpdateController"; };

public:
    void DrawDebugGUI() {};
private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
};

#endif // _DEBUG
