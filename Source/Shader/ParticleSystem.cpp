#include "ParticleSystem.h"
#include "../Hlsl/ParticleDisp.h"
#include "Shader/ShaderLoader.h"
#include "Graphics/Graphics.h"
#include "Camera/CameraManager.h"
#include "System/Misc.h"
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
			particle_data.timer_max = 0.8f;
			particle_data.default_size = { 0.340f, 1.28f };
			particle_data.f_scale = DirectX::XMFLOAT2(2.0f, 1.0f);
			particle_data.e_scale = DirectX::XMFLOAT2(1.0f, 3.5f);

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

	// CPU・GPU共通のデータを扱うデータの取得
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
	return this->free_particle_count;
}

void ParticleSystem::PlayEffect(
	DirectX::XMFLOAT3 p,
	DirectX::XMFLOAT3 c,
	float rot
)
{
	for (size_t i = 0 ; i < this->particle_data_pool.size(); ++i)
	{
		if (this->particle_data_pool[i].is_busy) continue;

		CPUGPUBuffer particle_data
		{
			p,
			c,
			rot,
			0,	// step
			1,	// is_busy
		};
		this->particle_data_pool[i] = particle_data;

		--this->free_particle_count;	// 空きパーティクルの数を減らす
		break;
	}
}

void ParticleSystem::PlayGroupEffect(const std::vector<ParticleParam>& particle_pool)
{
	const size_t effect_count = particle_pool.size();
	if (this->free_particle_count < effect_count) return;

	int count = 0;
	for (size_t i = 0; i < this->particle_data_pool.size(); ++i)
	{
		if (this->particle_data_pool[i].is_busy) continue;

		this->particle_data_pool[i].color = particle_pool[count].color;
		this->particle_data_pool[i].position = particle_pool[count].position;
		this->particle_data_pool[i].rot = particle_pool[count].rot;
		this->particle_data_pool[i].is_busy = 1;
		this->particle_data_pool[i].step = 0;

		++count;

		if (effect_count <= count) break;
	}

	this->free_particle_count -= effect_count;	// 空きパーティクルの数を減らす
}

#ifdef _DEBUG

void ParticleSystem::DebugDrawGUI()
{
	if(ImGui::Begin("ParticleSystem"))
	{
		ImGui::InputSize_t("Free Particle Count", this->free_particle_count);
		if (ImGui::TreeNodeEx("ParticleCommonConstant", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat2("Default Size", &particle_data.default_size.x);
			ImGui::InputFloat("Elapsed Time", &particle_data.elapsed_time);
			ImGui::InputFloat2("Fast Scale", &particle_data.f_scale.x);
			ImGui::InputFloat2("End Scale", &particle_data.e_scale.x);
			ImGui::InputFloat("Timer Max", &particle_data.timer_max);
			ImGui::TreePop();
		}

		for (size_t i = 0; i < this->particle_data_pool.size(); ++i)
		{
			std::string label = "Particle" + std::to_string(i);
			if (ImGui::TreeNode(label.c_str()))
			{
				CPUGPUBuffer& data = this->particle_data_pool[i];
				ImGui::InputFloat3("Position", &data.position.x);
				ImGui::InputFloat("Rrot", &data.rot);
				ImGui::InputInt("Step", &data.step);
				bool flag = static_cast<bool> (data.is_busy);
				if (ImGui::Checkbox("Is Busy", &flag))
				{
					data.is_busy = static_cast<int>(flag);
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

#endif // DEBUG