#pragma once

#include <string>
#include "Component\Component.h"

class EffekseerComponent : public Component
{
public:
    struct EffectParam
    {
        std::string filename;               // ファイル名
        std::string key;                    // エフェクトを管理する時のキー
        bool follow_position = true;         // エフェクトをオブジェクトの位置に追従させるかどうか
        bool align_rotation = false;         // エフェクトをオブジェクトの向きに回転させるかどうか
    };
public:
    EffekseerComponent() {};
    ~EffekseerComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "EffekseerComponent"; };
    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    //void PlayEffect(std::string key);

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