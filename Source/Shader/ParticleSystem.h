#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

#include <optional>
#include <memory>
#include <vector>

#include "System\ClassBase\Singleton.h"
#include "Texture/Texture.h"
#include "shader.h"

class ParticleSystem : public Singleton<ParticleSystem>
{
public:
	// �G�t�F�N�g�̃p�����[�^
	// Play�֐��̈����Ŏg�p����
	struct ParticleParam
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
		int type;       // �G�t�F�N�g�̎��
		float rot;      // �p�x
	};

private:
	// CPU�ŋ��L���Ȃ��f�[�^
	struct InputGp
	{
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT3 position;// �`��ʒu
		DirectX::XMFLOAT2 scale;   // �g�嗦
		float rot;      // �p�x
		float alpha;    // �����x
		float timer;    // ��������(�ő�l�͒萔�Ŏ���)
	};

	// CPU�ŋ��L����f�[�^
	struct CPUGPUBuffer
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
		float rot;      // �p�x
		int type;       // �G�t�F�N�g�̎��
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
		float elapsed_time;					// �o�ߎ���
	};

public:
	ParticleSystem();

	~ParticleSystem();
	// �R���s���[�g�V�F�[�_�[�̍X�V
	void Update();

	void Render();

	// �e�N�X�`���̓ǂݍ���
	void LoadTexture(const char* filename);

	// �󂢂Ă���p�[�e�B�N���̐����v�Z����
	int CalculateFreeParticleCount();

	/**
	 * �G�t�F�N�g�Đ�
	 * 
	 * \param p �����ʒu
	 * \param c	�F
	 * \param timer ��������
	 * \param rot �p�x
	 */
	void PlayEffect(
		DirectX::XMFLOAT3 p,
		DirectX::XMFLOAT3 c,
		int type,
		float rot
	);

	/**
	 * �O���[�v�G�t�F�N�g�Đ�
	 * 
	 * \param efect_pool �Đ�����p�[�e�B�N�������i�[�����R���e�i
	 */
	void PlayGroupEffect(const std::vector<ParticleParam>& particle_pool);

	// �e��擾�E�ݒ�֐�
	void SetElapsedTime(float time) { this->particle_data.elapsed_time = time; }
	size_t GetFreeParticleCount()const { return this->free_particle_count; }
private:
	std::vector<CPUGPUBuffer> particle_data_pool;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> compute_shader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particle_gpu_data_buffer[2] = { NULL,NULL };				// GPU��p�̃p�[�e�B�N�����������o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  particle_gpu_data_SRV[2] = { NULL, NULL };	// GPU��p�̃p�[�e�B�N���������V�F�[�_ ���\�[�X �r���[(�ǂݍ���)
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particle_gpu_data_UAV[2] = { NULL, NULL };	// GPU��p�̃p�[�e�B�N���������A���I�[�_�[�h �A�N�Z�X �r���[(��������)

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
	size_t free_particle_count = 0;	// �󂢂Ă���p�[�e�B�N���̐�

	ParticleCommonConstant particle_data{};
#ifdef _DEBUG
public:
	void DebugDrawGUI();

#endif // _DEBUG
};
