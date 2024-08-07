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
		UINT anime_frame;	// �A�j���[�V�����̍Đ�����
		int anime_index = 0;
		bool anime_loop = true;
		bool anime_play = true;
		bool exist;		// �g�p����
		bool dummy[2];
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
	 * \param animation_start_offset;    // �o�b�t�@���Ŏg�p����A�j���[�V�����̊J�n�ʒu�������I�t�Z�b�g�l
	 * \param frame;                     // ���݂̃t���[��
	 * \param world_transform ���[���h�g�����X�t�H�[��
	 */
	struct InstanceData
	{
		UINT animation_start_offset;    
		UINT frame;                     
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
	// �A�j���[�V�����X�V
	void UpdateAnimationFrame(int instancingIndex);

	// InstanceData�̍X�V
	void UpdateInstanceData(ID3D11DeviceContext* device, int& instancing_count);

	// �e��擾�E�ݒ�֐�
	const ModelResource* GetResource() const { return resource.get(); }
	ID3D11ShaderResourceView** GetBoneTransformTexture() { return this->bone_transform_texture.GetAddressOf(); }
	ID3D11ShaderResourceView** GetWorldTransformStructuredBuffer() { return this->instance_data_structured_buffer.GetAddressOf(); }
	const UINT& GetBoneTransformCount()const { return this->bone_transform_count; }
	const std::vector<UINT>& GetMeshOffsets()const { return this->mesh_offsets; }

	// TODO(08/04)�v�Z�ʒu�ύX����
	// BTT�v�Z�p�֐�
	void PlayAnimation(int index);
	void UpdateAnimation(float elapsed_time);
	bool IsPlayAnimation()const;
	void UpdateTransform();
private:
	std::shared_ptr<ModelResource>	resource;

	// BTT
	Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

	// �C���X�^���X���̃��[���h�g�����X�t�H�[����GPU�ɓn�����߂̃f�[�^
	InstanceData* instance_data = nullptr;	// �C���X�^���X���̃f�[�^�z��̐擪�̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_data_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> instance_data_structured_buffer;
	
	UINT bone_transform_count;				// �{�[���g�����X�t�H�[���̐�
	std::vector<UINT> mesh_offsets;			// BTT�Ŏg�p���郁�b�V�����̊J�n�ʒu�܂ł̃I�t�Z�b�g�l
	std::vector<UINT> animation_lengths;	// �A�j���[�V�����̒���(�t���[����)
	std::vector<UINT> animation_offsets;	// �A�j���[�V�����̒���(�t���[����)


	// TODO(08/04)�v�Z�ʒu�ύX����
	// BTT�v�Z�p�ϐ�
	std::vector<Node> nodes;
	int current_animation_index = -1;	// �l��(-1)�Ȃ��~���Ă���
	float current_animation_seconds = 0.0f;	// �o�ߎ���

	// TODO(08/04)�e�I�u�W�F�N�g���ɒ��ڒl���������悤�ɃV�F�A�h�|�C���^�ɕύX����
	std::vector<TransformData> transform_datas;	// �X��transform���Ǘ�����R���e�i
};

