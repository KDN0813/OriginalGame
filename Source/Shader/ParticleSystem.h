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
		DirectX::XMFLOAT3 initial_position;
		DirectX::XMFLOAT3 color;
		float initial_lifetime; // ������������
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
		DirectX::XMFLOAT3 initial_position;	// �����ʒu
		DirectX::XMFLOAT2 initial_scale;	// �����g�嗦
		DirectX::XMFLOAT2 f_scale;			// �g�嗦(��ԊJ�n)
		DirectX::XMFLOAT2 e_scale;			// �g�嗦(��ԏI��)
		DirectX::XMFLOAT3 color;
		float rot;							// �p�x
		float initial_lifetime;				// ������������
		int type;							// �G�t�F�N�g�̎��
		int step;
		int is_busy;						// �v�f���ғ����ł��邩
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
		float elapsed_time;					// �o�ߎ���
		float dummy[3];
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
	 * \param type �����ʒu
	 * \param pos �����ʒu
	 * \param rot �p�x
	 * \param color �F
	 */
	void PlayEffect(
		int type,
		DirectX::XMFLOAT3 pos,
		float rot,
		DirectX::XMFLOAT3 color
	);

	// �e��擾�E�ݒ�֐�
	void SetElapsedTime(float time) { this->particle_data.elapsed_time = time; }
	size_t GetFreeParticleCount()const { return this->free_particle_count; }

	// �G�t�F�N�g���
	std::vector<CPUGPUBuffer> effect_slash;			// �a���G�t�F�N�g�̏����l
	std::vector<CPUGPUBuffer> effect_fall_slash;	// �����a���G�t�F�N�g�̏����l
private:
	/**
	 * �G�t�F�N�g�Đ�
	 * 
	 * \param particle_pool �p�[�e�B�N���̏��
	 */
	void PlayEffect(
		DirectX::XMFLOAT3 parent_pos,
		float parent_rot,
		DirectX::XMFLOAT3 parent_color,
		const std::vector<CPUGPUBuffer>& particle_pool
	);
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
	size_t debug_particle_index_max = 20;	// �f�o�b�O�p�̃p�[�e�B�N���̍ő吔
	bool draw_debug_play = false;

	ParticleCommonConstant particle_data{};
#ifdef _DEBUG
public:
	void DebugDrawGUI();

	void PlayDebugEffect();
#endif // _DEBUG
};
