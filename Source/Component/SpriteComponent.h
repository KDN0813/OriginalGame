#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Component/BaseSpriteComponent.h"
#include "System\MyMath\MyMathf.h"

class Transform2DComponent;

class SpriteComponent : public BaseSpriteComponent
{
public:
    SpriteComponent(const BaseSpriteComponent::SpriteParam&);
    virtual ~SpriteComponent() {};

    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "SpriteComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };
    
    // 描画処理
    void Render(ID3D11DeviceContext* dc) override;

    void SetDisplaySize(const DirectX::XMFLOAT2 size) { this->sprite_param.display_size = size; };
    void SetDisplaySizeX(const float sizeX) { this->sprite_param.display_size.x = sizeX; };
    void SetDisplaySizeY(const float sizeY) { this->sprite_param.display_size.y = sizeY; };
    // 表示サイズをスプライトサイズに合わせる
    void AdjustDisplaySizeToSprite();
private:
    std::unique_ptr<Sprite> sprite;
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

