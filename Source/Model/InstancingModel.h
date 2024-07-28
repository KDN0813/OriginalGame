#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Model/ModelResource.h"
#include "Graphics/Shader/Shader.h"

// �C���X�^���V���O���f��
class InstancingModel
{
public:
	struct InstancingData
	{
		DirectX::XMFLOAT4X4	transform;
		bool exist;		// �g�p����
		bool dummy[3];
	};

	struct Node
	{
		Node() = default;
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};
public:
	InstancingModel(const char* filename);
	~InstancingModel() {};

	// �g���Ă��Ȃ��ԍ������蓖�ĂĕԂ�
	int AllocateInstancingIndex();

	// ���蓖�Ă�ꂽ�ԍ����������
	void FreeInstancingIndex(int instancingIndex);

	// �s��v�Z
	void UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform);

	// �e��擾�E�ݒ�֐�
	const ModelResource* GetResource() const { return resource.get(); }
	const size_t GetInstanceCount()const { return this->instance_cout; }
	// �V�F�[�_�[�ւ̐ݒ�p�o�b�t�@���擾
	BufferData GetBufferData(const ModelResource::Mesh& mesh) const;

	ID3D11ShaderResourceView** GetBoneTransformTexture() { return this->bone_transform_texture.GetAddressOf(); }
private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<InstancingData> instancing_data;
	size_t instance_cout;

	// 
	struct BoneTransformTextureData
	{
		std::vector<DirectX::XMFLOAT4X4> matrices;

	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

	// BTT�v�Z�p�֐�
	void PlayAnimation(int index);
	void UpdateAnimation(float elapsed_time);
	bool IsPlayAnimation()const;
	void UpdateTransform();
	// BTT�v�Z�p�ϐ�
	std::vector<Node> nodes;
	int current_animation_index = -1;	// �l��(-1)�Ȃ��~���Ă���
	float current_animation_seconds = 0.0f;	// �o�ߎ���
};

