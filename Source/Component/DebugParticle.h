#pragma once
#include "Component.h"
#include <DirectXMath.h>
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG

class Transform3DComponent;

// ParticleSystemを見せるためのデバッグ用コンポーネント
class DebugParticle : public Component
{
public:
    DebugParticle();
    ~ DebugParticle() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "DebugParticle"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };
    
    // 各種設定関数
    // エフェクトループフラグの反転させる
    void ReverseEffectLooping() { this->effect_looping = !this->effect_looping; }

    bool GetEffectLooping() const { return this->effect_looping; }

private:
    // 範囲内のランダムな位置にエフェクトを再生させる
    void PlayEffect(int type);

private:
    float effect_area_radius = 13.0f;    // エフェクトを再生するエリア(円柱)の半径
    float effect_area_height = 5.0f;    // エフェクトを再生するエリア(円柱)の高さ
    DirectX::XMFLOAT3 area_pos = {};    // エフェクトを再生するエリアの中心
    bool effect_looping = false;    // エフェクトをループ再生させるフラグ
    bool is_random_spawn_position = true;	// パーティクルの生成位置をランダムにするかのフラグ
    int draw_effect_type = 1;   // デバッグで描画するエフェクトのタイプ
    int loop_count = 10;   // エフェクトをループ再生する時の1フレームで実行するエフェクト数

private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;

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
    CylinderParam debug_cylinder_effect_area;
#endif // DEBUG
};

