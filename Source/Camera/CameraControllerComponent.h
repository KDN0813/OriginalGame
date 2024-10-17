#pragma once
#include "Component/Component.h"

class CameraComponent;
class Transform3DComponent;

// ゲームパッドの入力を基にカメラを制御
class GamepadCameraController : public Component
{
public:
    GamepadCameraController(float roll_speed = 90.0f);

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "GamepadCameraController"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };
private:
    float roll_speed;

private:
    std::weak_ptr<CameraComponent> camera_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override ;
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override {};
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() { return false; }   // DebugPrimitiveが存在するか
#endif // _DEBUG
};

#ifdef _DEBUG
class Transform3DComponent;

// デバッグ用カメラコントローラー
class DebugCameraController : public Component
{
public:
    DebugCameraController() {}

    // 更新関数
    void Update(float elapsed_time) override;
    // 名前取得
    const char* GetName() const override { return "DebugUpdateController"; };

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };

private:
    std::weak_ptr<CameraComponent> camera_Wptr;

public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override {};
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override {};
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() { return false; }   // DebugPrimitiveが存在するか
};

#endif // _DEBUG
