#include <memory>
#include <d3d11.h>
#include "Component/Component.h"
#include "Model/ModelCommonData.h"
#include "StateMachine/LegacyTransitionJudgement.h"

class ModelComponent;
class ModelResource;

// ���f���N���X�̃A�j���[�V�����N���X
class ModelAnimationComponent : public Component
{
public:
	struct AnimationParam
	{
		int current_animation_index = -1;		// �Đ����̃A�j���[�V�����̃C���f�b�N�X
		float current_animation_seconds = 0;	// ���݂̍Đ�����
		float animation_blend_time = 0;			// �A�j���[�V�����u�����h�̌o�ߎ���
		float animation_blend_seconds = 0;		// �A�j���[�V�����u�����h�̎���
		bool animation_loop_flag = 0;			// ���[�v�t���O
		bool animation_end_flag = 0;			// �I���t���O
		bool dummy[2]{};
	};
public:
	// �A�j���[�V�����̑J�ڏ��
	struct AnimeTransitionInfo
	{
		AnimeIndex next_anime_index = -1;							// ���̃A�j���̃C���f�b�N�X
		std::unique_ptr<TransitionJudgementBase> judgement;	// �J�ڔ���
		float blend_time = 1.0f;									// �u�����h����
	};
	// �A�j���[�V�������
	struct AnimeState
	{
		std::string name = {};													// �A�j���[�V������
		AnimeIndex anime_index = -1;											// �A�j���̃C���f�b�N�X
		bool loop = false;														// ���[�v�t���O
		float transition_ready_time = -1.0f;									// �J�ڏ�������������܂ł̎���(0�ȉ��Ȃ�Đ��I���ŏ�������)
		std::vector<std::unique_ptr<AnimeTransitionInfo>> transition_info_pool;	// �J�ڂ���A�j���[�V�������
	};
public:
	ModelAnimationComponent(AnimationParam param ,const char* filename);

	// �J�n�֐�
	void Start() override {};
	// ���X�^�[�g����
	void ReStart() override;      // �p�����[�^�̏�����
	// �X�V�֐�
	void Update(float elapsed_time) override;
	// ���O�擾
	const char* GetName()const override { return "ModelAnimationComponent"; };
	// �D��x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::MEDIUM; }

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
	bool PerformTransitionJudgement(TransitionJudgementBase* judgemen);

	// �A�j���[�V������Ԃ̍X�V
	void UpdateAnimationState();
	// �A�j���[�V������Ԃ̐ݒ�
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// �J�ڂ���A�j���[�V�����̒ǉ�
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<TransitionJudgementBase> judgement, float blend_time);

	// �e��f�[�^�擾
	float GetCurrentAnimationSeconds()const { return this->param.current_animation_seconds; }
private:
	std::vector<AnimeState>				anime_state_pool;	// �A�j���[�V�������	
	int animation_size = 0;					// �A�j���[�V�����̐�
	AnimationParam param;
	AnimationParam default_param;

private:
	std::weak_ptr<ModelComponent> model_Wptr;
	std::weak_ptr<ModelResource> model_resource_Wptr;

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
