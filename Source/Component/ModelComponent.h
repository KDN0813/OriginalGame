#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include "Component.h"

class ModelResource;

class Transform3DComponent;

using AnimeIndex = size_t;

// �A�j���[�V�����J�ڔ���
class AnimationTransitionJudgementBase
{
public:
	AnimationTransitionJudgementBase(){}
	virtual ~AnimationTransitionJudgementBase() {}

	virtual bool Judgement() = 0;
};

// �J�ڂ���A�j���[�V�������
struct AnimationTransitionState
{
	AnimeIndex next_anime_index = -1;							// ���̃A�j���̃C���f�b�N�X
	AnimationTransitionJudgementBase* judgement = nullptr;		// �J�ڔ���
	float blend_time = 1.0f;									// �u�����h����
};

// �A�j���[�V�������
struct AnimationInfo
{
	bool loop = false;
	AnimeIndex anime_index = -1;
	std::vector<AnimationTransitionState*> next_animation_vec;
};

class ModelComponent : public Component
{
public:
	struct Node
	{
		const char* name = {};
		Node* parent = {};
		DirectX::XMFLOAT3	scale = {};
		DirectX::XMFLOAT4	rotate = {};
		DirectX::XMFLOAT3	translate = {};
		DirectX::XMFLOAT4X4	local_transform = {};
		DirectX::XMFLOAT4X4	world_transform = {};
		std::vector<Node*>	children;
	};

public:
	ModelComponent(ID3D11Device* device, const char* filename);

	// �J�n�֐�
	void Initialize();
	// �X�V�֐�
	void Update(float elapsed_time);
	// ���O�擾
	const char* GetName()const { return "ModelComponent"; };
	// �D��x
	const unsigned int GetPriority()const noexcept override { return 2; }

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);
	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsed_time);
	// �A�j���[�V�����Đ�
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	void PlayAnimation(AnimationInfo* animation_info, float blend_seconds);
	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation()const;
	// �m�[�h����
	ModelComponent::Node* FindNode(const char* name);

	// �A�j���[�V�����̍X�V
	void AnimationStateUpdate();

	// �e��f�[�^�擾
	const std::vector<Node>& GetNodes() const { return node_vec; }
	std::vector<Node>& GetNodes() { return node_vec; }
	const ModelResource* GetResource() const { return resource.get(); }
	float GetCurrentAnimationSeconds()const { return current_animation_seconds; }

private:
	std::vector<AnimationInfo*>				animation_info_vec;	// �A�j���[�V�������	

	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				node_vec;

	int current_animation_index  = -1;
	float current_animation_seconds = 0;
	float animation_blend_time = 0;
	float animation_blend_seconds = 0;
	bool animation_loo_flag = 0;
	bool animation_end_flag = 0;
	bool dummy[2]{};
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;

#endif // _DEBUG
};

