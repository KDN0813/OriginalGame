#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

#include <vector>
#include <memory>

#include "Texture/Texture.h"
#include "shader.h"

class ParticleSystem
{
private:
	struct ParticleParam
	{
		float rot;							// ��]�p�x
		DirectX::XMFLOAT2 scale;			// �g�嗦
	};

	struct ParticleData
	{
		DirectX::XMFLOAT3 pos;			// �`��ʒu
		float w, h;						// �摜�T�C�Y
		DirectX::XMFLOAT2 scale;		// �g�嗦
		DirectX::XMFLOAT2 f_scale;		// �g�嗦(�J�n)
		DirectX::XMFLOAT2 e_scale;		// �g�嗦(�I��)
		DirectX::XMFLOAT3 v;			// �ړ����x
		DirectX::XMFLOAT3 a;			// �����x
		float alpha;					// �����x
		int timer_max;				// ��������(�ő�l)
		int timer;					// ��������
		float rot;						// �p�x
		float type;
	};
	// �V�[���萔
	struct SceneConstantsBuffer
	{
		DirectX::XMFLOAT4X4 view_projection;
		DirectX::XMFLOAT4X4 view_matrix;
		DirectX::XMFLOAT4X4 projection_matrix;
	};
	// �t���[�����̒萔
	struct ForPerFrameConstantBuffer
	{
		DirectX::XMFLOAT2 size;
		DirectX::XMFLOAT2 dummy;
	};

public:
	ParticleSystem() = delete;
	ParticleSystem(const char* filename, int num = 1);

	~ParticleSystem();
	// �R���s���[�g�V�F�[�_�[�̍X�V
	void Update();

	void Render();

	/**
	 * .
	 * 
	 * \param timer ��������
	 * \param p �����ʒu
	 * \param v �ړ����x
	 * \param f �����x
	 * \param rot �p�x
	 * \param tx �摜�T�C�Y
	 * \param scale �g�嗦
	 */
	void Set(
		int timer,
		DirectX::XMFLOAT3 p,
		DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT2 tx = DirectX::XMFLOAT2(1.0f, 1.0f),
		DirectX::XMFLOAT2 f_scale = DirectX::XMFLOAT2(1.0f, 1.0f),
		DirectX::XMFLOAT2 e_scale = DirectX::XMFLOAT2(1.0f, 1.0f),
		float rot = 0.0f
	);

private:
	ParticleData* datas;		//	�p�[�e�B�N�����
	int num_particles = 0;	//	�p�[�e�B�N����

	Microsoft::WRL::ComPtr<ID3D11Buffer> for_per_frame_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> compute_shader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_data_buffer[2] = { NULL,NULL };
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particle_data_bufferUAV[2] = { NULL, NULL }; // �A���I�[�_�[�h �A�N�Z�X �r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_data_bufferSRV[2] = { NULL, NULL }; // �V�F�[�_ ���\�[�X �r���[

	Microsoft::WRL::ComPtr<ID3D11Buffer> init_particle_data_buffer = NULL; // �������p�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  init_particle_data_bufferSRV = NULL; // �������\���̃o�b�t�@

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11Buffer> scene_constant_buffer;
	std::unique_ptr<Texture> texture;

	int chainSRV = 0;//�o�b�t�@�[�̐؂�ւ�
	int chainUAV = 1;//�o�b�t�@�[�̐؂�ւ�
};
