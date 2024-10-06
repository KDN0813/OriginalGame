#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <string>
#include <memory>
#include "Component.h"
#include "Model/ModelCommonData.h"

class ModelResource;

class Transform3DComponent;

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
	void Start();
	// �X�V�֐�
	void Update(float elapsed_time);
	// ���O�擾
	const char* GetName()const { return "ModelComponent"; };
	// �D��x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);
	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsed_time);
	// �A�j���[�V�����Đ�
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	void PlayAnimation(const AnimeState& animation_info, float blend_seconds);
	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation()const;
	// �A�j���[�V�����̑J�ڏ������������Ă��邩
	// �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
	bool IsTransitionReady();
	// �J�ڔ���̃��W�b�N�����s
	// `judgemen��should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
	bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);

	// �m�[�h����
	ModelComponent::Node* FindNode(const char* name);

	// �A�j���[�V������Ԃ̍X�V
	void UpdateAnimationState();
	// �A�j���[�V������Ԃ̐ݒ�
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// �J�ڂ���A�j���[�V�����̒ǉ�
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, float blend_time);

	// �e��f�[�^�擾
	const std::vector<Node>& GetNodes() const { return node_vec; }
	std::vector<Node>& GetNodes() { return node_vec; }
	const ModelResource* GetResource() const { return resource.get(); }
	float GetCurrentAnimationSeconds()const { return current_animation_seconds; }

private:
	std::shared_ptr<ModelResource>	resource;	// ���f�����\�[�X
	std::vector<Node>				node_vec;	// �m�[�h

	std::vector<AnimeState>				anime_state_pool;	// �A�j���[�V�������	
	int current_animation_index  = -1;		// �Đ����̃A�j���[�V�����̃C���f�b�N�X
	float current_animation_seconds = 0;	// ���݂̍Đ�����
	float animation_blend_time = 0;			// �A�j���[�V�����u�����h�̌o�ߎ���
	float animation_blend_seconds = 0;		// �A�j���[�V�����u�����h�̎���
	bool animation_loop_flag = 0;			// ���[�v�t���O
	bool animation_end_flag = 0;			// �I���t���O
	bool dummy[2]{};
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;
	void DrawDebugAnimationGUI();
	void DrawDetail();
private:
	std::vector<std::string> animation_name_pool;
	bool stop_anime_state_update = false;	// �A�j���[�V�����X�e�[�g�̍X�V��~�t���O
	bool is_draw_deletail = false;
	bool stop_anime = false;				// �A�j���[�V�����̒�~
	int select_animation_index = 0;			// �ڍׂ�\������A�j���[�V�����̃C���f�b�N�X
	const char* model_filename;
#endif // _DEBUG
};

