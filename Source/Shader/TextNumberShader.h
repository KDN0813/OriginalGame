#pragma once

#include <wrl.h>
#include <vector>
#include <memory>
#include "Shader\Shader.h"

class TextNumberComponent;

class TextNumberShader : public Shader
{
public:
    TextNumberShader() {};
    ~TextNumberShader() {};

    // 描画関数
    void Render() override;
    const char* GetName() { return "SpriteShader"; }

    // スプライトの追加
    void AddSprite(std::shared_ptr<TextNumberComponent>& sprite);
private:
    std::vector<std::weak_ptr<TextNumberComponent>> sprite_pool;

    Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override;

#endif // _DEBUG
};