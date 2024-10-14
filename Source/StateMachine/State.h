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

    virtual const char* Name() = 0;

    // �X�V�֐��̑O�̑J�ڔ���
    void PreTransitionJudgemen();
    // �X�V�֐��̌�̑J�ڔ���
    void PostTransitionJudgemen();

    // �J�ڔ���̃��W�b�N�����s
    // `judgemen��should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
    bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);
    
    // �J�ڃX�e�[�g�̒ǉ�
    void AddStateTransition(StateIndex state_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, JudgementUpdatePhase phase);
private:
    std::vector<StateTransitionInfo> pre_update_judgement_pool;     // Update�O�ɑJ�ڔ�����s��
    std::vector<StateTransitionInfo> post_update_judgement_pool;    // Update��ɑJ�ڔ�����s��
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};

template <class T>
concept is_State = requires{ std::is_base_of_v<StateBase, T>; };