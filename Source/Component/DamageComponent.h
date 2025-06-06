#pragma once
#include "Component.h"

class DamageComponent : public Component
{
public:
    struct DamageParam
    {
        int damage_amount = 1;          // ダメージ量
        float invincible_time = 0.0f;   // 無敵時間
        int get_point_rate = 1;         // 取得ポイント倍率
    };
public:
    DamageComponent(DamageParam param) : param(param) {};
    ~DamageComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "DamageComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    int GetDamageAmount()const { return this->param.damage_amount; }
    int GetPointRate()const { return this->param.get_point_rate; }
    float GetInvincibleTime()const { return this->param.invincible_time; }
private:
    DamageParam param;

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

