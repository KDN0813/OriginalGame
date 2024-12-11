#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Component.h"
#include "Sprite/Sprite.h"
#include "System\MyMath\MyMathf.h"

class SpriteComponent : public Component
{
public:
    struct SpriteParam
    {
        std::string filename = {};
        DirectX::XMFLOAT2 display_pos = { 0.0f,0.0f };
        DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
        DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
        DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };
        float angle = 0.0f;
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
        Sprite::CENTER_TYPE center_type = Sprite::CENTER_TYPE::TOP_LEFT;
    };
public:
    SpriteComponent(const SpriteParam&);
    virtual ~SpriteComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override { this->param = this->default_param; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "SpriteComponent"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };
    
    // 描画処理
    void Render(ID3D11DeviceContext* dc);

    // 各種取得・設定
    void SetAngle(float angle) { this->param.angle = MyMathf::ClampDegree(angle); };
    float GetAngle() const { return this->param.angle; };

private:
    std::unique_ptr<Sprite> sprite;
    SpriteParam param;
    SpriteParam default_param;

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
    bool IsDebugPrimitive() { return false; }   // DebugPrimitiveが存在するか

private:
    std::vector<std::string> center_type_name_pool;
#endif // DEBUG
};

