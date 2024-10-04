#pragma once
#include <memory>
class Object;

using Owner = std::shared_ptr<Object>;

// �A�j���[�V�����̑J�ڔ�����s�����N���X
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() = delete;
	AnimeTransitionJudgementBase(Owner object, bool reversal):owner_Wptr(object), should_reverse(reversal){}
	virtual ~AnimeTransitionJudgementBase() {}

	// ���O�擾
	virtual const char* GetName()const = 0;;

	// �J�ڔ���̃��W�b�N�����s
	// `should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
	bool PerformTransitionJudgement();

	// �e��擾�E�ݒ�֐�
	const bool& GetIsActive() { return this->is_active; }
	void SetIsActive(bool is_active) { this->is_active = is_active; }
protected:
	// ��̓I�ȑJ�ڏ����̔���
	virtual bool CheckTransitionCondition() = 0;

protected:
	std::weak_ptr<Object> owner_Wptr;
private:
	bool should_reverse;	// ���茋�ʂ𔽓]�����邩�ǂ����̃t���O
	bool is_active = true;

#ifdef _DEBUG
public:
	virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

