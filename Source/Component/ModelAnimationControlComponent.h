#pragma once
#include "Component.h"
#include "Model/ModelCommonData.h"

class ModelComponent;
class ModelResource;

// ���f���N���X�̃A�j���[�V�����N���X
// �O������̎w���ŃA�j���[�V�����𐧌䂷��
class ModelAnimationControlComponent : public Component
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
	ModelAnimationControlComponent(const char* filename);

	// �J�n�֐�
	void Start() override;
	// ���X�^�[�g����
	void ReStart() override;      // �p�����[�^�̏�����
	// �X�V�֐�
	void Update(float elapsed_time) override;
	// ���O�擾
	const char* GetName()const override { return "ModelAnimationControlComponent"; };
	// �D��x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::MEDIUM; }

	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsed_time);
	// �A�j���[�V�����Đ�
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation()const;

	// �e��f�[�^�擾
	float GetCurrentAnimationSeconds()const { return this->param.current_animation_seconds; }
private:
	AnimationParam param;

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

