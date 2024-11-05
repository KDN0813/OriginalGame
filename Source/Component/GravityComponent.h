#pragma once
#include "Component.h"

class MovementComponent;

class GravityComponent : public Component
{
public:
    struct GravityParam
    {
        float gravity = -0.5f;   // 重力加速度
        bool is_grounded = false;// 地面にいるか
    }; 
public:
    GravityComponent(GravityParam param) : param(param), default_param(param){};

    // リスタート処理
    void ReStart() override { this->param = this->default_param; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time);

    // 名前取得
    const char* GetName()const { return "GravityComponent"; };
    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::VERY_HIGH; };


    // 各種取得・設定関数
    float GetGravity() { return this->param.gravity; }
    bool GetIsGrounded() { return this->param.is_grounded; }
    void SetIsGrounded(bool is_grounded) { this->param.is_grounded = is_grounded; }
private:
    GravityParam param;
    GravityParam default_param;
private:
    std::weak_ptr<MovementComponent> movement_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};