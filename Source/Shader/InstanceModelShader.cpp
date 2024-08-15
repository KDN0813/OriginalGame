#include "InstanceModelShader.h"
#include "System/Misc.h"
#include "Component/ShaderComponent.h"
#include "Model/InstancingModelResource.h"
#include "Model/ModelResource.h"

InstanceModelShader::InstanceModelShader(ID3D11Device* device)
{
	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\InstanceVS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// 頂点シェーダー生成
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, this->vertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, this->inputLayout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\InstancePS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ピクセルシェーダー生成
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, this->pixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(SceneConstantBuffer);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, this->sceneConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// サブセット用バッファ
		desc.ByteWidth = sizeof(SubsetConstantBuffer);

		hr = device->CreateBuffer(&desc, 0, this->subsetConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// CommonDataConstantBuffer
		desc.ByteWidth = sizeof(CommonDataConstantBuffer);
		hr = device->CreateBuffer(&desc, 0, this->common_data_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// MeshConstantBuffer
		desc.ByteWidth = sizeof(MeshConstantBuffer);
		hr = device->CreateBuffer(&desc, 0, this->mesh_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, this->blendState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, this->depthStencilState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, this->rasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// サンプラステート
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		HRESULT hr = device->CreateSamplerState(&desc, this->samplerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void InstanceModelShader::Render(ID3D11DeviceContext* dc, const RenderContext& rc)
{
	if (shader_component_Wptr_vector.size() <= 0) return;

	Begin(dc, rc);
	
	Draw(dc);

	End(dc);
}

//void InstanceModelShader::SetInstancingResource(
//	ModelResource* model_resource,
//	InstancingModelResource* instancing_model_resource
//)
//{
//	// モデルリソースの取得
//	this->model_resource = model_resource;
//	// インスタンシングモデルリソースの取得
//	this->instancing_model_resource = instancing_model_resource;
//}

bool InstanceModelShader::SetInstancingResource(ModelResource* model_resource, InstancingModelResource* instancing_model_resource)
{
	this->model_resource = model_resource;
	this->instancing_model_resource = instancing_model_resource;
	
	return (this->model_resource != nullptr && this->instancing_model_resource != nullptr);
}

void InstanceModelShader::InstancingAdd()
{
}

void InstanceModelShader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
{
	dc->PSSetShader(this->pixelShader.Get(), nullptr, 0);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(this->blendState.Get(), blend_factor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	dc->RSSetState(this->rasterizerState.Get());
	dc->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());

	// 定数buffer設定
	ID3D11Buffer* constantBuffers[] =
	{
		this->sceneConstantBuffer.Get(),
		this->subsetConstantBuffer.Get(),
		this->common_data_constant_buffer.Get(),
		this->mesh_constant_buffer.Get(),
	};
	dc->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	dc->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);

	// シーン用定数バッファ更新
	SceneConstantBuffer cbScene;

	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMStoreFloat4x4(&cbScene.viewProjection, V * P);

	dc->UpdateSubresource(this->sceneConstantBuffer.Get(), 0, 0, &cbScene, 0, 0);

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void InstanceModelShader::Draw(ID3D11DeviceContext* dc)
{
	// 先頭のシェーダーコンポーネント取得
	int shader_component_index = 0;
	auto& fast_shader_component_weekPtr = shader_component_Wptr_vector[shader_component_index];
	while (fast_shader_component_weekPtr.expired() == true)
	{
		++shader_component_index;
		if (shader_component_Wptr_vector.size() <= shader_component_index)
		{
			return;
		}
		fast_shader_component_weekPtr = shader_component_Wptr_vector[shader_component_index];
	}
	std::shared_ptr<ShaderComponent> fast_shader_component = fast_shader_component_weekPtr.lock();

	// 設定に失敗したら処理を中断
	if (!fast_shader_component->SetInstancingResource()) return;

	// 描画するインスタンスデータの追加
	for (auto& shader_component_Wptr : shader_component_Wptr_vector)
	{
		if (shader_component_Wptr.expired() == false)
		{
			std::shared_ptr<ShaderComponent> shader_component = shader_component_Wptr.lock();
			shader_component->InstancingAdd();
		}
		else
		{
			// TODO (08/13) 削除処理作成
		}
	}

	// BTT設定
	dc->VSSetShaderResources(1, 1, this->instancing_model_resource->GetBoneTransformTexture());

	size_t mesh_index = 0;
	for (const ModelResource::Mesh& mesh : this->model_resource->GetMeshes())
	{
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			// バッファ設定
			{
				dc->VSSetShader(vertexShader.Get(), nullptr, 0);
				dc->IASetInputLayout(inputLayout.Get());

				ID3D11Buffer* vertex_buffers[] =
				{
					mesh.vertex_buffer.Get(),
				};
				UINT strides[] =
				{
					sizeof(ModelResource::Vertex),
				};
				UINT offset[_countof(vertex_buffers)] = { 0 };
				dc->IASetVertexBuffers(0, _countof(vertex_buffers), vertex_buffers, strides, offset);
				dc->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

				MeshConstantBuffer mesh_buffer{};
				mesh_buffer.offset = this->instancing_model_resource->GetMeshOffsets()[mesh_index];
				dc->UpdateSubresource(mesh_constant_buffer.Get(), 0, 0, &mesh_buffer, 0, 0);
			}

			//	サブセット単位で描画
			DrawSubset(dc, subset);
			++mesh_index;
		}
	}
}

void InstanceModelShader::End(ID3D11DeviceContext* dc)
{
	{
		dc->VSSetShader(nullptr, nullptr, 0);
		dc->PSSetShader(nullptr, nullptr, 0);
		dc->IASetInputLayout(nullptr);
	}
}

void InstanceModelShader::DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset)
{
}
