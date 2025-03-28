#pragma once
#include "Component.h"
#include "Model/ModelCommonData.h"
#include "Model\ModelResource.h"

class ModelComponent;

// ���f���N���X�̃A�j���[�V�����N���X
// �p�[�c���ɕʂ̃A�j���[�V�������Đ��ł���
class ModelPartAnimationControlComponent : public Component
{
public:
	struct InitAnimeParam
	{
		int init_anime_index = -1;				// �ŏ��ɍĐ�����A�j���[�V�����C���f�b�N�X
		bool init_anime_loop = false;			// �ŏ��ɍĐ�����A�j���[�V�����̃��[�v�t���O
	};
	struct AnimationParam
	{
		int current_animation_index = -1;		// �Đ����̃A�j���[�V�����̃C���f�b�N�X
		float current_animation_seconds = 0.0f;	// ���݂̍Đ�����
		float animation_blend_time = 0.0f;			// �A�j���[�V�����u�����h�̌o�ߎ���
		float animation_blend_seconds = 0.0f;		// �A�j���[�V�����u�����h�̎���
		bool animation_loop_flag = 0;			// ���[�v�t���O
		bool animation_end_flag = 0;			// �I���t���O
		bool dummy[2]{};
	};

	struct PartsParam
	{
		AnimationParam anime_param;							// �A�j���[�V�������
		std::vector<ModelResource::NodeId> node_index_vec;	// ��L�̏��ōX�V����m�[�h�̃C���f�b�N�X
	};
public:
	ModelPartAnimationControlComponent(InitAnimeParam init_param);

	// �J�n�֐�
	void Start() override;
	// ���X�^�[�g����
	void ReStart() override;      // �p�����[�^�̏�����
	// �X�V�֐�
	void Update(float elapsed_time) override;
	// ���O�擾
	const char* GetName()const override { return "ModelPartAnimationControlComponent"; };
	// �D��x
	const PRIORITY GetPriority()const noexcept override { return PRIORITY::MEDIUM; }

	/**
	 * �A�j���[�V�����X�V����
	 * 
	 * \param parts	�X�V����p�[�c�̎Q��
	 * \param elapsed_time �o�ߎ���
	 */
	void UpdateAnimation(PartsParam& parts, float elapsed_time);
	// �A�j���[�V�����Đ�
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation()const;

private:
	InitAnimeParam init_param;
	
	PartsParam main_parts;

private:
	std::weak_ptr<ModelComponent> model_Wptr;
	size_t animation_size = 0;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;
private:
	std::vector<std::string> animation_name_pool;
#endif // _DEBUG
};

