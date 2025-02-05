#pragma once
#include "Component.h"

class PlayerComponent;
class SpriteComponent;

class SpecialGageComponent : public Component
{
public:
    SpecialGageComponent() {};
    ~SpecialGageComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override { this->flash_timer = 0.0f; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "SpecialGageComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

private:
    float CalculateSpecialPointWidth();

private:
    float flash_timer = 0.0f;

private:
    std::weak_ptr<SpriteComponent> sprite_Wptr;
    std::weak_ptr<PlayerComponent> player_Wptr;
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
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitiveが存在するか
#endif // DEBUG
};

