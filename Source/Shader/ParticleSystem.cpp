#include "ParticleSystem.h"
#include "Shader/ParticleDisp.h"
#include "Shader/ShaderLoader.h"
#include "Graphics/Graphics.h"
#include "Camera/CameraManager.h"
#include "System/Misc.h"

#include "Component/CameraComponent.h"

ParticleSystem::ParticleSystem(const char* filename, int num)
	: num_particles(num),texture(std::make_unique<Texture>(filename))
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11Device* device = graphics->GetDevice();

	HRESULT hr;

	//	�p�[�e�B�N����񃊃X�g
	this->datas = new ParticleData[num];
	ZeroMemory(this->datas, sizeof(ParticleData) * num);

	//	���_��񃊃X�g
	this->v = new Vertex[num];
	ZeroMemory(v, sizeof(Vertex) * num);

	for (int i = 0; i < this->num_particles; i++) { this->datas[i].type = -1; }

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
		{ "SCALE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = CreateShader::VsFromCso(device, "Shader\\GeometryParticle_vs.cso", this->vertex_shader.ReleaseAndGetAddressOf(), this->input_layout.ReleaseAndGetAddressOf(), input_element_desc, _countof(input_element_desc));
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	�W�I���g���V�F�[�_�[
	hr = CreateShader::GsFromCso(device, "Shader\\GeometryParticle_gs.cso", this->geometry_shader.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	�s�N�Z���V�F�[�_�[
	hr = CreateShader::PsFromCso(device, "Shader\\GeometryParticle_ps.cso", this->pixel_shader.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �R���s���[�g�V�F�[�_�[
	hr = CreateShader::CsFromCso(device, "Shader\\GeometryParticle_cs.cso", this->compute_shader.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

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

	// �������p�o�b�t�@�̍쐬
	{
		struct ParticleData* data = new ParticleData[PERTICLES_PIECE_NO];
		for (int i = 0; i < PERTICLES_PIECE_NO; ++i) {

			data[i].pos = DirectX::XMFLOAT3(0, 0, 0);      // �ʒu
			data[i].w = 0.0f;							   // �摜�̍���
			data[i].h = 0.0f;							   // �摜��
			data[i].scale = DirectX::XMFLOAT3(0, 0, 0);	   // �g�嗦
			data[i].f_scale = DirectX::XMFLOAT3(0, 0, 0);  // �g�嗦(�J�n)
			data[i].e_scale = DirectX::XMFLOAT3(0, 0, 0);  // �g�嗦(�I��)
			data[i].v = DirectX::XMFLOAT3(0, 0, 0);        // ���x
			data[i].a = DirectX::XMFLOAT3(0, 0, 0); 	   // �����x
			data[i].alpha = 0;							   // �����x
			data[i].timer_max = 0;						   // ��������(�ő�)
			data[i].timer = 0;							   // ��������
			data[i].rot = 0.0f;							   // �p�x
			data[i].type = -1.0f;						   // 
		}

		// ���[�N���\�[�X�̐ݒ�
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(ParticleData); // �o�b�t�@ �T�C�Y
		Desc.Usage = D3D11_USAGE_DYNAMIC;//�X�e�[�W�̓��o�͂�OK�BGPU�̓��o��OK�B
		Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//UNORDERED�̃_�C�i�~�b�N�̓_���������B
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\�����o�b�t�@
		Desc.StructureByteStride = sizeof(ParticleData);
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // CPU���珑������
		D3D11_SUBRESOURCE_DATA SubResource;//�T�u���\�[�X�̏������p�f�[�^���`
		SubResource.pSysMem = data;
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// �ŏ��̓��̓��\�[�X(�f�[�^������������)
		hr = device->CreateBuffer(&Desc, &SubResource, this->init_particle_data_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		delete[] data;
	}

	// �������p�\���̍쐬
	{
		// ���̓��[�N���\�[�X �r���[�̐ݒ�i���͗p�j
		D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
		ZeroMemory(&DescSRV, sizeof(DescSRV));
		DescSRV.Format = DXGI_FORMAT_UNKNOWN;
		DescSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

		DescSRV.Buffer.ElementWidth = PERTICLES_PIECE_NO; // �f�[�^��

		// �V�F�[�_ ���\�[�X �r���[�̍쐬
		hr = device->CreateShaderResourceView(this->init_particle_data_buffer.Get(), &DescSRV, this->init_particle_data_bufferSRV.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �p�[�e�B�N���f�[�^�̐ݒ�
	{
		struct ParticleData* data = new ParticleData[PERTICLES_PIECE_NO];
		for (int i = 0; i < PERTICLES_PIECE_NO; ++i) {

			data[i].pos = DirectX::XMFLOAT3(0, 0, 0);      // �ʒu
			data[i].w = 0.0f;							   // �摜�̍���
			data[i].h = 0.0f;							   // �摜��
			data[i].scale = DirectX::XMFLOAT3(0, 0, 0);	   // �g�嗦
			data[i].f_scale = DirectX::XMFLOAT3(0, 0, 0);  // �g�嗦(�J�n)
			data[i].e_scale = DirectX::XMFLOAT3(0, 0, 0);  // �g�嗦(�I��)
			data[i].v = DirectX::XMFLOAT3(0, 0, 0);        // ���x
			data[i].a = DirectX::XMFLOAT3(0, 0, 0); 	   // �����x
			data[i].alpha = 0;							   // �����x
			data[i].timer_max = 0;						   // ��������(�ő�)
			data[i].timer = 0;							   // ��������
			data[i].rot = 0.0f;							   // �p�x
			data[i].type = 0.0f;						   // 
		}


		// �p�[�e�B�N���f�[�^�̐ݒ�
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(ParticleData); // �o�b�t�@ �T�C�Y
		Desc.Usage = D3D11_USAGE_DEFAULT;//�X�e�[�W�̓��o�͂�OK�BGPU�̓��o��OK�B
		Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\�����o�b�t�@
		Desc.StructureByteStride = sizeof(ParticleData);

		D3D11_SUBRESOURCE_DATA SubResource;//�T�u���\�[�X�̏������p�f�[�^���`
		SubResource.pSysMem = data;
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// �ŏ��̓��̓��\�[�X(�f�[�^������������)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_data_buffer[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �ŏ��̏o�̓��\�[�X�i�������p�f�[�^�͕K�v�Ȃ��j
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_data_buffer[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		delete[] data;
	}

	// ���͗p���\�[�X�r���[�̐ݒ�
	{
		// ���̓��[�N���\�[�X �r���[�̐ݒ�i���͗p�j
		D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
		ZeroMemory(&DescSRV, sizeof(DescSRV));
		DescSRV.Format = DXGI_FORMAT_UNKNOWN;
		DescSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

		DescSRV.Buffer.ElementWidth = PERTICLES_PIECE_NO; // �f�[�^��

		// �V�F�[�_ ���\�[�X �r���[�̍쐬
		hr = device->CreateShaderResourceView(this->particle_data_buffer[0].Get(), &DescSRV, this->particle_data_bufferSRV[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		hr = device->CreateShaderResourceView(this->particle_data_buffer[1].Get(), &DescSRV, this->particle_data_bufferSRV[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �o�͗p�A���I�[�_�[�h�E�A�N�Z�X�E�r���[�i�o�͗p�j
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
		ZeroMemory(&DescUAV, sizeof(DescUAV));
		DescUAV.Format = DXGI_FORMAT_UNKNOWN;
		DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		DescUAV.Buffer.NumElements = PERTICLES_PIECE_NO; // �f�[�^��

		// �A���I�[�_�[�h�E�A�N�Z�X�E�r���[�̍쐬
		hr = device->CreateUnorderedAccessView(this->particle_data_buffer[0].Get(), &DescUAV, this->particle_data_bufferUAV[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		hr = device->CreateUnorderedAccessView(this->particle_data_buffer[1].Get(), &DescUAV, this->particle_data_bufferUAV[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

ParticleSystem::~ParticleSystem()
{
	delete[] this->datas;
	delete[] this->v;
}

float f_lerp(float a,float b,float t)
{
	return a + t * (b - a);
}

float EaseOutQuad(float min, float max, float t) 
{
	// t��0.0����1.0�͈̔͂ɃN�����v����
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;

	// �C�[�W���O�v�Z (EaseOutQuad)
	float easedValue = 1 - (1 - t) * (1 - t);

	// �ŏ��l�ƍő�l�͈̔͂ɃX�P�[��
	return min + (max - min) * easedValue;
}

void ParticleSystem::Update()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11DeviceContext* immediate_context = graphics->GetDeviceContext();

	immediate_context->CSSetShader(this->compute_shader.Get(), 0, 0);

	// �������p�p�����[�^���X�V
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		HRESULT hr = immediate_context->Map(this->init_particle_data_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		memcpy_s(mappedResource.pData, sizeof(ParticleData) * PERTICLES_DISPATCH_NO,
			this->datas, sizeof(ParticleData) * this->num_particles);
		immediate_context->Unmap(this->init_particle_data_buffer.Get(), 0);

		for (int i = 0; i < this->num_particles; ++i)
		{
			this->datas[i].type = -1;
		}
	}

	// �A���I�[�_�[�h�E�A�N�Z�X�E�r���[�̐ݒ�
	immediate_context->CSSetUnorderedAccessViews(0, 1, this->particle_data_bufferUAV[chainUAV].GetAddressOf(), NULL);

	//�������ݗp���[�N���\�[�X �r���[�̐ݒ�
	immediate_context->CSSetShaderResources(0, 1, this->particle_data_bufferSRV[chainSRV].GetAddressOf());
	
	// �������p���\�[�X�r���[�̐ݒ�
	immediate_context->CSSetShaderResources(1, 1, this->init_particle_data_bufferSRV.GetAddressOf());

	// �R���s���[�g�E�V�F�[�_�̎��s
	immediate_context->Dispatch(PERTICLES_DISPATCH_NO, 1, 1);//�O���[�v�̐�

	// �o�b�t�@�̐؂�ւ�
	{
		this->chainSRV = this->chainSRV ? 0 : 1;//�o�b�t�@�[�̐؂�ւ�
		this->chainUAV = this->chainUAV ? 0 : 1;//�o�b�t�@�[�̐؂�ւ�
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
		if (this->datas[i].type < 0) continue;

		this->v[n].position.x = this->datas[i].pos.x;
		this->v[n].position.y = this->datas[i].pos.y;
		this->v[n].position.z = this->datas[i].pos.z;
		this->v[n].texture_size.x = this->datas[i].w;
		this->v[n].texture_size.y = this->datas[i].h;
		this->v[n].param.rot = this->datas[i].rot;
		this->v[n].param.scale.x = this->datas[i].scale.x;
		this->v[n].param.scale.y = this->datas[i].scale.y;
		this->v[n].color.x = this->v[n].color.y = this->v[n].color.z = 1.0f;
		this->v[n].color.w = this->datas[i].alpha;
		++n;
	}
	//	���_�f�[�^�X�V
	immediate_context->UpdateSubresource(this->vertex_buffer.Get(), 0, nullptr, v, 0, 0);

	// ���_�V�F�[�_�[�Ƀp�[�e�B�N����񑗂�
	immediate_context->VSSetShaderResources(0, 1, this->particle_data_bufferSRV[chainSRV].GetAddressOf());

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
	int timer,
	DirectX::XMFLOAT3 p,
	DirectX::XMFLOAT3 v,
	DirectX::XMFLOAT3 f,
	DirectX::XMFLOAT2 tx,
	DirectX::XMFLOAT2 f_scale,
	DirectX::XMFLOAT2 e_scale,
	float rot
)
{
	for (int i = 0; i < num_particles; i++)
	{
		this->datas[i].pos.x = p.x;
		this->datas[i].pos.y = p.y;
		this->datas[i].pos.z = p.z;
		this->datas[i].v.x = v.x;
		this->datas[i].v.y = v.y;
		this->datas[i].v.z = v.z;
		this->datas[i].a.x = f.x;
		this->datas[i].a.y = f.y;
		this->datas[i].a.z = f.z;
		this->datas[i].w = tx.x;
		this->datas[i].h = tx.y;
		this->datas[i].f_scale.x = f_scale.x;
		this->datas[i].f_scale.y = f_scale.y;
		this->datas[i].e_scale.x = e_scale.x;
		this->datas[i].e_scale.y = e_scale.y;
		this->datas[i].alpha = 1.0f;
		this->datas[i].timer_max = timer;
		this->datas[i].timer = timer;
		this->datas[i].rot = rot;
		this->datas[i].type = 1;
		break;
	}
}
