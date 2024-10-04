#pragma once
#include "CameraController.h"

// TODO 完成時削除する
// デフォルトのコントローラー(新規作成時のコピー用)
class DefaultUpdateBehavior : public CameraControllerBase
{
public:
    DefaultUpdateBehavior(OwnerPtr owner) :CameraControllerBase(owner) {}

    // 更新関数
    virtual void Update(float elapsed_time) {};
    // 名前取得
    virtual const char* GetName() const { return ""; };

#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

// ゲームパッドの入力を基にカメラを制御
class GamepadCameraController : public CameraControllerBase
{
public:
    GamepadCameraController(OwnerPtr owner) :CameraControllerBase(owner) {}

    // 更新関数
    virtual void Update(float elapsed_time) {};
    // 名前取得
    virtual const char* GetName() const { return "GamepadCameraController"; };

#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

