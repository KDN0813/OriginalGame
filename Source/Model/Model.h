#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Model/ModelResource.h"

// TODO (06/25)�J���w�j����܂莟����P
class Model
{
public:
	Model(const char* filename);
	~Model() {}

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

	// �e��f�[�^�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }
	const ModelResource* GetResource() const { return resource.get(); }
	float GetCurrentAnimationSeconds()const { return current_animation_seconds; }

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsedTIme);

	// �A�j���[�V�����Đ�
	void PlayAnimation(int index,bool loop,float blendSeconds = 0.2f);

	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation()const;

	// �m�[�h����
	Model::Node* FindNode(const char* name);

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	int current_animation_index;
	float current_animation_seconds;
	float animation_blend_time;
	float animation_blend_seconds;
	bool animation_loo_flag;
	bool animation_end_flag;
	bool dummy[3]{};
};
