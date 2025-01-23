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

	// ���͗p�p�[�e�B�N�����̏����ݒ�
	CPUGPUBuffer initial_value{};	// �������p�̒l�ݒ�
	particle_data_pool.resize(PERTICLES_PIECE_NO, initial_value);

	//	�萔�o�b�t�@����
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		{
			// �p�[�e�B�N���f�[�^�̏�����
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

	// ���_�V�F�[�_�[
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

	// �T���v���X�e�[�g�̐���
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

	// GPU��p�̃p�[�e�B�N���f�[�^�������o�b�t�@�̍쐬
	{
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(InputGp); // �o�b�t�@ �T�C�Y
		Desc.Usage = D3D11_USAGE_DEFAULT;//�X�e�[�W�̓��o�͂�OK�BGPU�̓��o��OK�B
		Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\�����o�b�t�@
		Desc.StructureByteStride = sizeof(InputGp);

		InputGp* input_gp = new InputGp[PERTICLES_PIECE_NO];
		for (int i = 0; i < PERTICLES_PIECE_NO;++i)
		{
			input_gp[i].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			input_gp[i].scale = DirectX::XMFLOAT2(0.0f, 0.0f);
			input_gp[i].alpha = 0.0f;
			input_gp[i].timer = 0;
		}

		D3D11_SUBRESOURCE_DATA SubResource{};//�T�u���\�[�X�̏������p�f�[�^���`
		SubResource.pSysMem = input_gp;
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// �ŏ��̓��̓��\�[�X(�f�[�^������������)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_gpu_data_buffer[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �ŏ��̏o�̓��\�[�X�i�������p�f�[�^�͕K�v�Ȃ��j
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_gpu_data_buffer[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		delete[] input_gp;
	}

	// GPU��p�̃p�[�e�B�N���f�[�^���������͗p���\�[�X�r���[�̍쐬
	{
		// ���̓��[�N���\�[�X �r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
		ZeroMemory(&DescSRV, sizeof(DescSRV));
		DescSRV.Format = DXGI_FORMAT_UNKNOWN;
		DescSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

		DescSRV.Buffer.ElementWidth = PERTICLES_PIECE_NO; // �f�[�^��

		// �V�F�[�_ ���\�[�X �r���[�̍쐬
		hr = device->CreateShaderResourceView(this->particle_gpu_data_buffer[0].Get(), &DescSRV, this->particle_gpu_data_SRV[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		hr = device->CreateShaderResourceView(this->particle_gpu_data_buffer[1].Get(), &DescSRV, this->particle_gpu_data_SRV[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// GPU��p�̃p�[�e�B�N���f�[�^�������o�͗p���\�[�X�r���[�̍쐬
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
		ZeroMemory(&DescUAV, sizeof(DescUAV));
		DescUAV.Format = DXGI_FORMAT_UNKNOWN;
		DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		DescUAV.Buffer.NumElements = PERTICLES_PIECE_NO; // �f�[�^��

		// �A���I�[�_�[�h�E�A�N�Z�X�E�r���[�̍쐬
		hr = device->CreateUnorderedAccessView(this->particle_gpu_data_buffer[0].Get(), &DescUAV, this->particle_gpu_data_UAV[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		hr = device->CreateUnorderedAccessView(this->particle_gpu_data_buffer[1].Get(), &DescUAV, this->particle_gpu_data_UAV[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �������p�o�b�t�@�̍쐬
	{
		// ���\�[�X�̐ݒ�
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(CPUGPUBuffer); // �o�b�t�@ �T�C�Y
		Desc.Usage = D3D11_USAGE_DYNAMIC;//�X�e�[�W�̓��o�͂�OK�BGPU�̓��o��OK�B
		Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//UNORDERED�̃_�C�i�~�b�N�̓_���������B
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\�����o�b�t�@
		Desc.StructureByteStride = sizeof(CPUGPUBuffer);
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // CPU���珑������
		
		D3D11_SUBRESOURCE_DATA SubResource{};//�T�u���\�[�X�̏������p�f�[�^���`
		SubResource.pSysMem = particle_data_pool.data();
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// �ŏ��̓��̓��\�[�X(�f�[�^������������)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_init_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �������p���͗p���\�[�X�r���[�̍쐬
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

	// CPU�ւ̏������ݗp�o�b�t�@�̍쐬
	{
		// ���\�[�X�̐ݒ�
		D3D11_BUFFER_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.ByteWidth = PERTICLES_PIECE_NO * sizeof(CPUGPUBuffer); // �o�b�t�@ �T�C�Y
		Desc.Usage = D3D11_USAGE_DEFAULT;//�X�e�[�W�̓��o�͂�OK�BGPU�̓��o��OK�B
		Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\�����o�b�t�@
		Desc.StructureByteStride = sizeof(CPUGPUBuffer);

		D3D11_SUBRESOURCE_DATA SubResource{};//�������T�u���\�[�X�̏������p�f�[�^���`
		SubResource.pSysMem = this->particle_data_pool.data();
		SubResource.SysMemPitch = 0;
		SubResource.SysMemSlicePitch = 0;

		// �ŏ��̓��̓��\�[�X(�f�[�^������������)
		hr = device->CreateBuffer(&Desc, &SubResource, this->particle_to_cpu_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// CPU�ւ̏������ݗp���\�[�X�r���[�̍쐬
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
		ZeroMemory(&DescUAV, sizeof(DescUAV));
		DescUAV.Format = DXGI_FORMAT_UNKNOWN;
		DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		DescUAV.Buffer.NumElements = PERTICLES_PIECE_NO; // �f�[�^��

		// �A���I�[�_�[�h�E�A�N�Z�X�E�r���[�̍쐬
		hr = device->CreateUnorderedAccessView(this->particle_to_cpu_buffer.Get(), &DescUAV, this->particle_to_cpu_UAV.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �G�t�F�N�g�쐬
	{
		// �a���G�t�F�N�g
		{
			CPUGPUBuffer effect{};
			effect.initial_position = {};						// �����ʒu
			effect.forward = {};								// �ړ�����
			effect.velocity = {};								// �ړ����x
			effect.acceleration = {};							// �����x
			effect.initial_scale = {};							// �����g�嗦
			effect.f_scale = DirectX::XMFLOAT2(0.02f, 0.01f);		// �g�嗦(��ԊJ�n)
			effect.e_scale = DirectX::XMFLOAT2(0.01f, 0.035f);		// �g�嗦(��ԏI��)
			effect.color = DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f);	// �F
			effect.rot = {};									// �p�x
			effect.rot_speed = {};								// ��]���x
			effect.initial_lifetime = 0.8f;						// ��������
			effect.type = EFFECT_SLASH;							// �G�t�F�N�g�^�C�v
			effect.step = 0;									// step
			effect.is_busy = 1;									// �ғ��t���O

			this->effect_slash.emplace_back(effect);
		}

		// �q�b�g�G�t�F�N�g
		{
			DirectX::XMFLOAT3 pos{};
			float rot{};
			const size_t particle_max = 10;	// �q�b�g�G�t�F�N�g�̐��̐�
			const float step = (-DirectX::XM_PI - DirectX::XM_PI) / static_cast<float>(particle_max);	// �e��Ԃ̊Ԋu
			const float moveXZ = 1.7f;	// �ړ���(��)
			const float moveY = 4.0f;	// �ړ���(��)
			const float initial_lifetime = 0.8f;
			const float velocityXZ = moveXZ / initial_lifetime;
			const float velocityY = moveY / initial_lifetime;
			const float accelerationY = -velocityY / initial_lifetime * 1.5f;
			for (size_t i = 0; i < particle_max; ++i)
			{
				CPUGPUBuffer effect{};
				effect.initial_position = pos;						// �����ʒu
				effect.forward = {};								// �O������
				// �ړ����x
				effect.velocity = 
				{
					sinf(step* i)* velocityXZ,
					velocityY,
					cosf(step* i)* velocityXZ
				};								
				effect.acceleration = { 0.0f ,accelerationY ,0.0f };   // �����x
				effect.initial_scale = DirectX::XMFLOAT2(0.03f, 0.03f);// �����g�嗦
				effect.f_scale = {};		// �g�嗦(��ԊJ�n)
				effect.e_scale = {};		// �g�嗦(��ԏI��)
				effect.color = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);	// �F
				effect.f_color = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);	// �F
				effect.e_color = DirectX::XMFLOAT3(0.74f, 0.32f, 0.0f);	// �F
				effect.rot = rot;									// �p�x
				effect.rot_speed = DirectX::XMConvertToRadians(360.0f);								// ��]���x
				effect.initial_lifetime = initial_lifetime;			// ��������
				effect.type = EFFECT_HIT;							// �G�t�F�N�g�^�C�v
				effect.step = 0;									// step
				effect.is_busy = 1;									// �ғ��t���O
				this->effect_hit.emplace_back(effect);
			}
		}

		// �q�b�g�G�t�F�N�g2
		{
			DirectX::XMFLOAT3 pos{};
			float rot{};
			const size_t particle_max = 10;	// �q�b�g�G�t�F�N�g�̐��̐�
			const float step = (-DirectX::XM_PI - DirectX::XM_PI) / static_cast<float>(particle_max);	// �e��Ԃ̊Ԋu
			const float moveXZ = 1.7f;	// �ړ���(��)
			const float moveY = 4.0f;	// �ړ���(��)
			const float initial_lifetime = 0.8f;
			const float velocityXZ = moveXZ / initial_lifetime;
			const float velocityY = moveY / initial_lifetime;
			const float accelerationY = -velocityY / initial_lifetime * 1.5f;
			for (size_t i = 0; i < particle_max; ++i)
			{
				CPUGPUBuffer effect{};
				effect.initial_position = pos;						// �����ʒu
				effect.forward = {};								// �O������
				// �ړ����x
				effect.velocity =
				{
					sinf(step * i) * velocityXZ,
					velocityY,
					cosf(step * i) * velocityXZ
				};
				effect.acceleration = { 0.0f ,accelerationY ,0.0f };   // �����x
				effect.initial_scale = DirectX::XMFLOAT2(0.005f, 0.005f);// �����g�嗦
				effect.f_scale = {};		// �g�嗦(��ԊJ�n)
				effect.e_scale = {};		// �g�嗦(��ԏI��)
				effect.color = DirectX::XMFLOAT3(0.36f, 0.9f, 1.0f);	// �F
				effect.f_color = DirectX::XMFLOAT3(0.36f, 0.9f, 1.0f);	// �F
				effect.e_color = DirectX::XMFLOAT3(0.25f, 0.61f, 0.7f);	// �F
				effect.rot = rot;									// �p�x
				effect.rot_speed = DirectX::XMConvertToRadians(360.0f);								// ��]���x
				effect.initial_lifetime = initial_lifetime;			// ��������
				effect.type = EFFECT_HIT2;							// �G�t�F�N�g�^�C�v
				effect.step = 0;									// step
				effect.is_busy = 1;									// �ғ��t���O
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

	// �萔�o�b�t�@�̐ݒ�
	{
		immediate_context->UpdateSubresource(this->particle_common_constant.Get(), 0, nullptr, &this->particle_data, 0, 0);
		immediate_context->CSSetConstantBuffers(1, 1, this->particle_common_constant.GetAddressOf());
	}

	// �������p�p�����[�^���X�V
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		HRESULT hr = immediate_context->Map(this->particle_init_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		memcpy_s(mappedResource.pData, sizeof(CPUGPUBuffer) * PERTICLES_PIECE_NO,
			this->particle_data_pool.data(), sizeof(CPUGPUBuffer) * PERTICLES_PIECE_NO);
		immediate_context->Unmap(this->particle_init_buffer.Get(), 0);
	}

	// GPU��p�f�[�^���������̓��\�[�X�r���[�̐ݒ� 
	immediate_context->CSSetShaderResources(0, 1, this->particle_gpu_data_SRV[chainSRV].GetAddressOf());
	// CPU�EGPU���ʂ̃f�[�^���������̓��\�[�X�r���[�̐ݒ�
	immediate_context->CSSetShaderResources(1, 1, this->particle_init_SRV.GetAddressOf());	

	// GPU��p�f�[�^�������������݃��\�[�X�r���[�̐ݒ� 
	immediate_context->CSSetUnorderedAccessViews(0, 1, this->particle_gpu_data_UAV[chainUAV].GetAddressOf(), NULL);
	// CPU�EGPU���ʂ̃f�[�^�������o�̓��\�[�X�r���[�̐ݒ�
	immediate_context->CSSetUnorderedAccessViews(1, 1, this->particle_to_cpu_UAV.GetAddressOf(), NULL);

	// �R���s���[�g�E�V�F�[�_�̎��s
	immediate_context->Dispatch(PERTICLES_PIECE_NO, 1, 1);//�O���[�v�̐�

	{
		HRESULT hr = S_OK;

		immediate_context->CopyResource(this->staging_buffer.Get(), this->particle_to_cpu_buffer.Get());
		// ���ʂ�CPU����ǂݍ���
		// ���[�h�o�b�t�@�ǂݍ��ׂ݂̈̃��b�N
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		hr = immediate_context->Map(this->staging_buffer.Get(), 0, D3D11_MAP_READ, 0, &mapped_resource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		CPUGPUBuffer* cpgp_buffer = (CPUGPUBuffer*)mapped_resource.pData;

		this->particle_data_pool.assign(cpgp_buffer, &cpgp_buffer[PERTICLES_PIECE_NO]);

		//���[�h�o�b�t�@�̃��b�N����
		immediate_context->Unmap(this->staging_buffer.Get(), 0);

		// �󂢂Ă���p�[�e�B�N���̐����v�Z����
		CalculateFreeParticleCount();
	}

	// ���\�[�X�r���[�̉���
	{
		ID3D11UnorderedAccessView* view_null_uav = nullptr;
		immediate_context->CSSetUnorderedAccessViews(0, 1, &view_null_uav, NULL);
		immediate_context->CSSetUnorderedAccessViews(1, 1, &view_null_uav, NULL);
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
	// �T���v���[�X�e�[�g�̐ݒ�
	immediate_context->PSSetSamplers(0, 1, this->sampler_state.GetAddressOf());

	//�萔�o�b�t�@�̍X�V
	{
		// �V�[���萔�X�V�E�ݒ�
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

		// �p�[�e�B�N�����ʃf�[�^�̐ݒ�
		immediate_context->VSSetConstantBuffers(1, 1, this->particle_common_constant.GetAddressOf());
	}
	
	//	�_�`��ݒ�
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//	�V�F�[�_�[�ݒ�
	immediate_context->VSSetShader(this->vertex_shader.Get(), nullptr, 0);
	immediate_context->GSSetShader(this->geometry_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(this->pixel_shader.Get(), nullptr, 0);

	//	�e�N�X�`���ݒ�
	immediate_context->PSSetShaderResources(0, 1, this->texture->GetAddressOf());
	immediate_context->GSSetShaderResources(0, 1, this->texture->GetAddressOf());

	// ���_�V�F�[�_�[�Ƀp�[�e�B�N����񑗂�
	immediate_context->VSSetShaderResources(0, 1, this->particle_gpu_data_SRV[chainSRV].GetAddressOf());

	//	�p�[�e�B�N����񕪕`��R�[��
	immediate_context->Draw(PERTICLES_PIECE_NO, 0);

	//	�V�F�[�_������
	immediate_context->VSSetShader(nullptr, nullptr, 0);
	immediate_context->GSSetShader(nullptr, nullptr, 0);
	immediate_context->PSSetShader(nullptr, nullptr, 0);
}

void ParticleSystem::LoadTexture(const char* filename)
{
	_ASSERT_EXPR_W(!this->texture.get(), L"�e�N�X�`���͊��ɓǂݍ��܂�Ă��܂�");
	if (this->texture.get()) return;
	this->texture = std::make_unique<Texture>(filename);
}

int ParticleSystem::CalculateFreeParticleCount()
{
	this->free_particle_count = 0;	// �J�E���g�����Z�b�g

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
	case EFFECT_HIT:		// �a���G�t�F�N�g
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
	case EFFECT_HIT2:	// �����a���G�t�F�N�g
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
	case EFFECT_SLASH:	// �����a���G�t�F�N�g
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
		_ASSERT_EXPR_W(false, L"�G�t�F�N�g�^�C�v�����݂��܂���");
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
	if (this->free_particle_count < particle_pool.size()) return;	// �K�p���̋󂫂��Ȃ����return

	size_t count = 0;	// �ǉ������p�[�e�B�N���̐�
	for (size_t i = 0; i < this->particle_data_pool.size(); ++i)
	{
		if (this->particle_data_pool[i].is_busy) continue;	// �ғ����Ă������΂�

		this->particle_data_pool[i] = particle_pool[count]; // �f�[�^�ݒ�

		// �|�W�V�����ݒ�
		{
			// �e�̈ʒu�ɍ��킹��
			this->particle_data_pool[i].initial_position.x += parent_pos.x;
			this->particle_data_pool[i].initial_position.y += parent_pos.y;
			this->particle_data_pool[i].initial_position.z += parent_pos.z;
		}
		// �ړ������ݒ�
		{
			this->particle_data_pool[i].forward = forward;
		}
		// �p�x�ݒ�
		{
			this->particle_data_pool[i].rot += parent_rot;
		}
		// �F�ݒ�
		{
			this->particle_data_pool[i].color.x *= parent_color.x;
			this->particle_data_pool[i].color.y *= parent_color.y;
			this->particle_data_pool[i].color.z *= parent_color.z;
		}

		++count;

		if (particle_pool.size() <= count) break;
	}

	this->free_particle_count -= particle_pool.size();	// �󂫃p�[�e�B�N���̐������炷
}

#ifdef _DEBUG

void ParticleSystem::DebugDrawGUI()
{
	if(ImGui::Begin("ParticleSystem"))
	{
		// �󂢂Ă�p�[�e�B�N�����\��
		std::string text = "Free Particle Count: " + std::to_string(this->free_particle_count);
		ImGui::Text(text.c_str());

		// �ғ����̃p�[�e�B�N�����\��
		text = "Working Particle Count: " + std::to_string(PERTICLES_PIECE_NO  - this->free_particle_count);
		ImGui::Text(text.c_str());

		// �p�[�e�B�N���萔
		if (ImGui::TreeNodeEx("ParticleCommonConstant", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat("Elapsed Time", &particle_data.elapsed_time);
			ImGui::TreePop();
		}

		// �Đ�����G�t�F�N�g�̏��
		if (ImGui::TreeNodeEx("Effect Param"))
		{
			DebugDrawEffectParamGUI("SLASH", this->effect_slash);
			DebugDrawEffectParamGUI("EFFECT HIT", this->effect_hit);

			ImGui::TreePop();
		}

		// �eCPU�ő��삷��p�[�e�B�N���̏��
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