#pragma once
#include <vector>
#include <memory>

class AnimeTransitionJudgementBase;
using StateIndex = size_t;

class StateBase
{
public:
    // �X�e�[�g�̑J�ڏ��
    struct StateTransitionInfo
    {
        StateIndex state_index;
        std::unique_ptr<AnimeTransitionJudgementBase> judgement;	// �J�ڔ���
    };

    enum class JudgementUpdatePhase
    {
        PreUpdate,      // �X�V�O�̏��
        PostUpdate,     // �X�V��̏��
    };
public:
    StateBase(){}
    virtual ~StateBase() {};

    virtual void Start() {};

    // �X�̍X�V�Ə�ԑJ�ڂ̍X�V������
    // �X�V�֐�
    void Update(float elapsed_time) {};

    // �J�ڔ���
    bool RreTransitionJudgemen() {};
    bool PostTransitionJudgemen() {};

    virtual const char* Name() = 0;

    void AddStateTransition(StateIndex state_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, JudgementUpdatePhase phase);

private:
    std::vector<StateTransitionInfo> pre_update_judgement_pool;     // Update�O�ɑJ�ڔ�����s��
    std::vector<StateTransitionInfo> post_update_judgement_pool;    // Update��ɑJ�ڔ�����s��
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};