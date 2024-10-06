#pragma once
#include <memory>
#include <d3d11.h>
#include "Component/Component.h"
#include "Model/ModelCommonData.h"
#include "Model/AnimeTransitionJudgement.h"

class ModelComponent;
class ModelResource;
class InstancingModelResource;

class InstancingModelAnimationComponent : public Component
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
		int transition_ready_frame = -1;										// �J�ڏ�������������܂ł̎���(0�ȉ��Ȃ�Đ��I���ŏ�������)
		std::vector<std::unique_ptr<AnimeTransitionInfo>> transition_info_pool;	// �J�ڂ���A�j���[�V�������
	};
public:
	InstancingModelAnimationComponent(ID3D11Device* device, const char* filename);

	// �J�n�֐�
	void Start() override {};
	// �X�V�֐�
	void Update(float elapsed_time) override;
	// ���O�擾
	const char* GetName()const override { return "ModelAnimationComponent"; };
	// �D��x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::MEDIUM; }

	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsed_time);
	// �A�j���[�V�����Đ�
	void PlayAnimation(int animeIndex, bool loop = true);
	void PlayAnimation(const AnimeState& animation_info);
	// �A�j���[�V�����̑J�ڏ������������Ă��邩
	// �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
	bool IsTransitionReady();
	// �J�ڔ���̃��W�b�N�����s
	// `judgemen��should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
	bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);

	// �A�j���[�V������Ԃ̍X�V
	void UpdateAnimationState();
	// �A�j���[�V������Ԃ̐ݒ�
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// �J�ڂ���A�j���[�V�����̒ǉ�
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement);

	// �e��f�[�^�擾
	float GetCurrentAnimationSeconds()const { return anime_frame; }
private:
	std::vector<AnimeState>				anime_state_pool;	// �A�j���[�V�������	
	int animation_size = 0;					// �A�j���[�V�����̐�
	UINT anime_index = -1;					// �Đ����̃A�j���[�V�����̃C���f�b�N�X
	UINT anime_frame = 0;					// ���݂̍Đ��t���[��
	int anime_speed = 1;					// �A�j���[�V�����̍Đ����x
	bool anime_loop = 0;					// ���[�v�t���O
	bool anime_play = false;				// �A�j���[�V�������Đ����ł��邩
	bool dummy[2]{};
private:
	std::weak_ptr<ModelComponent> model_Wptr;
	std::weak_ptr<ModelResource> model_resource_Wptr;
	std::weak_ptr<InstancingModelResource> instancing_model_resource_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;
	void DrawDetail();
private:
	std::vector<std::string> animation_name_pool;
	bool stop_anime_state_update = false;	// �A�j���[�V�����X�e�[�g�̍X�V��~�t���O
	bool is_draw_deletail = false;
	int select_animation_index = 0;			// �ڍׂ�\������A�j���[�V�����̃C���f�b�N�X
#endif // _DEBUG
};

