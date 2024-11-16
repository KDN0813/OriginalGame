#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

#include <optional>
#include <memory>
#include <vector>

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

	// CPU�ŋ��L���Ȃ��f�[�^
	struct InputGp
	{
		DirectX::XMFLOAT3 position;// �`��ʒu
		DirectX::XMFLOAT2 scale;   // �g�嗦
		float alpha;    // �����x
		float timer;    // ��������(�ő�l�͒萔�Ŏ���)
	};

	// CPU�ŋ��L����f�[�^
	struct CPUGPUBuffer
	{
		DirectX::XMFLOAT3 position;
		float rot;      // �p�x
		int step;
		int is_busy;    // �v�f���ғ����ł��邩
	};

	// �V�[���萔
	struct SceneConstantsBuffer
	{
		DirectX::XMFLOAT4X4 view_projection;
		DirectX::XMFLOAT4X4 view_matrix;
		DirectX::XMFLOAT4X4 projection_matrix;
	};
	// �p�[�e�B�N���ʂ̒萔
	struct ParticleCommonConstant
	{
		DirectX::XMFLOAT2 default_size;		// �摜�T�C�Y
		DirectX::XMFLOAT2 f_scale;			// �g�嗦(�J�n)
		DirectX::XMFLOAT2 e_scale;			// �g�嗦(�I��)
		float timer_max;					// ��������
		float dummy;
	};

public:
	ParticleSystem() = delete;
	ParticleSystem(const char* filename);

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
		DirectX::XMFLOAT3 p,
		float rot = 0.0f
	);

private:
	std::vector<CPUGPUBuffer> particle_data_pool;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> compute_shader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_data_buffer[2] = { NULL,NULL };
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_data_SRV[2] = { NULL, NULL }; // �V�F�[�_ ���\�[�X �r���[(�ǂݍ���)
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particle_data_UAV[2] = { NULL, NULL }; // �A���I�[�_�[�h �A�N�Z�X �r���[(��������)

	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_init_buffer = NULL;			// �������o�b�t�@ CPU�ŊǗ������f�[�^(CPUGPUBuffer)�����ɍX�V���Ă���o�b�t�@�B�X�V��CS�ɑ���O�ɖ��񂵂Ă���Bparticle_init_SRV�̌��ɂȂ����o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_init_SRV = NULL; // ���������\�[�X�r���[�@particle_init_buffer�����ɍ��ꂽ

	Microsoft::WRL::ComPtr<ID3D11Buffer>  particle_to_cpu_buffer = NULL;			// CPU�ւ̏������ݗp�o�b�t�@ ���\�[�X�istaging�̎�O�jparticle_to_cpu_UAV�쐬�Ɏg��ꂽ�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>  particle_to_cpu_UAV = NULL;	// �A���I�[�_�[�h �A�N�Z�X �r���[
	Microsoft::WRL::ComPtr<ID3D11Buffer> staging_buffer;							// GPU�ł̏o�̓o�b�t�@��CPU�ň������߂̃o�b�t�@

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11Buffer> scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_common_constant;
	std::unique_ptr<Texture> texture;

	int chainSRV = 0;//�o�b�t�@�[�̐؂�ւ�
	int chainUAV = 1;//�o�b�t�@�[�̐؂�ւ�
};
