#pragma once
#include "Component.h"

class ModelPartAnimationControlComponent;
class InstancedModelWithAnimationComponent;

// デバッグ用のキャラ操作コンポーネント
class DebugCharacterComponent : public Component
{
public:
    DebugCharacterComponent() {};
    ~DebugCharacterComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "DebugCharacterComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void SetBlendTime(const float time) { this->blend_time = time; }
private:
    float blend_time = 0.3f;

private:
    std::weak_ptr<ModelPartAnimationControlComponent> animation_Wptr;

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

class DebugCharacterComponent_I : public Component
{
public:
    DebugCharacterComponent_I() {};
    ~DebugCharacterComponent_I() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "DebugCharacterComponent_I"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void SetBlendTime(const float time) { this->blend_time = time; }
private:
    float blend_time = 0.3f;

private:
    std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wptr;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override;
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

