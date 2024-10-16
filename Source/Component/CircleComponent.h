#pragma once
#include "Component.h"

// コライダーの種類を定義する列挙型
// オブジェクトが衝突の際にどの役割を果たすかを指定する
enum class COLLISION_TYPE
{
    NONE = 0,                       // 当たり判定なし
    ATTACKER = 1 << 0,              // ぶつける側（アクティブに衝突を引き起こすオブジェクト）
    DEFENDER = 1 << 1,              // ぶつけられる側（衝突されるオブジェクト）
    BOTH = ATTACKER | DEFENDER,     // 両方に当たり判定がある場合（ぶつける側とぶつけられる側の両方）
    DEFAULT = BOTH,                 // デフォルトでは両方に判定を行う
};

// 対象となるターゲットの種類を定義する列挙型
// ATTACKER（攻撃者）および DEFENDER（防御者）で使用し、
// それぞれどのタイプの相手をターゲットとするかを指定する
enum class TARGET_TYPE
{
    NONE = 0,                   // ターゲットなし
    PLAYER = 1 << 0,            // プレイヤーをターゲットとする
    ENEMY = 1 << 1,             // 敵をターゲットとする
    ALL = PLAYER | ENEMY,       // すべてのターゲット
    DEFAULT = ALL               // デフォルトではすべてのターゲットを指定
};

// 円の当たり判定用コンポーネント
class CircleComponent : public Component
{
public:
    CircleComponent() {};
    virtual ~CircleComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override;
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "CircleComponent"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // 各種取得・設定関数
    COLLISION_TYPE GetCollisionType() { return this->collision_type; }
    TARGET_TYPE GetTargetType() { return this->target_type; }
    void SetCollisionType(COLLISION_TYPE type) { this->collision_type = type; }
    void SetTargetType(TARGET_TYPE type) { this->target_type = type; }

private:
    COLLISION_TYPE collision_type = COLLISION_TYPE::DEFAULT;
    TARGET_TYPE target_type = TARGET_TYPE::DEFAULT;

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

