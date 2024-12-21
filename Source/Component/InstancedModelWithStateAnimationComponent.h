#pragma once
#include "Component/InstancedModelWithAnimationComponent.h"

// �A�j���[�V�����t�C���X�^���X���f��
// �����ŃX�e�[�g���Ǘ����A�Ɨ����ăA�j���[�V�����J�ڂ��s��
class InstancedModelWithStateAnimationComponent : public InstancedModelWithAnimationComponent
{
public:
	// �A�j���[�V�����̑J�ڏ��
	struct AnimeTransitionInfo
	{
		AnimeIndex next_anime_index = -1;							// ���̃A�j���̃C���f�b�N�X
		std::unique_ptr<TransitionJudgementBase> judgement;	// �J�ڔ���
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
	InstancedModelWithStateAnimationComponent(InstancedModelParam param, const char* filename);

	// ���X�^�[�g����
	void ReStart() override;      // �p�����[�^�̏�����
    void Update(float elapsed_time) override;

	const char* GetName()const override { return "AnimatedInstancedModelComponent"; }
	// �D��x
	const PRIORITY GetPriority()const noexcept override { return PRIORITY::LOW; }
	
	void PlayAnimation(const AnimeState& animation_info);

	// �A�j���[�V������Ԃ̍X�V
	void UpdateAnimationState();
	// �A�j���[�V������Ԃ̐ݒ�
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// �J�ڂ���A�j���[�V�����̒ǉ�
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<TransitionJudgementBase> judgement);

	// �A�j���[�V�����̑J�ڏ������������Ă��邩
	// �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
	bool IsTransitionReady();
	// �J�ڔ���̃��W�b�N�����s
	// `judgemen��should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
	bool PerformTransitionJudgement(TransitionJudgementBase* judgemen);

	// �e�擾�E�ݒ�֐�
private:
	std::vector<AnimeState>	anime_state_pool;	// �A�j���[�V�������

#ifdef _DEBUG
public:
	void DrawDebugGUI()override;
	void DrawDebugAnimationGUI();
	void DrawDetail();

private:
	bool stop_anime_state_update = false;
	bool is_draw_deletail = false;
	int select_animation_index = 0;			// �ڍׂ�\������A�j���[�V�����̃C���f�b�N�X
#endif // _DEBUG
};

