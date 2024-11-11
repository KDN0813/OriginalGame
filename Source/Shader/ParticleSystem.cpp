#include "ParticleSystem.h"
#include "Shader/ShaderLoader.h"
#include "Graphics/Graphics.h"
#include "Camera/CameraManager.h"
#include "Component/CameraComponent.h"
#include "System/Misc.h"

ParticleSystem::ParticleSystem(const char* filename, int num)
	: num_particles(num),texture(std::make_unique<Texture>(filename))
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11Device* device = graphics->GetDevice();

	HRESULT hr;

	//	�p�[�e�B�N����񃊃X�g
	data = new ParticleData[num];
	ZeroMemory(this->data, sizeof(ParticleData) * num);

	//	���_��񃊃X�g
	this->v = new Vertex[num];
	ZeroMemory(v, sizeof(Vertex) * num);

	for (int i = 0; i < this->num_particles; i++) { this->data[i].type = -1; }

	//	���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	//	���_�����̃o�b�t�@
	bd.ByteWidth = sizeof(Vertex) * num_particles;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&bd, NULL, this->vertex_buffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	//	�萔�o�b�t�@����
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0; 
	{
		buffer_desc.ByteWidth = sizeof(ForPerFrameConstantBuffer);
		hr = device->CreateBuffer(&buffer_desc, nullptr, this->for_per_frame_constant_buffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	{
		buffer_desc.ByteWidth = sizeof(SceneConstantsBuffer);
		hr = device->CreateBuffer(&buffer_desc, nullptr, this->scene_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	//	���_�V�F�[�_�[
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	CreateShader::VsFromCso(device, "Shader\\GeometryParticle_vs.cso", this->vertex_shader.ReleaseAndGetAddressOf(), this->input_layout.ReleaseAndGetAddressOf(), input_element_desc, _countof(input_element_desc));

	//	�W�I���g���V�F�[�_�[
	CreateShader::GsFromCso(device, "Shader\\GeometryParticle_gs.cso", this->geometry_shader.ReleaseAndGetAddressOf());

	//	�s�N�Z���V�F�[�_�[
	CreateShader::PsFromCso(device, "Shader\\GeometryParticle_ps.cso", this->pixel_shader.ReleaseAndGetAddressOf());

	// �u�����h�X�e�[�g�쐬
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

	// �[�x�X�e�[�g�쐬
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, this->depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���X�^���C�U�X�e�[�g
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
}

ParticleSystem::~ParticleSystem()
{
	delete[] this->data;
	delete[] this->v;
}

void ParticleSystem::Update(float elapsed_time)
{
	for (int i = 0; i < num_particles; i++) {
		if (data[i].type < 0) continue;

		this->data[i].vx += this->data[i].ax * elapsed_time;
		this->data[i].vy += this->data[i].ay * elapsed_time;
		this->data[i].vz += this->data[i].az * elapsed_time;

		this->data[i].x += this->data[i].vx * elapsed_time;
		this->data[i].y += this->data[i].vy * elapsed_time;
		this->data[i].z += this->data[i].vz * elapsed_time;

		this->data[i].timer -= elapsed_time;
		this->data[i].alpha = sqrtf(this->data[i].timer);

		// �I������
		if (this->data[i].timer <= 0)
			this->data[i].type = -1;
	}
}

void ParticleSystem::Render()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11DeviceContext* immediate_context = graphics->GetDeviceContext();

	RenderContext rc{};
	// RenderContext�ݒ�
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

	// �u�����h�X�e�[�g�ݒ�
	immediate_context->OMSetBlendState(this->blend_state.Get(), nullptr, 0xFFFFFFFF);
	// �[�x�X�e���V���X�e�[�g�̐ݒ�
	immediate_context->OMSetDepthStencilState(this->depth_stencil_state.Get(), 0);
	// ���X�^���C�U�[�X�e�[�g�̐ݒ�
	immediate_context->RSSetState(this->rasterizer_state.Get());

	//�萔�o�b�t�@�̍X�V
	// �V�[���萔�X�V�E�ݒ�
	SceneConstantsBuffer cbScene;
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
	// �p�[�e�B�N���萔�X�V�E�ݒ�
	ForPerFrameConstantBuffer cb;
	cb.size = { 1.0f,1.0f };
	immediate_context->UpdateSubresource(this->for_per_frame_constant_buffer.Get(), 0, nullptr, &cb, 0, 0);
	immediate_context->VSSetConstantBuffers(1, 1, this->for_per_frame_constant_buffer.GetAddressOf());
	immediate_context->GSSetConstantBuffers(1, 1, this->for_per_frame_constant_buffer.GetAddressOf());
	immediate_context->PSSetConstantBuffers(1, 1, this->for_per_frame_constant_buffer.GetAddressOf());

	//	�_�`��ݒ�
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//	�V�F�[�_�[�ݒ�
	immediate_context->VSSetShader(this->vertex_shader.Get(), nullptr, 0);
	immediate_context->GSSetShader(this->geometry_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(this->pixel_shader.Get(), nullptr, 0);

	//	���̓��C�A�E�g�ݒ�
	immediate_context->IASetInputLayout(this->input_layout.Get());

	//	�e�N�X�`���ݒ�
	immediate_context->PSSetShaderResources(0, 1, this->texture->GetAddressOf());

	//	�p�[�e�B�N�����𒸓_�o�b�t�@�ɓ]��
	int n = 0; //�p�[�e�B�N��������
	for (int i = 0; i < this->num_particles; i++)
	{
		if (this->data[i].type < 0) continue;

		this->v[n].position.x = this->data[i].x;
		this->v[n].position.y = this->data[i].y;
		this->v[n].position.z = this->data[i].z;
		this->v[n].texcoord.x = this->data[i].w;
		this->v[n].texcoord.y = this->data[i].h;
		this->v[n].color.x = this->v[n].color.y = this->v[n].color.z = 1.0f;
		this->v[n].color.w = this->data[i].alpha;

		this->v[n].param.rot = 0;
		++n;
	}
	//	���_�f�[�^�X�V
	immediate_context->UpdateSubresource(this->vertex_buffer.Get(), 0, nullptr, v, 0, 0);

	//	�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	immediate_context->IASetVertexBuffers(0, 1, this->vertex_buffer.GetAddressOf(), &stride, &offset);

	//	�p�[�e�B�N����񕪕`��R�[��
	immediate_context->Draw(n, 0);

	//	�V�F�[�_������
	immediate_context->VSSetShader(nullptr, nullptr, 0);
	immediate_context->GSSetShader(nullptr, nullptr, 0);
	immediate_context->PSSetShader(nullptr, nullptr, 0);
}

void ParticleSystem::Set(
	float timer,
	DirectX::XMFLOAT3 p,
	DirectX::XMFLOAT3 v,
	DirectX::XMFLOAT3 f,
	DirectX::XMFLOAT2 size
)
{
	for (int i = 0; i < num_particles; i++)
	{
		this->data[i].x = p.x;
		this->data[i].y = p.y;
		this->data[i].z = p.z;
		this->data[i].vx = v.x;
		this->data[i].vy = v.y;
		this->data[i].vz = v.z;
		this->data[i].ax = f.x;
		this->data[i].ay = f.y;
		this->data[i].az = f.z;
		this->data[i].w = size.x;
		this->data[i].h = size.y;
		this->data[i].alpha = 1.0f;
		this->data[i].timer = timer;
		this->data[i].type = 1;
		break;
	}
}
