#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Model/ModelResource.h"
#include "Graphics/Shader/Shader.h"

using WorldTransform = DirectX::XMFLOAT4X4;

// �C���X�^���V���O���f��
class InstancingModel
{
public:
	static const int MAX_INSTANCES = 512;

public:
	// TODO(08/04)�e�I�u�W�F�N�g���ɒ��ڒl���������悤�ɃV�F�A�h�|�C���^�ɕύX����
	// �X��transform���Ǘ����邽�߂̍\����
	struct TransformData
	{
		DirectX::XMFLOAT4X4	transform;
		bool exist;		// �g�p����
		bool dummy[3];
	};

	struct Node
	{
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};

	// �{�[���g�����X�t�H�[���p�\����
	struct BoneTransform
	{
		DirectX::XMFLOAT4X4 transform;
	};
	// �{�[���g�����X�t�H�[���e�N�X�`���p�\����
	struct BoneTransformTextureData
	{
		std::vector<BoneTransform> bone_transforms;
	};

	/**
	* �{�[���g�����X�t�H�[�����擾���邽�߂ɕK�v�ȃp�����[�^
	* \param bone_size 1��̕`��Ŏg�p����{�[���g�����X�t�H�[���̐�
	* \param mesh_first_bone_index �`�悷�郁�b�V���̐擪�̃{�[���g�����X�t�H�[���̃C���f�b�N�X
	* \param animation_first_bone_index �g���A�j���[�V�����̐擪�̃{�[���g�����X�t�H�[���̃C���f�b�N�X
	* \param frame �A�j���[�V�����̃t���[��
	*/
	struct BoneTransformData
	{
		UINT bone_size = { 0 };
		UINT mesh_first_bone_index = { 0 };
		UINT animation_first_bone_index = { 0 };
		UINT frame = { 0 };
	};
	/**
	 * \param bone_transform_data �{�[���g�����X�t�H�[�����擾���邽�߂ɕK�v�ȃp�����[�^
	 * \param world_transform ���[���h�g�����X�t�H�[��
	 */
	struct InstanceData
	{
		BoneTransformData bone_transform_data{};
		WorldTransform world_transform{};
	};
public:
	InstancingModel(ID3D11Device* dc,const char* filename);
	~InstancingModel() {};

	// �X��transform���Ǘ����邽�߂̊֐�
	// �g���Ă��Ȃ��ԍ������蓖�ĂĕԂ�
	int AllocateInstancingIndex();
	// ���蓖�Ă�ꂽ�ԍ����������
	void FreeInstancingIndex(int instancingIndex);
	// �s��v�Z
	void UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform);

	// world_transform_structured_buffer�̍X�V
	void UpdateWorldTransformBuffer(ID3D11DeviceContext* device, int& instancing_count);

	// �e��擾�E�ݒ�֐�
	const ModelResource* GetResource() const { return resource.get(); }
	ID3D11ShaderResourceView** GetBoneTransformTexture() { return this->bone_transform_texture.GetAddressOf(); }
	ID3D11ShaderResourceView** GetWorldTransformStructuredBuffer() { return this->world_transform_structured_buffer.GetAddressOf(); }

	// TODO(08/04)�v�Z�ʒu�ύX����
	// BTT�v�Z�p�֐�
	void PlayAnimation(int index);
	void UpdateAnimation(float elapsed_time);
	bool IsPlayAnimation()const;
	void UpdateTransform();
private:
	std::shared_ptr<ModelResource>	resource;

	// BTT
	std::vector<BoneTransformData>			bone_transform_datas;
	Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

	// �C���X�^���X���̃��[���h�g�����X�t�H�[����GPU�ɓn�����߂̃f�[�^
	WorldTransform* world_transforms = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> world_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> world_transform_structured_buffer;

	
	// TODO(08/04)�v�Z�ʒu�ύX����
	// BTT�v�Z�p�ϐ�
	std::vector<Node> nodes;
	int current_animation_index = -1;	// �l��(-1)�Ȃ��~���Ă���
	float current_animation_seconds = 0.0f;	// �o�ߎ���

	// TODO(08/04)�e�I�u�W�F�N�g���ɒ��ڒl���������悤�ɃV�F�A�h�|�C���^�ɕύX����
	std::vector<TransformData> transform_datas;	// �X��transform���Ǘ�����R���e�i
};

