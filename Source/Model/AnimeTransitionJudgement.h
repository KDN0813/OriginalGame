#pragma once
#include <memory>
class Object;

using Owner = std::shared_ptr<Object>;

// �A�j���[�V�����̑J�ڔ���
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() = delete;
	AnimeTransitionJudgementBase(Owner object, bool reversal):owner_Wptr(object), reversal(reversal){}
	virtual ~AnimeTransitionJudgementBase() {}

	// ���O�擾
	virtual const char* GetName()const = 0;;

	// reversal�t���O��True�Ȃ�Check�̐��딻�肪�t�]����
	// �J�ڔ���
	bool Judgement();
protected:
	// �ʏ펞�̑J�ڔ��������
	virtual bool Check() = 0;

protected:
	std::weak_ptr<Object> owner_Wptr;
private:
	bool reversal;	// ���茋�ʂ𔽓]������t���O

#ifdef _DEBUG
public:
	virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

