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
private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
};

