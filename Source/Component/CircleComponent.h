#pragma once
#include "Component.h"

// コライダーの種類を定義する列挙型
// オブジェクトが衝突の際にどの役割を果たすかを指定する
enum class COLLISION_TYPE
{
    NONE = 0,           // 当たり判定なし
    ATTACKER = 1 << 0,  // ぶつける側（アクティブに衝突を引き起こすオブジェクト）
    DEFENDER = 1 << 1,  // ぶつけられる側（衝突されるオブジェクト）
    BOTH = 1 << 2,      // 両方に当たり判定がある場合（ぶつける側とぶつけられる側の両方）
    DEFAULT = BOTH,     // デフォルトでは両方に判定を行う
};

// 対象となるターゲットの種類を定義する列挙型
// ATTACKER（攻撃者）および DEFENDER（防御者）で使用し、
// それぞれどのタイプの相手をターゲットとするかを指定する
enum class TARGET_TYPE
{
    NONE = 0,           // ターゲットなし
    PLAYER = 1 << 0,    // プレイヤーをターゲットとする
    ENEMY = 1 << 1,     // 敵をターゲットとする
    ALL = 1 << 2,       // すべてのターゲット
    DEFAULT = ALL       // デフォルトではすべてのターゲットを指定
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

