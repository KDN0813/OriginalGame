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
		float rot;
	};
	struct Vertex
	{
		DirectX::XMFLOAT3 position;	//	�ʒu
		DirectX::XMFLOAT2 texcoord;	//	UV
		DirectX::XMFLOAT4 color;	//	���_�F		
		ParticleParam param;
	};

	struct ParticleData
	{
		float x, y, z;		// �ʒu
		float w, h;			// �摜�T�C�Y
		float vx, vy, vz;	// �ړ����x
		float ax, ay, az;	// �����x
		float alpha;		// �����x
		float timer;		// ��������
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
	ParticleSystem(const char* filename, int num = 1000);

	~ParticleSystem();
	void Update(float elapsed_time);

	void Render();

	/**
	 * .
	 * 
	 * \param timer ��������
	 * \param p �����ʒu
	 * \param v �ړ����x
	 * \param f �����x
	 * \param size �T�C�Y
	 */
	void Set(
		float timer,
		DirectX::XMFLOAT3 p,
		DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT2 size = DirectX::XMFLOAT2(1.0f, 1.0f)
	);

private:
	ParticleData* data;		//	�p�[�e�B�N�����
	Vertex* v;				//	���_�o�b�t�@�������ݏ��
	int num_particles = 0;	//	�p�[�e�B�N����

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> for_per_frame_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11Buffer> scene_constant_buffer;
	std::unique_ptr<Texture> texture;
};
