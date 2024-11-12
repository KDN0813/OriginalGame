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
		float rot;							// 回転角度
		DirectX::XMFLOAT2 scale;			// 拡大率
	};

	struct ParticleData
	{
		DirectX::XMFLOAT3 pos;			// 描画位置
		float w, h;						// 画像サイズ
		DirectX::XMFLOAT2 scale;		// 拡大率
		DirectX::XMFLOAT2 f_scale;		// 拡大率(開始)
		DirectX::XMFLOAT2 e_scale;		// 拡大率(終了)
		DirectX::XMFLOAT3 v;			// 移動速度
		DirectX::XMFLOAT3 a;			// 加速度
		float alpha;					// 透明度
		int timer_max;				// 生存時間(最大値)
		int timer;					// 生存時間
		float rot;						// 角度
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
	ParticleSystem(const char* filename, int num = 1);

	~ParticleSystem();
	// コンピュートシェーダーの更新
	void Update();

	void Render();

	/**
	 * .
	 * 
	 * \param timer 生存時間
	 * \param p 生成位置
	 * \param v 移動速度
	 * \param f 加速度
	 * \param rot 角度
	 * \param tx 画像サイズ
	 * \param scale 拡大率
	 */
	void Set(
		int timer,
		DirectX::XMFLOAT3 p,
		DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT2 tx = DirectX::XMFLOAT2(1.0f, 1.0f),
		DirectX::XMFLOAT2 f_scale = DirectX::XMFLOAT2(1.0f, 1.0f),
		DirectX::XMFLOAT2 e_scale = DirectX::XMFLOAT2(1.0f, 1.0f),
		float rot = 0.0f
	);

private:
	ParticleData* datas;		//	パーティクル情報
	int num_particles = 0;	//	パーティクル数

	Microsoft::WRL::ComPtr<ID3D11Buffer> for_per_frame_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> compute_shader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_data_buffer[2] = { NULL,NULL };
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particle_data_bufferUAV[2] = { NULL, NULL }; // アンオーダード アクセス ビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_data_bufferSRV[2] = { NULL, NULL }; // シェーダ リソース ビュー

	Microsoft::WRL::ComPtr<ID3D11Buffer> init_particle_data_buffer = NULL; // 初期化用バッファ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  init_particle_data_bufferSRV = NULL; // 初期化構造体バッファ

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11Buffer> scene_constant_buffer;
	std::unique_ptr<Texture> texture;

	int chainSRV = 0;//バッファーの切り替え
	int chainUAV = 1;//バッファーの切り替え
};
