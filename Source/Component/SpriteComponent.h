#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Component/BaseSpriteComponent.h"
#include "Sprite/Sprite.h"
#include "System\MyMath\MyMathf.h"

class Transform2DComponent;

class SpriteComponent : public BaseSpriteComponent
{
public:
    struct SpriteParam
    {
        std::string filename = {};
        DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
        DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
        DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
        PRIORITY draw_priority = PRIORITY::DEFAULT;
        Sprite::CENTER_TYPE center_type = Sprite::CENTER_TYPE::TOP_LEFT;
    };
public:
    SpriteComponent(const SpriteParam&);
    virtual ~SpriteComponent() {};

    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override { this->param = this->default_param; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "SpriteComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };
    
    // 描画処理
    void Render(ID3D11DeviceContext* dc) override;

    void SetColor(DirectX::XMFLOAT4 color) { this->param.color = color; };
    void SetAlpha(float a) { this->param.color.w = a; };
    void SetDisplaySize(const DirectX::XMFLOAT2 size) { this->param.display_size = size; };
    void SetDisplaySizeX(const float sizeX) { this->param.display_size.x = sizeX; };
    void SetDisplaySizeY(const float sizeY) { this->param.display_size.y = sizeY; };
    // 表示サイズをスプライトサイズに合わせる
    void AdjustDisplaySizeToSprite();
private:
    std::unique_ptr<Sprite> sprite;
    SpriteParam param;
    SpriteParam default_param;
private:
    std::weak_ptr<Transform2DComponent> transform_Wptr;

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

