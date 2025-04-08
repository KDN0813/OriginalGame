#include <imgui.h>
#include "Shader/ShaderLoader.h"
#include "Graphics/Graphics.h"
#include "Camera/CameraManager.h"
#include "System/MyMath/MYMATRIX.h"
#include "InstanceModelShader.h"
#include "System/Misc.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Model/InstancingModelResource.h"
#include "Model/ModelResource.h"
#include "Shader\LightManager.h"

#include "Component/InstancedModelWithStateAnimationComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CameraComponent.h"

InstancingModelShader::InstancingModelShader()
	:Singleton(this)
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11Device* device = graphics->GetDevice();

	HRESULT hr{};
	// 入力レイアウト
	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// 頂点シェーダー
	hr = CreateShader::VsFromCso(device, "Shader\\InstanceVS.cso", this->vertex_shader.GetAddressOf(), this->input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ピクセルシェーダー
	hr = CreateShader::PsFromCso(device, "Shader\\InstancePS.cso", this->pixel_shader.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

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

		HRESULT hr = device->CreateBuffer(&desc, 0, this->scene_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// サブセット用バッファ
		desc.ByteWidth = sizeof(SubsetConstantBuffer);

		hr = device->CreateBuffer(&desc, 0, this->subset_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// CommonDataConstantBuffer
		desc.ByteWidth = sizeof(CommonDataConstantBuffer);
		hr = device->CreateBuffer(&desc, 0, this->common_data_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// MeshConstantBuffer
		desc.ByteWidth = sizeof(MeshConstantBuffer);
		hr = device->CreateBuffer(&desc, 0, this->mesh_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// ライト用定数バッファ作成
		desc.ByteWidth = sizeof(LightConstantBuffer);
		hr = device->CreateBuffer(&desc, 0, this->light_constant_buffer.GetAddressOf());
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

		HRESULT hr = device->CreateBlendState(&desc, this->blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, this->depthStencil_state.GetAddressOf());
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

		HRESULT hr = device->CreateRasterizerState(&desc, this->rasterizer_state.GetAddressOf());
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

		HRESULT hr = device->CreateSamplerState(&desc, this->sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// instance_data_buffer作成
	{
		InstanceData* instance_data = new InstanceData[MAX_INSTANCES];

		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // SRV としてバインドする
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// 構造体バッファに設定
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.ByteWidth = (sizeof(InstanceData) * MAX_INSTANCES);	// バッファサイズ設定
		buffer_desc.StructureByteStride = sizeof(InstanceData);	// 構造体の各要素のサイズ設定
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = instance_data;	// 初期データ設定

		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, this->instance_data_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		delete[] instance_data;
	}

	// instance_data_structured_bufferの作成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;	// 要素の先頭インデックス
		srvDesc.Buffer.NumElements = static_cast<UINT>(MAX_INSTANCES);	// 要素の数

		HRESULT hr = device->CreateShaderResourceView(this->instance_data_buffer.Get(), &srvDesc, this->instance_data_structured_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void InstancingModelShader::Render()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();
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

	if (shader_component_vec_map.size() <= 0) return;

	// 初期設定
	{
		dc->PSSetShader(this->pixel_shader.Get(), nullptr, 0);

		const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		dc->OMSetBlendState(this->blend_state.Get(), blend_factor, 0xFFFFFFFF);
		dc->OMSetDepthStencilState(this->depthStencil_state.Get(), 0);
		dc->RSSetState(this->rasterizer_state.Get());
		dc->PSSetSamplers(0, 1, this->sampler_state.GetAddressOf());

		// 定数Buffer設定
		ID3D11Buffer* constantBuffers[] =
		{
			this->scene_constant_buffer.Get(),
			this->subset_constant_buffer.Get(),
			this->common_data_constant_buffer.Get(),
			this->mesh_constant_buffer.Get(),
			this->light_constant_buffer.Get()
		};
		dc->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
		dc->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);

		// シーン用定数バッファ更新
		SceneConstantBuffer cb_scene{};

		MYMATRIX View = rc.view;
		MYMATRIX Projection = rc.projection;
		MYMATRIX View_projection = View * Projection;

		// ライト用定数バッファ更新
		LightConstantBuffer light_cb{};
		if (LightManager::Instance light_manager = LightManager::GetInstance(); light_manager.Get())
		{
			light_cb.ambient_color = light_manager->GetAmbientColor();
			light_cb.directional_lights = light_manager->GetLightDirection();
		}

		View_projection.GetFlaot4x4(cb_scene.viewProjection);

		dc->UpdateSubresource(this->scene_constant_buffer.Get(), 0, 0, &cb_scene, 0, 0);
		dc->UpdateSubresource(this->light_constant_buffer.Get(), 0, 0, &light_cb, 0, 0);

		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	for (auto shader_component_vec : shader_component_vec_map)
	{
		bool is_render_ready = false;	// 描画準備ができているか
		InstancingModelShaderComponent* valid_Sc = nullptr;	// 使用可能なシェーダーコンポーネント
		
		for (auto shader_component : shader_component_vec.second)
		{
			if (!shader_component) continue;

			if (!is_render_ready)
			{
				// シェーダーコンポーネントが使用可能か判定
				if (!(valid_Sc = IsShaderValid(shader_component))) continue;
				shader_component->InstancingStart();
				shader_component->InstancingAdd();
				is_render_ready = true;
			}
			else
			{
				shader_component->InstancingAdd();
			}
		}
		if (is_render_ready) valid_Sc->InstancingEnd(dc);
	}

	// 修了処理
	{
		dc->VSSetShader(nullptr, nullptr, 0);
		dc->PSSetShader(nullptr, nullptr, 0);
		dc->IASetInputLayout(nullptr);
	}
}

void InstancingModelShader::InstancingStart()
{
	// カウントリセット
	this->instance_count = 0;
}

void InstancingModelShader::InstancingAdd(
	InstancedModelWithAnimationComponent* model,
	Transform3DComponent* transform
)
{
	this->instance_datas[this->instance_count] =
	{
		model->GetAnimationStartOffset(),
		model->GetAnimeFrame(),
		transform->GetWorldTransform(),
		model->GetBaseColor(),
	};
	++this->instance_count;
}

void InstancingModelShader::InstancingEnd(ID3D11DeviceContext* dc, InstancedModelWithAnimationComponent* model)
{
	InstancingRender(dc, model);
}

void InstancingModelShader::AddShaderComponent(InstancingModelShaderComponent* shader_component)
{
	if (shader_component == nullptr) return;

	// 同じモデルのコンテナがあるなら追加、ないなら要素を追加する
	const int modelId = shader_component->GetModelId();
	shader_component_vec_map[modelId].emplace_back(shader_component);
}

void InstancingModelShader::RemoveShaderComponent(InstancingModelShaderComponent* shader_component)
{
	if (shader_component == nullptr) return;

	const int& modelId = shader_component->GetModelId();
	auto it = shader_component_vec_map.find(modelId);
	if (it != shader_component_vec_map.end())
	{
		auto it = std::find(this->shader_component_vec_map[modelId].begin(), this->shader_component_vec_map[modelId].end(), shader_component);
		if (it == this->shader_component_vec_map[modelId].end()) return;
		this->shader_component_vec_map[modelId].erase(it);
	}
}

InstancingModelShaderComponent* InstancingModelShader::IsShaderValid(InstancingModelShaderComponent* shader_component)
{
	return (shader_component->IsShaderValid()) ? shader_component : nullptr;
}

void InstancingModelShader::InstancingRender(ID3D11DeviceContext* dc, InstancedModelWithAnimationComponent* model)
{
	// インスタンシングバッファの更新
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HRESULT hr = dc->Map(instance_data_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	memcpy_s(mappedResource.pData, sizeof(InstanceData) * MAX_INSTANCES,
		this->instance_datas, sizeof(InstanceData) * this->instance_count);
	dc->Unmap(instance_data_buffer.Get(), 0);

	dc->VSSetShaderResources(2, 1, instance_data_structured_buffer.GetAddressOf());

	// ボーントランスフォームテクスチャ設定
	dc->VSSetShaderResources(1, 1, model->GetInstancingModelResource()->GetBoneTransformTexture());
	// 共通定数バッファの更新
	CommonDataConstantBuffer common_data_buffer{};
	common_data_buffer.bone_transform_count = model->GetInstancingModelResource()->GetBoneTransformCount();
	dc->UpdateSubresource(common_data_constant_buffer.Get(), 0, 0, &common_data_buffer, 0, 0);

	size_t mesh_index = 0;
	for (const ModelResource::Mesh& mesh : model->GetModelResource()->GetMeshes())
	{
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			// バッファ設定
			{
				dc->VSSetShader(vertex_shader.Get(), nullptr, 0);
				dc->IASetInputLayout(input_layout.Get());

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
				mesh_buffer.offset = model->GetInstancingModelResource()->GetMeshOffsets()[mesh_index];
				dc->UpdateSubresource(mesh_constant_buffer.Get(), 0, 0, &mesh_buffer, 0, 0);
			}

			//	サブセット単位で描画
			DrawSubset(dc, subset);
			++mesh_index;
		}
	}
}

void InstancingModelShader::DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset)
{
	SubsetConstantBuffer cbSubset;
	cbSubset.materialColor = subset.material->color;
	dc->UpdateSubresource(subset_constant_buffer.Get(), 0, 0, &cbSubset, 0, 0);
	dc->PSSetShaderResources(0, 1, subset.material->shader_resource_view.GetAddressOf());
	dc->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	dc->DrawIndexedInstanced(subset.index_count, this->instance_count, subset.start_index, 0, 0);
}

#ifdef _DEBUG

void InstancingModelShader::DrawDebugGUI()
{
	std::string text = "shader_component_vector.size" + std::to_string(this->shader_component_vec_map.size());
	ImGui::Text(text.c_str());
	ImGui::InputInt("Instance Count", &this->instance_count);
}

#endif // _DEBUG