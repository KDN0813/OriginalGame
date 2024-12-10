#include <imgui.h>
#include "Shader/ShaderLoader.h"
#include "Graphics/Graphics.h"
#include "Camera/CameraManager.h"
#include "ModelShader.h"
#include "Model/ModelResource.h"
#include "System/Misc.h"
#include "Shader\LightManager.h"

#include "Component/ModelShaderComponent.h"
#include "Component/ModelComponent.h"
#include "Component/CameraComponent.h"

ModelShader::ModelShader()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11Device* device = graphics->GetDevice();

	HRESULT hr{};

	// ���̓��C�A�E�g
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
	// ���_�V�F�[�_�[
	hr = CreateShader::VsFromCso(device, "Shader\\TemporaryVS.cso", this->vertex_shader.GetAddressOf(), this->input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �s�N�Z���V�F�[�_�[
	hr = CreateShader::PsFromCso(device, "Shader\\TemporaryPS.cso", this->pixel_shader.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �萔�o�b�t�@
	{
		// �V�[���p�o�b�t�@
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

		// ���b�V���p�o�b�t�@
		desc.ByteWidth = sizeof(MeshConstantBuffer);

		hr = device->CreateBuffer(&desc, 0, this->mesh_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �T�u�Z�b�g�p�o�b�t�@
		desc.ByteWidth = sizeof(SubsetConstantBuffer);

		hr = device->CreateBuffer(&desc, 0, this->subset_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// ���C�g�p�o�b�t�@
		desc.ByteWidth = sizeof(LightConstantBuffer);

		hr = device->CreateBuffer(&desc, 0, this->light_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �u�����h�X�e�[�g
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

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, this->depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���X�^���C�U�[�X�e�[�g
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

	// �T���v���X�e�[�g
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
}

void ModelShader::Render()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();
	RenderContext rc{};

	// RenderContext�ݒ�
	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		if (!camera_manager.Get()) return;
		std::shared_ptr<CameraComponent> camera = camera_manager->GetCurrentCamera();
		if (camera)
		{
			rc.view = camera->GetViewTransform();
			rc.projection = camera->GetProjectionTransform();
		}
	}

    Begin(dc, rc);

	for (auto shader_component : this->shader_component_vec)
	{
		shader_component->Draw(dc);
	}

    End(dc);
}

void ModelShader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
{
	dc->VSSetShader(vertex_shader.Get(), nullptr, 0);
	dc->PSSetShader(pixel_shader.Get(), nullptr, 0);
	dc->IASetInputLayout(input_layout.Get());

	ID3D11Buffer* constant_buffers[] =
	{
		this->scene_constant_buffer.Get(),
		this->mesh_constant_buffer.Get(),
		this->subset_constant_buffer.Get(),
		this->light_constant_buffer.Get()
	};
	dc->VSSetConstantBuffers(0, ARRAYSIZE(constant_buffers), constant_buffers);
	dc->PSSetConstantBuffers(0, ARRAYSIZE(constant_buffers), constant_buffers);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(blend_state.Get(), blend_factor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	dc->RSSetState(rasterizer_state.Get());
	dc->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	// �V�[���p�萔�o�b�t�@�X�V
	SceneConstantBuffer scene_CB{};

	MYMATRIX View = rc.view;
	MYMATRIX Projection = rc.projection;
	MYMATRIX View_projection = View * Projection;

	View_projection.GetFlaot4x4(scene_CB.view_projection);

	// ���C�g�p�萔�o�b�t�@�X�V
	LightConstantBuffer light_CB{};
	if (LightManager::Instance light_manager = LightManager::GetInstance(); light_manager.Get())
	{
		light_CB.ambient_color = light_manager->GetAmbientColor();
		light_CB.directional_lights = light_manager->GetLightDirection();

	}

	dc->UpdateSubresource(this->light_constant_buffer.Get(), 0, 0, &light_CB, 0, 0);
	dc->UpdateSubresource(this->scene_constant_buffer.Get(), 0, 0, &scene_CB, 0, 0);
}

void ModelShader::Draw(ID3D11DeviceContext* dc, ModelComponent* model)
{
	const ModelResource* resource = model->GetResource();
	const std::vector<ModelComponent::Node>& node_vec = model->GetNodes();

	for (size_t mesh_index = 0; mesh_index < resource->GetMeshes().size(); ++mesh_index)
	{
		const ModelResource::Mesh& mesh = resource->GetMeshes()[mesh_index];

		// �t���X�^���J�����O���s��
		if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
		{
			if (!camera_manager->IsMeshVisible(model->GetBoundingBox(mesh_index)))
			{
				// ��ʊO�Ȃ�X�L�b�v����
				continue;
			}
		}

		// ���b�V���p�萔�o�b�t�@�X�V
		MeshConstantBuffer mesh_CB;
		::memset(&mesh_CB, 0, sizeof(mesh_CB));
		if (mesh.node_indices.size() > 0)
		{
			for (size_t i = 0; i < mesh.node_indices.size(); ++i)
			{
				MYMATRIX World_transform = node_vec.at(mesh.node_indices.at(i)).world_transform;
				MYMATRIX Offset_transform = DirectX::XMLoadFloat4x4(&mesh.offset_transforms.at(i));
				MYMATRIX Bone_transform = Offset_transform * World_transform;
				Bone_transform.GetFlaot4x4(mesh_CB.bone_transforms[i]);
			}
		}
		else
		{
			MYMATRIX World_transform = node_vec.at(mesh.node_index).world_transform;
			World_transform.GetFlaot4x4(mesh_CB.bone_transforms[0]);
		}
		dc->UpdateSubresource(mesh_constant_buffer.Get(), 0, 0, &mesh_CB, 0, 0);

		UINT stride = sizeof(ModelResource::Vertex);
		UINT offset = 0;
		dc->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		dc->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			SubsetConstantBuffer cbSubset;
			cbSubset.material_color = subset.material->color;
			cbSubset.tile_count = model->GetTileCount();
			dc->UpdateSubresource(subset_constant_buffer.Get(), 0, 0, &cbSubset, 0, 0);
			dc->PSSetShaderResources(0, 1, subset.material->shader_resource_view.GetAddressOf());
			dc->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

			dc->DrawIndexed(subset.index_count, subset.start_index, 0);
		}
	}
}

void ModelShader::End(ID3D11DeviceContext* dc)
{
	dc->VSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
	dc->IASetInputLayout(nullptr);
}

void ModelShader::AddShaderComponent(ModelShaderComponent* shader_component)
{
	if (shader_component)
	{
		this->shader_component_vec.emplace_back(shader_component);
	}
}

void ModelShader::RemoveShaderComponent(ModelShaderComponent* shader_component)
{
	auto it = std::find(this->shader_component_vec.begin(), this->shader_component_vec.end(), shader_component);
	if (it == this->shader_component_vec.end()) return;
	this->shader_component_vec.erase(it);
}

#ifdef _DEBUG

void ModelShader::DrawDebugGUI()
{
	std::string text = "shader_component_vector.size" + std::to_string(this->shader_component_vec.size());
	ImGui::Text(text.c_str());
}

#endif // _DEBUG