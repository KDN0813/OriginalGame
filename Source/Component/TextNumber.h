#pragma once
#include <string>
#include "Component.h"
#include "Sprite\Sprite.h"

class TextNumber : public Component
{
public:
    struct TextParam
    {
        std::string font_name = {};
        DirectX::XMFLOAT2 pos = { 0.0f,0.0f };
        int value = 0;  // 表示する値
        float font_size = 0.0f; // １文字あたりのサイズ
        float angle = 0.0f;
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
        Sprite::CENTER_TYPE center_type = Sprite::CENTER_TYPE::CENTER;
    };
public:
    TextNumber(TextParam param) : param(param), default_param(param) {};
    ~TextNumber() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override {};

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };

    // 取得・設定関数
    int GetDrawValue()const { return this->param.value; }
    void SetDrawValue(const int v) { this->param.value = v; }

private:
    std::unique_ptr<Sprite> sprite;
    TextParam param;
    TextParam default_param;

    DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
    DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
    DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };

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

