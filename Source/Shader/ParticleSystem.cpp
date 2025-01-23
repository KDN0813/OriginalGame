#include "ParticleSystem.h"
#include "../Hlsl/ParticleDisp.h"
#include "Shader/ShaderLoader.h"
#include "Graphics/Graphics.h"
#include "Camera/CameraManager.h"
#include "System/Misc.h"
#include "System\Misc.h"
#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG

#include "Component/CameraComponent.h"

ParticleSystem::ParticleSystem()
	: Singleton(this)
	, particle_data_pool()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11Device* device = graphics->GetDevice();

	HRESULT hr;

	// 入力用パーティクル情報の初期設定
	CPUGPUBuffer initial_value{};	// 初期化用の値設定
	particle_data_pool.resize(PERTICLES_PIECE_NO, initial_value);

	//	定数バッファ生成
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		{
			// パーティクルデータの初期化
			particle_data.elapsed_time = 0.0f;

			D3D11_SUBRESOURCE_DATA subresource{};
			subresource.pSysMem = &particle_data;
			buffer_desc.ByteWidth = sizeof(ParticleCommonConstant);
			hr = device->CreateBuffer(&buffer_desc, &subresource, this->particle_common_constant.GetAddressOf());
			assert(SUCCEEDED(hr));
		}
		{
			buffer_desc.ByteWidth = sizeof(SceneConstantsBuffer);
			hr = device->CreateBuffer(&buffer_desc, nullptr, this->scene_constant_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}
	}

	// 頂点シェーダー
	hr = CreateShader::VsFromCso(device, "Shader\\GeometryParticle_vs.cso", this->vertex_shader.ReleaseAndGetAddressOf(), nullptr, nullptr, 0);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	ジオメトリシェーダー
	hr = CreateShader::GsFromCso(device, "Shader\\GeometryParticle_gs.cso", this->geometry_shader.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	ピクセルシェーダー
	hr = CreateShader::PsFromCso(device, "Shader\\GeometryParticle_ps.cso", this->pixel_shader.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// コンピュートシェーダー
	hr = CreateShader::CsFromCso(device, "Shader\\GeometryParticle_cs.cso", this->compute_shader.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ブレンドステート作成
	{
		D3D11_BLEND_DESC blend_desc{};
		blend_desc.AlphaToCoverageEnable = FALSE;
		blend_desc.IndependentBlendEnable = FALSE;
		blend_desc.RenderTarget[0].BlendEnable = TRUE;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blend_desc, this->blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステート作成
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, this->depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ラスタライザステート
	{
		D3D11_RASTERIZER_DESC rasterizer_desc{};
		rasterizer_desc.FrontCounterClockwise = TRUE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_desc.AntialiasedLineEnable = TRUE;
		hr = device->CreateRasterizerState(&rasterizer_desc, this->rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// サンプラステートの生成
	{
		D3D11_SAMPLER_DESC sampler_desc{};
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = device->CreateSamplerState(&sampler_desc, this->sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// GPU専用のパーティクルデータを扱うバッファの作成
	{
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(InputGp); // バッファ サイズ
		Desc.Usage = D3D11_USAGE_DEFAULT;//ステージの入出力はOK。GPUの入出力OK。
		Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 構造化バッファ
		Desc.StructureByteStride = sizeof(InputGp);

		InputGp* input_gp = new InputGp[PERTICLES_PIECE_NO];
		for (int i = 0; i < PERTICLES_PIECE_NO;++i)
		{
			input_gp[i].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			input_gp[i].scale = DirectX::XMFLOAT2(0.0f, 0.0f);
			input_gp[i].alpha = 0.0f;
			input_gp[i].timer = 0;
		}

		D3D11_SUBRESOURCE_DATA SubResource{};//サブリソースの初期化用データを定義
		SubResource.pSysMem = input_gp;
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// 最初の入力リソース(データを初期化する)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_gpu_data_buffer[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// 最初の出力リソース（初期化用データは必要ない）
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_gpu_data_buffer[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		delete[] input_gp;
	}

	// GPU専用のパーティクルデータを扱う入力用リソースビューの作成
	{
		// 入力ワークリソース ビューの作成
		D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
		ZeroMemory(&DescSRV, sizeof(DescSRV));
		DescSRV.Format = DXGI_FORMAT_UNKNOWN;
		DescSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

		DescSRV.Buffer.ElementWidth = PERTICLES_PIECE_NO; // データ数

		// シェーダ リソース ビューの作成
		hr = device->CreateShaderResourceView(this->particle_gpu_data_buffer[0].Get(), &DescSRV, this->particle_gpu_data_SRV[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		hr = device->CreateShaderResourceView(this->particle_gpu_data_buffer[1].Get(), &DescSRV, this->particle_gpu_data_SRV[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// GPU専用のパーティクルデータを扱う出力用リソースビューの作成
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
		ZeroMemory(&DescUAV, sizeof(DescUAV));
		DescUAV.Format = DXGI_FORMAT_UNKNOWN;
		DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		DescUAV.Buffer.NumElements = PERTICLES_PIECE_NO; // データ数

		// アンオーダード・アクセス・ビューの作成
		hr = device->CreateUnorderedAccessView(this->particle_gpu_data_buffer[0].Get(), &DescUAV, this->particle_gpu_data_UAV[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		hr = device->CreateUnorderedAccessView(this->particle_gpu_data_buffer[1].Get(), &DescUAV, this->particle_gpu_data_UAV[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 初期化用バッファの作成
	{
		// リソースの設定
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(CPUGPUBuffer); // バッファ サイズ
		Desc.Usage = D3D11_USAGE_DYNAMIC;//ステージの入出力はOK。GPUの入出力OK。
		Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//UNORDEREDのダイナミックはダメだった。
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 構造化バッファ
		Desc.StructureByteStride = sizeof(CPUGPUBuffer);
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // CPUから書き込む
		
		D3D11_SUBRESOURCE_DATA SubResource{};//サブリソースの初期化用データを定義
		SubResource.pSysMem = particle_data_pool.data();
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// 最初の入力リソース(データを初期化する)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_init_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 初期化用入力用リソースビューの作成
	{
		// 入力ワークリソース ビューの設定（入力用）
		D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
		ZeroMemory(&DescSRV, sizeof(DescSRV));
		DescSRV.Format = DXGI_FORMAT_UNKNOWN;
		DescSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

		DescSRV.Buffer.ElementWidth = PERTICLES_PIECE_NO; // データ数

		// シェーダ リソース ビューの作成
		hr = device->CreateShaderResourceView(this->particle_init_buffer.Get(), &DescSRV, this->particle_init_SRV.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ステージングバッファ(GPU出力データをCPUで扱うためのバッファ)
	{
		// リードバック用バッファ リソースの作成
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(CPUGPUBuffer);	// バッファ サイズ
		Desc.Usage = D3D11_USAGE_STAGING;  // CPUから読み書き可能なリソース
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPUから読み込む
		Desc.StructureByteStride = sizeof(CPUGPUBuffer);//コンピュートシェーダーで構造体を扱う場合必要
		
		// ステージングバッファの作成
		hr = device->CreateBuffer(&Desc, NULL, this->staging_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// CPUへの書き込み用バッファの作成
	{
		// リソースの設定
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(CPUGPUBuffer); // バッファ サイズ
		Desc.Usage = D3D11_USAGE_DEFAULT;//ステージの入出力はOK。GPUの入出力OK。
		Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 構造化バッファ
		Desc.StructureByteStride = sizeof(CPUGPUBuffer);

		D3D11_SUBRESOURCE_DATA SubResource{};//初期化サブリソースの初期化用データを定義
		SubResource.pSysMem = this->particle_data_pool.data();
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// 最初の入力リソース(データを初期化する)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_to_cpu_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// CPUへの書き込み用リソースビューの作成
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
		ZeroMemory(&DescUAV, sizeof(DescUAV));
		DescUAV.Format = DXGI_FORMAT_UNKNOWN;
		DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		DescUAV.Buffer.NumElements = PERTICLES_PIECE_NO; // データ数

		// アンオーダード・アクセス・ビューの作成
		hr = device->CreateUnorderedAccessView(this->particle_to_cpu_buffer.Get(), &DescUAV, this->particle_to_cpu_UAV.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// エフェクト作成
	{
		// 斬撃エフェクト
		{
			CPUGPUBuffer effect{};
			effect.initial_position = {};						// 初期位置
			effect.forward = {};								// 移動方向
			effect.velocity = {};								// 移動速度
			effect.acceleration = {};							// 加速度
			effect.initial_scale = {};							// 初期拡大率
			effect.f_scale = DirectX::XMFLOAT2(0.02f, 0.01f);		// 拡大率(補間開始)
			effect.e_scale = DirectX::XMFLOAT2(0.01f, 0.035f);		// 拡大率(補間終了)
			effect.color = DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f);	// 色
			effect.rot = {};									// 角度
			effect.rot_speed = {};								// 回転速度
			effect.initial_lifetime = 0.8f;						// 生存時間
			effect.type = EFFECT_SLASH;							// エフェクトタイプ
			effect.step = 0;									// step
			effect.is_busy = 1;									// 稼働フラグ

			this->effect_slash.emplace_back(effect);
		}

		// ヒットエフェクト
		{
			DirectX::XMFLOAT3 pos{};
			float rot{};
			const size_t particle_max = 10;	// ヒットエフェクトの星の数
			const float step = (-DirectX::XM_PI - DirectX::XM_PI) / static_cast<float>(particle_max);	// 各区間の間隔
			const float moveXZ = 1.7f;	// 移動量(仮)
			const float moveY = 4.0f;	// 移動量(仮)
			const float initial_lifetime = 0.8f;
			const float velocityXZ = moveXZ / initial_lifetime;
			const float velocityY = moveY / initial_lifetime;
			const float accelerationY = -velocityY / initial_lifetime * 1.5f;
			for (size_t i = 0; i < particle_max; ++i)
			{
				CPUGPUBuffer effect{};
				effect.initial_position = pos;						// 初期位置
				effect.forward = {};								// 前方方向
				// 移動速度
				effect.velocity = 
				{
					sinf(step* i)* velocityXZ,
					velocityY,
					cosf(step* i)* velocityXZ
				};								
				effect.acceleration = { 0.0f ,accelerationY ,0.0f };   // 加速度
				effect.initial_scale = DirectX::XMFLOAT2(0.03f, 0.03f);// 初期拡大率
				effect.f_scale = {};		// 拡大率(補間開始)
				effect.e_scale = {};		// 拡大率(補間終了)
				effect.color = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);	// 色
				effect.f_color = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);	// 色
				effect.e_color = DirectX::XMFLOAT3(0.74f, 0.32f, 0.0f);	// 色
				effect.rot = rot;									// 角度
				effect.rot_speed = DirectX::XMConvertToRadians(360.0f);								// 回転速度
				effect.initial_lifetime = initial_lifetime;			// 生存時間
				effect.type = EFFECT_HIT;							// エフェクトタイプ
				effect.step = 0;									// step
				effect.is_busy = 1;									// 稼働フラグ
				this->effect_hit.emplace_back(effect);
			}
		}

		// ヒットエフェクト2
		{
			DirectX::XMFLOAT3 pos{};
			float rot{};
			const size_t particle_max = 10;	// ヒットエフェクトの星の数
			const float step = (-DirectX::XM_PI - DirectX::XM_PI) / static_cast<float>(particle_max);	// 各区間の間隔
			const float moveXZ = 1.7f;	// 移動量(仮)
			const float moveY = 4.0f;	// 移動量(仮)
			const float initial_lifetime = 0.8f;
			const float velocityXZ = moveXZ / initial_lifetime;
			const float velocityY = moveY / initial_lifetime;
			const float accelerationY = -velocityY / initial_lifetime * 1.5f;
			for (size_t i = 0; i < particle_max; ++i)
			{
				CPUGPUBuffer effect{};
				effect.initial_position = pos;						// 初期位置
				effect.forward = {};								// 前方方向
				// 移動速度
				effect.velocity =
				{
					sinf(step * i) * velocityXZ,
					velocityY,
					cosf(step * i) * velocityXZ
				};
				effect.acceleration = { 0.0f ,accelerationY ,0.0f };   // 加速度
				effect.initial_scale = DirectX::XMFLOAT2(0.005f, 0.005f);// 初期拡大率
				effect.f_scale = {};		// 拡大率(補間開始)
				effect.e_scale = {};		// 拡大率(補間終了)
				effect.color = DirectX::XMFLOAT3(0.36f, 0.9f, 1.0f);	// 色
				effect.f_color = DirectX::XMFLOAT3(0.36f, 0.9f, 1.0f);	// 色
				effect.e_color = DirectX::XMFLOAT3(0.25f, 0.61f, 0.7f);	// 色
				effect.rot = rot;									// 角度
				effect.rot_speed = DirectX::XMConvertToRadians(360.0f);								// 回転速度
				effect.initial_lifetime = initial_lifetime;			// 生存時間
				effect.type = EFFECT_HIT2;							// エフェクトタイプ
				effect.step = 0;									// step
				effect.is_busy = 1;									// 稼働フラグ
				this->effect_hit2.emplace_back(effect);
			}
		}
	}
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11DeviceContext* immediate_context = graphics->GetDeviceContext();

	immediate_context->CSSetShader(this->compute_shader.Get(), 0, 0);

	// 定数バッファの設定
	{
		immediate_context->UpdateSubresource(this->particle_common_constant.Get(), 0, nullptr, &this->particle_data, 0, 0);
		immediate_context->CSSetConstantBuffers(1, 1, this->particle_common_constant.GetAddressOf());
	}

	// 初期化用パラメータを更新
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		HRESULT hr = immediate_context->Map(this->particle_init_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		memcpy_s(mappedResource.pData, sizeof(CPUGPUBuffer) * PERTICLES_PIECE_NO,
			this->particle_data_pool.data(), sizeof(CPUGPUBuffer) * PERTICLES_PIECE_NO);
		immediate_context->Unmap(this->particle_init_buffer.Get(), 0);
	}

	// GPU専用データを扱う入力リソースビューの設定 
	immediate_context->CSSetShaderResources(0, 1, this->particle_gpu_data_SRV[chainSRV].GetAddressOf());
	// CPU・GPU共通のデータを扱う入力リソースビューの設定
	immediate_context->CSSetShaderResources(1, 1, this->particle_init_SRV.GetAddressOf());	

	// GPU専用データを扱う書き込みリソースビューの設定 
	immediate_context->CSSetUnorderedAccessViews(0, 1, this->particle_gpu_data_UAV[chainUAV].GetAddressOf(), NULL);
	// CPU・GPU共通のデータを扱う出力リソースビューの設定
	immediate_context->CSSetUnorderedAccessViews(1, 1, this->particle_to_cpu_UAV.GetAddressOf(), NULL);

	// コンピュート・シェーダの実行
	immediate_context->Dispatch(PERTICLES_PIECE_NO, 1, 1);//グループの数

	{
		HRESULT hr = S_OK;

		immediate_context->CopyResource(this->staging_buffer.Get(), this->particle_to_cpu_buffer.Get());
		// 結果をCPUから読み込む
		// リードバッファ読み込みの為のロック
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		hr = immediate_context->Map(this->staging_buffer.Get(), 0, D3D11_MAP_READ, 0, &mapped_resource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		CPUGPUBuffer* cpgp_buffer = (CPUGPUBuffer*)mapped_resource.pData;

		this->particle_data_pool.assign(cpgp_buffer, &cpgp_buffer[PERTICLES_PIECE_NO]);

		//リードバッファのロック解除
		immediate_context->Unmap(this->staging_buffer.Get(), 0);

		// 空いているパーティクルの数を計算する
		CalculateFreeParticleCount();
	}

	// リソースビューの解除
	{
		ID3D11UnorderedAccessView* view_null_uav = nullptr;
		immediate_context->CSSetUnorderedAccessViews(0, 1, &view_null_uav, NULL);
		immediate_context->CSSetUnorderedAccessViews(1, 1, &view_null_uav, NULL);
		ID3D11ShaderResourceView* view_null_srv = nullptr;
		immediate_context->CSSetShaderResources(0, 1, &view_null_srv);
		immediate_context->CSSetShaderResources(1, 1, &view_null_srv);
	}

	// バッファの切り替え
	{
		this->chainSRV = this->chainSRV ? 0 : 1;//バッファーの切り替え
		this->chainUAV = this->chainUAV ? 0 : 1;//バッファーの切り替え
	}
}

void ParticleSystem::Render()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11DeviceContext* immediate_context = graphics->GetDeviceContext();

	RenderContext rc{};
	// RenderContext設定
	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		if (camera_manager.Get())
		{
			std::shared_ptr<CameraComponent> camera = camera_manager->GetCurrentCamera();
			if (camera)
			{
				rc.view = camera->GetViewTransform();
				rc.projection = camera->GetProjectionTransform();
			}
		}
	}

	// ブレンドステート設定
	immediate_context->OMSetBlendState(this->blend_state.Get(), nullptr, 0xFFFFFFFF);
	// 深度ステンシルステートの設定
	immediate_context->OMSetDepthStencilState(this->depth_stencil_state.Get(), 0);
	// ラスタライザーステートの設定
	immediate_context->RSSetState(this->rasterizer_state.Get());
	// サンプラーステートの設定
	immediate_context->PSSetSamplers(0, 1, this->sampler_state.GetAddressOf());

	//定数バッファの更新
	{
		// シーン定数更新・設定
		SceneConstantsBuffer cbScene{};
		MYMATRIX View = rc.view;
		MYMATRIX Projection = rc.projection;
		MYMATRIX View_projection = View * Projection;
		cbScene.view_matrix = rc.view;
		cbScene.projection_matrix = rc.projection;
		View_projection.GetFlaot4x4(cbScene.view_projection);
		immediate_context->UpdateSubresource(this->scene_constant_buffer.Get(), 0, 0, &cbScene, 0, 0);
		immediate_context->VSSetConstantBuffers(0, 1, scene_constant_buffer.GetAddressOf());
		immediate_context->GSSetConstantBuffers(0, 1, scene_constant_buffer.GetAddressOf());
		immediate_context->PSSetConstantBuffers(0, 1, scene_constant_buffer.GetAddressOf());

		// パーティクル共通データの設定
		immediate_context->VSSetConstantBuffers(1, 1, this->particle_common_constant.GetAddressOf());
	}
	
	//	点描画設定
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//	シェーダー設定
	immediate_context->VSSetShader(this->vertex_shader.Get(), nullptr, 0);
	immediate_context->GSSetShader(this->geometry_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(this->pixel_shader.Get(), nullptr, 0);

	//	テクスチャ設定
	immediate_context->PSSetShaderResources(0, 1, this->texture->GetAddressOf());
	immediate_context->GSSetShaderResources(0, 1, this->texture->GetAddressOf());

	// 頂点シェーダーにパーティクル情報送る
	immediate_context->VSSetShaderResources(0, 1, this->particle_gpu_data_SRV[chainSRV].GetAddressOf());

	//	パーティクル情報分描画コール
	immediate_context->Draw(PERTICLES_PIECE_NO, 0);

	//	シェーダ無効化
	immediate_context->VSSetShader(nullptr, nullptr, 0);
	immediate_context->GSSetShader(nullptr, nullptr, 0);
	immediate_context->PSSetShader(nullptr, nullptr, 0);
}

void ParticleSystem::LoadTexture(const char* filename)
{
	_ASSERT_EXPR_W(!this->texture.get(), L"テクスチャは既に読み込まれています");
	if (this->texture.get()) return;
	this->texture = std::make_unique<Texture>(filename);
}

int ParticleSystem::CalculateFreeParticleCount()
{
	this->free_particle_count = 0;	// カウントをリセット

	for (size_t i = 0; i < this->particle_data_pool.size(); ++i)
	{
		if (!this->particle_data_pool[i].is_busy) ++this->free_particle_count;
	}
	return static_cast<int>(this->free_particle_count);
}

void ParticleSystem::PlayEffect(
	int type,
	DirectX::XMFLOAT3 parent_pos,
	float parent_rot,
	DirectX::XMFLOAT3 parent_color,
	DirectX::XMFLOAT3 forward
)
{
	switch (type)
	{
	case EFFECT_HIT:		// 斬撃エフェクト
	{
		PlayEffect(
			parent_pos,
			parent_rot,
			parent_color,
			forward,
			this->effect_hit
		);
		break;
	}
	case EFFECT_HIT2:	// 落下斬撃エフェクト
	{
		PlayEffect(
			parent_pos,
			parent_rot,
			parent_color,
			forward,
			this->effect_hit2
		);
		break;
	}
	case EFFECT_SLASH:	// 落下斬撃エフェクト
	{
		PlayEffect(
			parent_pos,
			parent_rot,
			parent_color,
			forward,
			this->effect_slash
		);
		break;
	}
	default:
		_ASSERT_EXPR_W(false, L"エフェクトタイプが存在しません");
		break;
	}
}

void ParticleSystem::PlayEffect(
	DirectX::XMFLOAT3 parent_pos,
	float parent_rot,
	DirectX::XMFLOAT3 parent_color,
	DirectX::XMFLOAT3 forward,
	const std::vector<CPUGPUBuffer>& particle_pool
)
{
	if (this->free_particle_count < particle_pool.size()) return;	// 必用数の空きがなければreturn

	size_t count = 0;	// 追加したパーティクルの数
	for (size_t i = 0; i < this->particle_data_pool.size(); ++i)
	{
		if (this->particle_data_pool[i].is_busy) continue;	// 稼働していたら飛ばす

		this->particle_data_pool[i] = particle_pool[count]; // データ設定

		// ポジション設定
		{
			// 親の位置に合わせる
			this->particle_data_pool[i].initial_position.x += parent_pos.x;
			this->particle_data_pool[i].initial_position.y += parent_pos.y;
			this->particle_data_pool[i].initial_position.z += parent_pos.z;
		}
		// 移動方向設定
		{
			this->particle_data_pool[i].forward = forward;
		}
		// 角度設定
		{
			this->particle_data_pool[i].rot += parent_rot;
		}
		// 色設定
		{
			this->particle_data_pool[i].color.x *= parent_color.x;
			this->particle_data_pool[i].color.y *= parent_color.y;
			this->particle_data_pool[i].color.z *= parent_color.z;
		}

		++count;

		if (particle_pool.size() <= count) break;
	}

	this->free_particle_count -= particle_pool.size();	// 空きパーティクルの数を減らす
}

#ifdef _DEBUG

void ParticleSystem::DebugDrawGUI()
{
	if(ImGui::Begin("ParticleSystem"))
	{
		// 空いてるパーティクル数表示
		std::string text = "Free Particle Count: " + std::to_string(this->free_particle_count);
		ImGui::Text(text.c_str());

		// 稼働中のパーティクル数表示
		text = "Working Particle Count: " + std::to_string(PERTICLES_PIECE_NO  - this->free_particle_count);
		ImGui::Text(text.c_str());

		// パーティクル定数
		if (ImGui::TreeNodeEx("ParticleCommonConstant", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat("Elapsed Time", &particle_data.elapsed_time);
			ImGui::TreePop();
		}

		// 再生するエフェクトの情報
		if (ImGui::TreeNodeEx("Effect Param"))
		{
			DebugDrawEffectParamGUI("SLASH", this->effect_slash);
			DebugDrawEffectParamGUI("EFFECT HIT", this->effect_hit);

			ImGui::TreePop();
		}

		// 各CPUで操作するパーティクルの情報
		if (ImGui::TreeNodeEx("Particle Param Pool"))
		{
			for (size_t i = 0; i < this->particle_data_pool.size(); ++i)
			{
				std::string label = "Particle" + std::to_string(i);
				if (ImGui::TreeNode(label.c_str()))
				{
					CPUGPUBuffer& data = this->particle_data_pool[i];
					ImGui::DragFloat3(("initial_position##" + label + std::to_string(i)).c_str(), &data.initial_position.x, 0.01f);
					ImGui::DragFloat3(("velocity##" + label + std::to_string(i)).c_str(), &data.velocity.x, 0.01f);
					ImGui::DragFloat3(("acceleration##" + label + std::to_string(i)).c_str(), &data.acceleration.x, 0.01f);
					ImGui::DragFloat2(("initial_scale##" + label + std::to_string(i)).c_str(), &data.initial_scale.x);
					ImGui::DragFloat2(("f_scale##" + label + std::to_string(i)).c_str(), &data.f_scale.x);
					ImGui::DragFloat2(("e_scale##" + label + std::to_string(i)).c_str(), &data.e_scale.x);
					ImGui::ColorEdit3(("color##" + label + std::to_string(i)).c_str(), &data.color.x);
					ImGui::DragAngleSlider(("rot##" + label + std::to_string(i)).c_str(), data.rot);
					ImGui::DragAngleSlider(("rot_speed##" + label + std::to_string(i)).c_str(), data.rot_speed);
					ImGui::DragFloat(("initial_lifetime##" + label + std::to_string(i)).c_str(), &data.initial_lifetime); 
					ImGui::InputInt("type", &data.type);
					ImGui::InputInt("Step", &data.step);
					bool flag = static_cast<bool> (data.is_busy);
					if (ImGui::Checkbox("Is Busy", &flag))
					{
						data.is_busy = static_cast<int>(flag);
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void ParticleSystem::DebugDrawEffectParamGUI(std::string label, std::vector<CPUGPUBuffer>& effect_pool)
{
	int count = 0;
	if (ImGui::TreeNodeEx(label.c_str()))
	{
		for (auto& effect : effect_pool)
		{
			if (ImGui::TreeNodeEx(std::to_string(count).c_str()))
			{
				ImGui::DragFloat3(("initial_position##" + label + std::to_string(count)).c_str(), &effect.initial_position.x, 0.01f);
				ImGui::SliderFloat3(("forward##" + label + std::to_string(count)).c_str(), &effect.forward.x, 0.0f, 1.0f);
				ImGui::DragFloat3(("velocity##" + label + std::to_string(count)).c_str(), &effect.velocity.x, 0.01f);
				ImGui::DragFloat3(("acceleration##" + label + std::to_string(count)).c_str(), &effect.acceleration.x, 0.01f);
				ImGui::DragFloat2(("initial_scale##" + label + std::to_string(count)).c_str(), &effect.initial_scale.x);
				ImGui::DragFloat2(("f_scale##" + label + std::to_string(count)).c_str(), &effect.f_scale.x);
				ImGui::DragFloat2(("e_scale##" + label + std::to_string(count)).c_str(), &effect.e_scale.x);
				ImGui::ColorEdit3(("color##" + label + std::to_string(count)).c_str(), &effect.color.x);
				ImGui::DragAngleSlider(("rot##" + label + std::to_string(count)).c_str(), effect.rot);
				ImGui::DragAngleSlider(("rot_speed##" + label + std::to_string(count)).c_str(), effect.rot_speed);
				ImGui::DragFloat(("initial_lifetime##" + label + std::to_string(count)).c_str(), &effect.initial_lifetime);
				ImGui::TreePop();
			}
			++count;
		}
		ImGui::TreePop();
	}
}

#endif // DEBUG