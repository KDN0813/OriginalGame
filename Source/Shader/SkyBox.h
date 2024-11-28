#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "Shader\Shader.h"

class SpriteComponent;

class SkyBox : public Shader
{
public:
	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4 view_projection;
		DirectX::XMFLOAT4 camera_position;
		DirectX::XMFLOAT4X4 inverse_view_projection;
	};
public:
	SkyBox();
	virtual ~SkyBox() {}

	void Render();

	const char* GetName() { return "SkyBox"; };

	void SetTexture(std::shared_ptr<SpriteComponent> texture) { this->texture_Wptr = texture; }
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

	std::weak_ptr<SpriteComponent>					texture_Wptr;
#ifdef _DEBUG
public:
	void DrawDebugGUI();

#endif // _DEBUG
};

