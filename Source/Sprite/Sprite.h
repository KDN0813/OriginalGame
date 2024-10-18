#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

// ÉXÉvÉâÉCÉg
class Sprite
{
public:
	Sprite() = delete;
	Sprite(const char* filename);
	~Sprite() {}

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
	};

	// ï`âÊé¿çs
	void Render(ID3D11DeviceContext* dc,
		float dx, float dy,	// 0.0fÅ`1.0f
		float dw, float dh,	// 0.0fÅ`1.0f
		float sx, float sy,	// 0.0fÅ`1.0f
		float sw, float sh,	// 0.0fÅ`1.0f
		float angle,
		float r, float g, float b, float a) const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertex_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_state;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;
};