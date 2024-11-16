#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

#include <optional>
#include <memory>
#include <vector>

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

	// CPUで共有しないデータ
	struct InputGp
	{
		DirectX::XMFLOAT3 position;// 描画位置
		DirectX::XMFLOAT2 scale;   // 拡大率
		float alpha;    // 透明度
		float timer;    // 生存時間(最大値は定数で持つ)
	};

	// CPUで共有するデータ
	struct CPUGPUBuffer
	{
		DirectX::XMFLOAT3 position;
		float rot;      // 角度
		int step;
		int is_busy;    // 要素が稼働中であるか
	};

	// シーン定数
	struct SceneConstantsBuffer
	{
		DirectX::XMFLOAT4X4 view_projection;
		DirectX::XMFLOAT4X4 view_matrix;
		DirectX::XMFLOAT4X4 projection_matrix;
	};
	// パーティク共通の定数
	struct ParticleCommonConstant
	{
		DirectX::XMFLOAT2 default_size;		// 画像サイズ
		DirectX::XMFLOAT2 f_scale;			// 拡大率(開始)
		DirectX::XMFLOAT2 e_scale;			// 拡大率(終了)
		float timer_max;					// 生存時間
		float dummy;
	};

public:
	ParticleSystem() = delete;
	ParticleSystem(const char* filename);

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
		DirectX::XMFLOAT3 p,
		float rot = 0.0f
	);

private:
	std::vector<CPUGPUBuffer> particle_data_pool;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> compute_shader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_data_buffer[2] = { NULL,NULL };
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_data_SRV[2] = { NULL, NULL }; // シェーダ リソース ビュー(読み込み)
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particle_data_UAV[2] = { NULL, NULL }; // アンオーダード アクセス ビュー(書き込み)

	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_init_buffer = NULL;			// 初期化バッファ CPUで管理したデータ(CPUGPUBuffer)を元に更新しているバッファ。更新はCSに送る前に毎回している。particle_init_SRVの元になったバッファ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_init_SRV = NULL; // 初期化リソースビュー　particle_init_bufferを元に作られた

	Microsoft::WRL::ComPtr<ID3D11Buffer>  particle_to_cpu_buffer = NULL;			// CPUへの書き込み用バッファ リソース（stagingの手前）particle_to_cpu_UAV作成に使われたバッファ
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>  particle_to_cpu_UAV = NULL;	// アンオーダード アクセス ビュー
	Microsoft::WRL::ComPtr<ID3D11Buffer> staging_buffer;							// GPUでの出力バッファをCPUで扱うためのバッファ

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11Buffer> scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_common_constant;
	std::unique_ptr<Texture> texture;

	int chainSRV = 0;//バッファーの切り替え
	int chainUAV = 1;//バッファーの切り替え
};
