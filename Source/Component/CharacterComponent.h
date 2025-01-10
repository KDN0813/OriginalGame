#pragma once
#include "Component.h"

class CharacterComponent : public Component
{
public:
    struct CharacterParam
    {
        int hp = 1;     // 体力
        int max_hp = 1; // 最大体力
    };
public:
    CharacterComponent(CharacterParam param);
    ~CharacterComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "CharacterComponent"; };

    // ダメージを適用する
    void ApplyDamage(int damege);
    // 生存しているか
    bool IsAlive();

    // 各種取得関数
    int GetHP()const { return this->param.hp; }
    int GetMaxHP()const { return this->param.max_hp; }
    // 体力の割合を取得
    float GetHealthPercentage() { return static_cast<float>(this->param.hp) / static_cast<float>(this->param.max_hp); };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };
private:
    CharacterParam param;
    CharacterParam default_param;

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

private:
    int test_damage = 1;    // テストで与えるダメージ量
#endif // DEBUG
};

