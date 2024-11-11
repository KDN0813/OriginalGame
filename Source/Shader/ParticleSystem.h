#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

#include <vector>
#include <memory>

#include "Texture/Texture.h"
#include "shader.h"

class ParticleSystem
{
private:
	struct ParticleParam
	{
		float rot;
	};
	struct Vertex
	{
		DirectX::XMFLOAT3 position;	//	位置
		DirectX::XMFLOAT2 texcoord;	//	UV
		DirectX::XMFLOAT4 color;	//	頂点色		
		ParticleParam param;
	};

	struct ParticleData
	{
		float x, y, z;		// 位置
		float w, h;			// 画像サイズ
		float vx, vy, vz;	// 移動速度
		float ax, ay, az;	// 加速度
		float alpha;		// 透明度
		float timer;		// 生存時間
		float type;
	};
	// シーン定数
	struct SceneConstantsBuffer
	{
		DirectX::XMFLOAT4X4 view_projection;
		DirectX::XMFLOAT4X4 view_matrix;
		DirectX::XMFLOAT4X4 projection_matrix;
	};
	// フレーム毎の定数
	struct ForPerFrameConstantBuffer
	{
		DirectX::XMFLOAT2 size;
		DirectX::XMFLOAT2 dummy;
	};

public:
	ParticleSystem() = delete;
	ParticleSystem(const char* filename, int num = 1000);

	~ParticleSystem();
	void Update(float elapsed_time);

	void Render();

	/**
	 * .
	 * 
	 * \param timer 生存時間
	 * \param p 生成位置
	 * \param v 移動速度
	 * \param f 加速度
	 * \param size サイズ
	 */
	void Set(
		float timer,
		DirectX::XMFLOAT3 p,
		DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT2 size = DirectX::XMFLOAT2(1.0f, 1.0f)
	);

private:
	ParticleData* data;		//	パーティクル情報
	Vertex* v;				//	頂点バッファ書き込み情報
	int num_particles = 0;	//	パーティクル数

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> for_per_frame_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11Buffer> scene_constant_buffer;
	std::unique_ptr<Texture> texture;
};
