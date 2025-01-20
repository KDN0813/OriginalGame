#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

#include <optional>
#include <memory>
#include <vector>

#include "System\ClassBase\Singleton.h"
#include "Texture/Texture.h"
#include "shader.h"

class ParticleSystem : public Singleton<ParticleSystem>
{
public:
	// エフェクトのパラメータ
	// Play関数の引数で使用する
	struct ParticleParam
	{
		DirectX::XMFLOAT3 initial_position;
		DirectX::XMFLOAT3 color;
		float initial_lifetime; // 初期生存時間
		int type;       // エフェクトの種類
		float rot;      // 角度
	};

private:
	// CPUで共有しないデータ
	struct InputGp
	{
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT3 position;// 描画位置
		DirectX::XMFLOAT3 velocity;// 移動速度
		DirectX::XMFLOAT2 scale;   // 拡大率
		float rot;      // 角度
		float alpha;    // 透明度
		float timer;    // 生存時間(最大値は定数で持つ)
		int texture_no; // テクスチャNO
	};

	// CPUで共有するデータ
	struct CPUGPUBuffer
	{
		DirectX::XMFLOAT3 initial_position;	// 初期位置
		DirectX::XMFLOAT3 forward;			// 前方方向
		DirectX::XMFLOAT3 velocity;			// 移動速度
		DirectX::XMFLOAT3 acceleration;		// 加速度
		DirectX::XMFLOAT2 initial_scale;	// 初期拡大率
		DirectX::XMFLOAT2 f_scale;			// 拡大率(補間開始)
		DirectX::XMFLOAT2 e_scale;			// 拡大率(補間終了)
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT3 f_color;
		DirectX::XMFLOAT3 e_color;
		float rot;							// 角度
		float rot_speed;					// 回転速度
		float initial_lifetime;				// 初期生存時間
		int type;							// エフェクトの種類
		int step;
		int is_busy;						// 要素が稼働中であるか
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
		float elapsed_time;					// 経過時間
		float dummy[3];
	};

public:
	ParticleSystem();

	~ParticleSystem();
	// コンピュートシェーダーの更新
	void Update();

	void Render();

	// テクスチャの読み込み
	void LoadTexture(const char* filename);

	// 空いているパーティクルの数を計算する
	int CalculateFreeParticleCount();

	/**
	 * エフェクト再生
	 * 
	 * \param type 生成位置
	 * \param pos 生成位置
	 * \param rot 角度
	 * \param color 色
	 * \param forward 前方方向
	 */
	void PlayEffect(
		int type,
		DirectX::XMFLOAT3 pos,
		float rot,
		DirectX::XMFLOAT3 color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
	);

	// 各種取得・設定関数
	void SetElapsedTime(float time) { this->particle_data.elapsed_time = time; }
	size_t GetFreeParticleCount()const { return this->free_particle_count; }

private:
	/**
	 * エフェクト再生
	 * 
	 * \param particle_pool パーティクルの情報
	 */
	void PlayEffect(
		DirectX::XMFLOAT3 parent_pos,
		float parent_rot,
		DirectX::XMFLOAT3 parent_color,
		DirectX::XMFLOAT3 forward,
		const std::vector<CPUGPUBuffer>& particle_pool
	);
private:
	std::vector<CPUGPUBuffer> particle_data_pool;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> compute_shader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_gpu_data_buffer[2] = { NULL,NULL };				// GPU専用のパーティクル情報を扱うバッファ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_gpu_data_SRV[2] = { NULL, NULL };	// GPU専用のパーティクル情報を扱シェーダ リソース ビュー(読み込み)
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particle_gpu_data_UAV[2] = { NULL, NULL };	// GPU専用のパーティクル情報を扱アンオーダード アクセス ビュー(書き込み)

	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_init_buffer = NULL;			// 初期化バッファ CPUで管理したデータ(CPUGPUBuffer)を元に更新しているバッファ。更新はCSに送る前に毎回している。particle_init_SRVの元になったバッファ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_init_SRV = NULL; // 初期化リソースビュー　particle_init_bufferを元に作られた

	Microsoft::WRL::ComPtr<ID3D11Buffer>  particle_to_cpu_buffer = NULL;			// CPUへの書き込み用バッファ リソース（stagingの手前）particle_to_cpu_UAV作成に使われたバッファ
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>  particle_to_cpu_UAV = NULL;	// アンオーダード アクセス ビュー
	Microsoft::WRL::ComPtr<ID3D11Buffer> staging_buffer;							// GPUでの出力バッファをCPUで扱うためのバッファ

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;
	Microsoft::WRL::ComPtr<ID3D11Buffer> scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_common_constant;
	std::unique_ptr<Texture> texture;

	int chainSRV = 0;//バッファーの切り替え
	int chainUAV = 1;//バッファーの切り替え
	size_t free_particle_count = 0;	// 空いているパーティクルの数

	// エフェクト情報
	std::vector<CPUGPUBuffer> effect_slash;			// 斬撃エフェクトの初期値
	std::vector<CPUGPUBuffer> effect_hit;			// ヒットエフェクト

	// パーティクの定数バッファ
	ParticleCommonConstant particle_data{};
#ifdef _DEBUG
public:
	void DebugDrawGUI();

	void DebugDrawEffectParamGUI(std::string label, std::vector<CPUGPUBuffer>& effect_pool);
#endif // _DEBUG
};
