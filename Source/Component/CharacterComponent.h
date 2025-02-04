#pragma once
#include "Component.h"

#ifdef _DEBUG
#include "Debug\DebugPrimitiveRenderer.h"
class Transform3DComponent;
#endif // _DEBUG

class CharacterComponent : public Component
{
public:
    struct CharacterParam
    {
        int hp = 1;     // 体力
        int old_hp = 1; // 前フレームの体力
        int max_hp = 1; // 最大体力
        float invincible_timer = 0.0f;      // 無敵時間
        bool invincible_flag = false;       // 無敵フラグ(永続的に無敵状態にする際に使用する)
        float radius = 0.5f;                // キャラクターの半径
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
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "CharacterComponent"; };

    // ダメージを適用する
    void ApplyDamage(int damage, float invincible_timer);
    // 生存しているか
    bool IsAlive();
    // ダメージを受けたか
    bool IsDamage();

    // 無敵状態であるか
    bool IsInvincible();

    // 各種取得・設定関数
    int GetHP()const { return this->param.hp; }
    int GetMaxHP()const { return this->param.max_hp; }
    float GetRadius()const { return this->param.radius; }
    // 体力の割合を取得
    float GetHealthPercentage() { return static_cast<float>(this->param.hp) / static_cast<float>(this->param.max_hp); };
    bool GetInvincibleFlag()const { return this->param.invincible_flag; }
    void SetInvincibleFlag(const bool flag) { this->param.invincible_flag = flag; }

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::LOWEST; };
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
    void DrawDebugPrimitive()  override;
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override;
    bool IsDebugPrimitive() override { return true; }   // DebugPrimitiveが存在するか

private:
    int test_damage = 1;    // テストで与えるダメージ量
    CylinderParam character_radius{};
    std::weak_ptr<Transform3DComponent> transform_Wptr;
#endif // DEBUG
};

