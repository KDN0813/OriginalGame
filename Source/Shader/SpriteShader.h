#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include "System\ClassBase\Singleton.h"
#include "Shader/Shader.h"

class BaseSpriteComponent;

class SpriteShader : public Shader, public Singleton<SpriteShader>
{
public:
    SpriteShader();
    ~SpriteShader() {};

    // 描画関数
    void Render() override;
    const char* GetName() { return "SpriteShader"; }

    // スプライトの追加
    void AddSprite(std::shared_ptr<BaseSpriteComponent> sprite);

    void SetShouldSort(bool flag) { this->should_sort = flag; }
private:
    std::vector<std::weak_ptr<BaseSpriteComponent>> sprite_pool;

    Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;

    bool should_sort = false;
#ifdef _DEBUG
public:
    void DrawDebugGUI() override;

#endif // _DEBUG
};

