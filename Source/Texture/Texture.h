#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class Texture
{
public:
    Texture() = delete;
    Texture(const char* filename);
    ~Texture() {};

    ID3D11ShaderResourceView*const* GetAddressOf()
    {
        return shader_resource_view.GetAddressOf();
    }
    ID3D11ShaderResourceView* const Get()
    {
        return shader_resource_view.Get();
    }

    float GetTextureWidth() const { return this->texture_width; }
    float GetTextureHeight() const { return this->texture_height; }
private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
    float texture_width = 0;        // ダミーテクスチャを使用した場合0のまま
    float texture_height = 0;       // ダミーテクスチャを使用した場合0のまま
};

