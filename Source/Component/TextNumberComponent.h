#pragma once
#include <string>
#include <vector>
#include "Component/BaseSpriteComponent.h"
#include "Sprite\Sprite.h"

class Transform2DComponent;

class TextNumberComponent : public BaseSpriteComponent
{
public:
    const float NUMBER_COUNT = 10;                  // 数字の数(0～9までの整数)
    const float FONT_WIGTH = 1.0f / NUMBER_COUNT;   // フォントの幅
public:
    struct TextParam
    {
        std::string font_name = {};
        int value = 0;  // 表示する値
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
        PRIORITY draw_priority;
        Sprite::CENTER_TYPE center_type = Sprite::CENTER_TYPE::TOP_LEFT;
    };
public:
    TextNumberComponent(TextParam param) : BaseSpriteComponent(param.draw_priority), param(param), default_param(param) {};
    ~TextNumberComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "TextNumberComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // 描画関数
    void Render(ID3D11DeviceContext* dc) override;

    // 取得・設定関数
    int GetDrawValue()const { return this->param.value; }
    void SetDrawValue(const int v) { this->param.value = (std::max)(v, 0); }

private:
    std::unique_ptr<Sprite> sprite;
    TextParam param;
    TextParam default_param;

    DirectX::XMFLOAT2 font_draw_size = {};  // １文字あたりの描画サイズ
    DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
    DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
    DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };

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
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitiveが存在するか

private:
    std::vector<std::string> center_type_name_pool;
#endif // DEBUG
};

