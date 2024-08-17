#pragma once
#include <d3d11.h>
#include "Shader/Shader.h"
#include "Model/InstancingModelResource.h"
#include "Model/ModelResource.h"

class InstancingModelShaderComponent;
class InstancingModelComponent;
class Transform3DComponent;

class InstancingModelShader : public Shader
{
public:
	static const int MaxBones = 128;
	static const int MAX_INSTANCES = 512;
private:
	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};
	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	materialColor;
	};
	// �C���X�^���V���O�`��Ŏg�p���鋤�ʂ̒萔
	struct CommonDataConstantBuffer
	{
		UINT bone_transform_count; // 1��̕`��Ŏg�p����{�[���g�����X�t�H�[���̐�
		DirectX::XMUINT3 dummy;
	};
	struct MeshConstantBuffer
	{
		UINT offset;				// �o�b�t�@���Ń��b�V���̊J�n�ʒu�������I�t�Z�b�g�l
		DirectX::XMUINT3 dummy;
	};
	/**
	* \brief GPU�ɑ���f�[�^
	*
	* \param animation_start_offset �o�b�t�@���Ŏg�p����A�j���[�V�����̊J�n�ʒu�������I�t�Z�b�g�l
	* \param anime_frame ���݂̃t���[��
	* \param world_transform ���[���h�g�����X�t�H�[��
	*/
	struct InstanceData
	{
		UINT animation_start_offset;
		UINT anime_frame;
		DirectX::XMFLOAT4X4 world_transform{};
	};
public:
	InstancingModelShader(ID3D11Device* device);
	~InstancingModelShader() override {}

	void Render(ID3D11DeviceContext* dc, const RenderContext& rc)override;
	
	const char* GetName()override { return "InstancingModelShader"; }

	// �C���X�^���V���O�`��J�n
	void InstancingStart();
	// �C���X�^���X�̒ǉ�
	void InstancingAdd(InstancingModelComponent* model, Transform3DComponent* transform);
	// �C���X�^���V���O�`��C��
	void InstancingEnd(ID3D11DeviceContext* dc, InstancingModelComponent* model);

	// �`�悷��object�̃V�F�[�_�[��ǉ�
	void AddShaderComponent(InstancingModelShaderComponent* shader_component);

	/**
	 * \fn InstancingModelShaderComponent
	 * \brief �V�F�[�_�[�R���|�[�l���g���L���ł��邩���肷��B
	 * �`��ɕK�p�ȃp�����[�^���������Ă��邩���肷��B
	 * 
	 * \param shader_component ���肷��V�F�[�_�[�R���|�[�l���g
	 * \return �L���Ȃ�����̃|�C���^�B�L���łȂ��Ȃ�nullptr��Ԃ�
	 */
	InstancingModelShaderComponent* IsShaderValid(InstancingModelShaderComponent* shader_component);
private:
	// �C���X�^���V���O�`��
	void InstancingRender(ID3D11DeviceContext* dc, InstancingModelComponent* model);
	void DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset);

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			common_data_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;

	// �C���X�^���X���̃��[���h�g�����X�t�H�[����GPU�ɓn�����߂̃f�[�^
	int instance_count = 0;
	InstanceData instance_datas[MAX_INSTANCES];	// �C���X�^���X���̃f�[�^
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_data_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> instance_data_structured_buffer;

	// �`�悷��C���X�^���X�̃V�F�[�_�[
	std::vector<InstancingModelShaderComponent*> shader_component_vector;
};

