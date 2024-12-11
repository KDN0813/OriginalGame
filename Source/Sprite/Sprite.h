#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include "Texture/Texture.h"

// スプライト
class Sprite
{
public:
	// 中心位置の設定
	enum class CENTER_TYPE
	{
		TOP_LEFT = 0,
		TOP_RIGHT,
		CENTER,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,

		MAX,
	};

	// 中心位置のオフセット値取得
	static void GetCenterTypeRate(float& rateX, float& rateY, CENTER_TYPE type);

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

	// 描画実行
	void Render(ID3D11DeviceContext* dc,
		float dx, float dy,	// 0.0f～1.0f
		float dw, float dh,	// 0.0f～1.0f
		float sx, float sy,	// 0.0f～1.0f
		float sw, float sh,	// 0.0f～1.0f
		float angle,
		float r, float g, float b, float a,
		CENTER_TYPE center_type
	) const;
	// 描画実行
	void Render(ID3D11DeviceContext* dc,
		DirectX::XMFLOAT2 display_pos,
		DirectX::XMFLOAT2 display_size,
		DirectX::XMFLOAT2 clip_pos,
		DirectX::XMFLOAT2 clip_size,
		float angle,
		DirectX::XMFLOAT4 color,
		CENTER_TYPE center_type
		) const;

	float GetTextureWidth() { return (this->texture) ? this->texture->GetTextureWidth() : 0.0f; }
	float GetTextureHeight() { return (this->texture) ? this->texture->GetTextureHeight() : 0.0f; }
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertex_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_state;
	std::unique_ptr<Texture>							texture;
};