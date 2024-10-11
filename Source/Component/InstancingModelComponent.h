#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include "Model/ModelCommonData.h"

class InstancingModelResource;
class ModelResource;

class Transform3DComponent;

// �A�j���[�V�����t�C���X�^���X���f��
class AnimatedInstancedModelComponent : public Component
{
public:
	// �A�j���[�V�����̑J�ڏ��
	struct AnimeTransitionInfo
	{
		AnimeIndex next_anime_index = -1;							// ���̃A�j���̃C���f�b�N�X
		std::unique_ptr<AnimeTransitionJudgementBase> judgement;	// �J�ڔ���
	};
	// �A�j���[�V�������
	struct AnimeState
	{
		std::string name = {};													// �A�j���[�V������
		AnimeIndex anime_index = -1;											// �A�j���̃C���f�b�N�X
		bool loop = false;														// ���[�v�t���O
		float transition_ready_time = -1;										// �J�ڏ�������������܂ł̎���(0�ȉ��Ȃ�Đ��I���ŏ�������)
		std::vector<std::unique_ptr<AnimeTransitionInfo>> transition_info_pool;	// �J�ڂ���A�j���[�V�������
	};
public:
	AnimatedInstancedModelComponent(ID3D11Device* device, const char* filename);

    void Update(float elapsed_time) override;

	const char* GetName()const override { return "AnimatedInstancedModelComponent"; }
	// �D��x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }
	
	void PlayAnimation(int animeIndex, bool loop = true);
	void PlayAnimation(const AnimeState& animation_info);

	void UpdateAnimation(float elapsed_time);
	// �A�j���[�V������Ԃ̍X�V
	void UpdateAnimationState();
	// �A�j���[�V������Ԃ̐ݒ�
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// �J�ڂ���A�j���[�V�����̒ǉ�
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement);

	// �A�j���[�V�����̑J�ڏ������������Ă��邩
	// �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
	bool IsTransitionReady();
	// �J�ڔ���̃��W�b�N�����s
	// `judgemen��should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
	bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);

	// �e�擾�E�ݒ�֐�
	InstancingModelResource* GetInstancingModelResource() { return this->instancing_model_resource.get(); }
	ModelResource* GetModelResource() { return this->model_resource.get(); }
	UINT GetAnimeFrame();
	float GetCurrentAnimationSeconds() const { return this->current_animation_seconds; }
	UINT GetAnimationStartOffset();
	int GetModelId();
private:
	std::shared_ptr<InstancingModelResource> instancing_model_resource;
	std::shared_ptr<ModelResource> model_resource;

	std::vector<AnimeState>	anime_state_pool;	// �A�j���[�V�������

	float current_animation_seconds = 0.0f;		// ���݂̍Đ�����
	UINT anime_index = 0;
	int anime_speed = 1;
	bool anime_loop = false;
	bool anime_play = false;					// �A�j���[�V�������Đ����ł��邩

private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI()override;
	void DrawDebugAnimationGUI();
	void DrawDetail();

private:
	std::vector<std::string> animation_name_pool;
	bool stop_anime_state_update = false;
	bool is_draw_deletail = false;
	int select_animation_index = 0;			// �ڍׂ�\������A�j���[�V�����̃C���f�b�N�X
	const char* model_filename;
#endif // _DEBUG
};

