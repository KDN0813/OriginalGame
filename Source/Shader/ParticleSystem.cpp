#include "ParticleSystem.h"
#include "../Hlsl/ParticleDisp.h"
#include "Shader/ShaderLoader.h"
#include "Graphics/Graphics.h"
#include "Camera/CameraManager.h"
#include "System/Misc.h"

#include "Component/CameraComponent.h"

ParticleSystem::ParticleSystem(const char* filename)
	: texture(std::make_unique<Texture>(filename))
	, particle_data_pool()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11Device* device = graphics->GetDevice();

	HRESULT hr;

	// ���͗p�p�[�e�B�N�����̏����ݒ�
	CPUGPUBuffer initial_value{};	// �������p�̒l�ݒ�
	particle_data_pool.resize(PERTICLES_PIECE_NO, initial_value);

	//	�萔�o�b�t�@����
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0; 
	{
		buffer_desc.ByteWidth = sizeof(ParticleCommonConstant);
		hr = device->CreateBuffer(&buffer_desc, nullptr, this->particle_common_constant.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	{
		buffer_desc.ByteWidth = sizeof(SceneConstantsBuffer);
		hr = device->CreateBuffer(&buffer_desc, nullptr, this->scene_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	hr = CreateShader::VsFromCso(device, "Shader\\GeometryParticle_vs.cso", this->vertex_shader.ReleaseAndGetAddressOf(), nullptr, nullptr, 0);
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
		struct CPUGPUBuffer* data = new CPUGPUBuffer[PERTICLES_PIECE_NO];
		for (int i = 0; i < PERTICLES_PIECE_NO; ++i) {

			data[i].rot = 0.0f;
			data[i].step = 0;
			data[i].is_busy = 0;
		}

		// ���\�[�X�̐ݒ�
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(CPUGPUBuffer); // �o�b�t�@ �T�C�Y
		Desc.Usage = D3D11_USAGE_DYNAMIC;//�X�e�[�W�̓��o�͂�OK�BGPU�̓��o��OK�B
		Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//UNORDERED�̃_�C�i�~�b�N�̓_���������B
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\�����o�b�t�@
		Desc.StructureByteStride = sizeof(CPUGPUBuffer);
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // CPU���珑������
		D3D11_SUBRESOURCE_DATA SubResource;//�T�u���\�[�X�̏������p�f�[�^���`
		SubResource.pSysMem = data;
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// �ŏ��̓��̓��\�[�X(�f�[�^������������)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_init_buffer.GetAddressOf());
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
		hr = device->CreateShaderResourceView(this->particle_init_buffer.Get(), &DescSRV, this->particle_init_SRV.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �p�[�e�B�N���f�[�^�̐ݒ�
	{
		// �p�[�e�B�N���f�[�^�̐ݒ�
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(CPUGPUBuffer); // �o�b�t�@ �T�C�Y
		Desc.Usage = D3D11_USAGE_DEFAULT;//�X�e�[�W�̓��o�͂�OK�BGPU�̓��o��OK�B
		Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\�����o�b�t�@
		Desc.StructureByteStride = sizeof(CPUGPUBuffer);

		D3D11_SUBRESOURCE_DATA SubResource;//�T�u���\�[�X�̏������p�f�[�^���`
		SubResource.pSysMem = particle_data_pool.data();
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// �ŏ��̓��̓��\�[�X(�f�[�^������������)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_data_buffer[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �ŏ��̏o�̓��\�[�X�i�������p�f�[�^�͕K�v�Ȃ��j
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_data_buffer[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
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
		hr = device->CreateShaderResourceView(this->particle_data_buffer[0].Get(), &DescSRV, this->particle_data_SRV[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		hr = device->CreateShaderResourceView(this->particle_data_buffer[1].Get(), &DescSRV, this->particle_data_SRV[1].GetAddressOf());
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
		hr = device->CreateUnorderedAccessView(this->particle_data_buffer[0].Get(), &DescUAV, this->particle_data_UAV[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		hr = device->CreateUnorderedAccessView(this->particle_data_buffer[1].Get(), &DescUAV, this->particle_data_UAV[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �X�e�[�W���O�o�b�t�@(GPU�o�̓f�[�^��CPU�ň������߂̃o�b�t�@)
	{
		// ���[�h�o�b�N�p�o�b�t�@ ���\�[�X�̍쐬
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(CPUGPUBuffer);	// �o�b�t�@ �T�C�Y
		Desc.Usage = D3D11_USAGE_STAGING;  // CPU����ǂݏ����\�ȃ��\�[�X
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU����ǂݍ���
		Desc.StructureByteStride = sizeof(CPUGPUBuffer);//�R���s���[�g�V�F�[�_�[�ō\���̂������ꍇ�K�v
		
		// �X�e�[�W���O�o�b�t�@�̍쐬
		hr = device->CreateBuffer(&Desc, NULL, this->staging_buffer.GetAddressOf());
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

	// �������p�p�����[�^���X�V
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		HRESULT hr = immediate_context->Map(this->particle_init_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		memcpy_s(mappedResource.pData, sizeof(CPUGPUBuffer) * PERTICLES_PIECE_NO,
			this->particle_data_pool.data(), sizeof(CPUGPUBuffer) * PERTICLES_PIECE_NO);
		immediate_context->Unmap(this->particle_init_buffer.Get(), 0);
	}

	// �A���I�[�_�[�h�E�A�N�Z�X�E�r���[�̐ݒ�
	immediate_context->CSSetUnorderedAccessViews(0, 1, this->particle_data_UAV[chainUAV].GetAddressOf(), NULL);

	//�������ݗp���[�N���\�[�X �r���[�̐ݒ�
	immediate_context->CSSetShaderResources(0, 1, this->particle_data_SRV[chainSRV].GetAddressOf());
	
	// �������p���\�[�X�r���[�̐ݒ�
	immediate_context->CSSetShaderResources(1, 1, this->particle_init_SRV.GetAddressOf());

	// �R���s���[�g�E�V�F�[�_�̎��s
	immediate_context->Dispatch(PERTICLES_PIECE_NO, 1, 1);//�O���[�v�̐�

	// �o�̓o�b�t�@��CPU�ň�����悤�ɂ���
	{
		HRESULT hr = S_OK;

		immediate_context->CopyResource(this->staging_buffer.Get(), this->particle_data_buffer[this->chainUAV].Get());
		// ���ʂ�CPU����ǂݍ���
		//���[�h�o�b�t�@�ǂݍ��ׂ݂̈̃��b�N
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		hr = immediate_context->Map(this->staging_buffer.Get(), 0, D3D11_MAP_READ, 0, &mapped_resource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		CPUGPUBuffer* pd = (CPUGPUBuffer*)mapped_resource.pData;

		this->particle_data_pool.assign(pd, &pd[PERTICLES_PIECE_NO]);

		//���[�h�o�b�t�@�̃��b�N����
		immediate_context->Unmap(this->staging_buffer.Get(), 0);
	}

	// ���\�[�X�r���[�̉���
	{
		ID3D11UnorderedAccessView* view_null_uav = nullptr;
		immediate_context->CSSetUnorderedAccessViews(0, 1, &view_null_uav, NULL);
		ID3D11ShaderResourceView* view_null_srv = nullptr;
		immediate_context->CSSetShaderResources(0, 1, &view_null_srv);
		immediate_context->CSSetShaderResources(1, 1, &view_null_srv);
	}

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
	ParticleCommonConstant pcc;
	pcc.default_size = { 0.340f, 1.28f };
	pcc.timer_max = 100.0f;
	pcc.f_scale = DirectX::XMFLOAT2(2.0f, 1.0f);
	pcc.e_scale = DirectX::XMFLOAT2(1.0f, 3.5f);
	immediate_context->UpdateSubresource(this->particle_common_constant.Get(), 0, nullptr, &pcc, 0, 0);
	immediate_context->VSSetConstantBuffers(1, 1, this->particle_common_constant.GetAddressOf());
	immediate_context->GSSetConstantBuffers(1, 1, this->particle_common_constant.GetAddressOf());
	immediate_context->PSSetConstantBuffers(1, 1, this->particle_common_constant.GetAddressOf());

	//	�_�`��ݒ�
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//	�V�F�[�_�[�ݒ�
	immediate_context->VSSetShader(this->vertex_shader.Get(), nullptr, 0);
	immediate_context->GSSetShader(this->geometry_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(this->pixel_shader.Get(), nullptr, 0);

	//	�e�N�X�`���ݒ�
	immediate_context->PSSetShaderResources(0, 1, this->texture->GetAddressOf());

	// ���_�V�F�[�_�[�Ƀp�[�e�B�N����񑗂�
	immediate_context->VSSetShaderResources(0, 1, this->particle_data_SRV[chainSRV].GetAddressOf());

	//	�p�[�e�B�N����񕪕`��R�[��
	immediate_context->Draw(PERTICLES_PIECE_NO, 0);

	//	�V�F�[�_������
	immediate_context->VSSetShader(nullptr, nullptr, 0);
	immediate_context->GSSetShader(nullptr, nullptr, 0);
	immediate_context->PSSetShader(nullptr, nullptr, 0);
}

void ParticleSystem::Set(
	DirectX::XMFLOAT3 p,
	float rot
)
{
	for (size_t i = 0 ; i < this->particle_data_pool.size(); ++i)
	{
		if ( this->particle_data_pool[i].is_busy) continue;

		CPUGPUBuffer particle_data
		{
			p,
			rot,
			0,	// step
			1,	// is_busy
		};
		this->particle_data_pool[i] = particle_data;
	}
}
