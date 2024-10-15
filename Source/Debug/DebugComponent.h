#pragma once
#include "Component/Component.h"

// トランスフォーム更新後にトランスフォームを変更する
class DebugComponent : public Component
{
public:
    DebugComponent() {};
    virtual ~DebugComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "DebugComponent"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::LOW; };

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override {};
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override ;
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override;
    bool IsDebugPrimitive() { return true; }   // DebugPrimitiveが存在するか
#endif
};

