#pragma once
#include <memory>
#include "System/Alias/OwnerAlias.h"

// �A�j���[�V�����̑J�ڔ�����s�����N���X
class TransitionJudgementBase
{
public:
	TransitionJudgementBase() = delete;
	TransitionJudgementBase(OwnerPtr object, bool reversal ,bool require_transition_ready):owner_Wptr(object), should_reverse(reversal), require_transition_ready(require_transition_ready){}
	virtual ~TransitionJudgementBase() {}

	// ���O�擾
	virtual const char* GetName()const = 0;;

	// �J�ڏ����̔���
	virtual bool CheckTransitionCondition() = 0;

	// �e��擾�E�ݒ�֐�
	bool GetIsActive() { return this->is_active; }
	bool GetShouldReversey() { return this->should_reverse; }
	bool GetRequireTransitionReady() { return this->require_transition_ready; }
	void SetIsActive(bool is_active) { this->is_active = is_active; }
	OwnerPtr GetOwner() { return this->owner_Wptr.lock(); }

protected:
	OwnerWPtr owner_Wptr;
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

