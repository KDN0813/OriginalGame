#pragma once
#include <memory>
class Object;
class ModelComponent;

using Owner = Object;
using OwnerPtr = std::shared_ptr<Owner>;

// �A�j���[�V�����̑J�ڔ�����s�����N���X
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() = delete;
	AnimeTransitionJudgementBase(OwnerPtr object, bool reversal ,bool require_transition_ready):owner_Wptr(object), should_reverse(reversal), require_transition_ready(require_transition_ready){}
	virtual ~AnimeTransitionJudgementBase() {}

	// ���O�擾
	virtual const char* GetName()const = 0;;

	// �J�ڔ���̃��W�b�N�����s
	// `should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
	bool PerformTransitionJudgement();

	// �e��擾�E�ݒ�֐�
	const bool& GetIsActive() { return this->is_active; }
	void SetIsActive(bool is_active) { this->is_active = is_active; }
	OwnerPtr GetOwner() { return this->owner_Wptr.lock(); }
protected:
	// ��̓I�ȑJ�ڏ����̔���
	virtual bool CheckTransitionCondition() = 0;

protected:
	std::weak_ptr<Owner> owner_Wptr;
	std::weak_ptr<ModelComponent> model_Wptr;
private:
	bool should_reverse;			// ���茋�ʂ𔽓]�����邩�ǂ����̃t���O
	bool require_transition_ready;	// �J�ڂ̏����������Ă��邱�Ƃ��K�v���ǂ����������t���O
	bool is_active = true;

#ifdef _DEBUG
public:
	void DrawCommonDebugGUI(int unique_id);	// ���ʂ�DebugGUI
	virtual void DrawDebugGUI(int unique_id) {};
#endif // _DEBUG
};

