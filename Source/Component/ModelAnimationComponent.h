#include "Component/Component.h"
#include "Model/ModelCommonData.h"
#include "Model/AnimeTransitionJudgement.h"


class ModelAnimationComponent
{
public:
	// �A�j���[�V�����̑J�ڏ��
	struct AnimeTransitionInfo
	{
		AnimeIndex next_anime_index = -1;							// ���̃A�j���̃C���f�b�N�X
		std::unique_ptr<AnimeTransitionJudgementBase> judgement;	// �J�ڔ���
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
	ModelAnimationComponent();
};
