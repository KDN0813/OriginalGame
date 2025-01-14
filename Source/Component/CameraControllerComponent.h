#pragma once
#include "Component/Component.h"

class CameraComponent;
class Transform3DComponent;

// ゲームパッドの入力を基にカメラを制御
class CameraControllerGamepad : public Component
{
public:
    struct CameraControllerParam
    {
        float roll_speedY = DirectX::XMConvertToRadians(90.0f);
        float roll_speedX = DirectX::XMConvertToRadians(90.0f);
        float max_angleX = DirectX::XMConvertToRadians(45.0f);
        float min_angleX = DirectX::XMConvertToRadians(45.0f);
    };

public:
    CameraControllerGamepad(CameraControllerParam param);

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "CameraControllerGamepad"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // 各種取得・設定関数
    float GetRollSpeed()const { return this->param.roll_speedY; }
    void SetRollSpeed(float speed) { this->param.roll_speedY = speed; }

private:
    CameraControllerParam param;
    CameraControllerParam default_param;

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

// デスカメラ更新クラス
class CameraControllerDeathComponent : public Component
{
public:
    struct CameraControllerParam
    {
        float rotation_speed = DirectX::XMConvertToRadians(5.0f);
        float range_move_speed = 1.0f;
    };
public:
    CameraControllerDeathComponent(CameraControllerParam param) : param(param) {};
    ~CameraControllerDeathComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "CameraControllerDeathComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

private:
    CameraControllerParam param;
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
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitiveが存在するか
#endif // DEBUG
};

#if defined(_DEBUG) || defined(RELEASE_DEBUG)

// デバッグ用カメラコントローラー
class CameraControllerDebug : public Component
{
public:
    CameraControllerDebug() {}

    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;
    // 名前取得
    const char* GetName() const override { return "CameraControllerDebug"; };

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

private:
    std::weak_ptr<CameraComponent> camera_Wptr;

#ifdef _DEBUG

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
#endif // DEBUG
};

#endif // _DEBUG
