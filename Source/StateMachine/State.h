#pragma once
#include <vector>
#include <memory>
#include <string>
#include "System/Alias/OwnerAlias.h"
#include "Model/AnimeTransitionJudgement.h"

using StateIndex = size_t;

// �X�e�[�g�̑J�ڏ��
class StateTransitionInfo
{
public:
    StateTransitionInfo(std::string next_state_name, std::unique_ptr<AnimeTransitionJudgementBase> judgement);
    ~StateTransitionInfo() {};

    StateIndex GetNextStateIndex() { return this->next_state_index; }
    std::string GetNextStateName() { return this->next_state_name; }
    const char* GetNextStateNameC() { return this->next_state_name.c_str(); }
    AnimeTransitionJudgementBase* GetJudgement() { return judgement.get(); }
private:
    StateIndex next_state_index;                                // �J�ڐ�̃X�e�[�g�̃C���f�b�N�X
    std::string next_state_name;                                // �J�ڐ�̃X�e�[�g�̖��O
    std::unique_ptr<AnimeTransitionJudgementBase> judgement;	// �J�ڔ���
};

class StateBase
{
public:
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
    virtual void Update(float elapsed_time) {};

    virtual const char* Name() = 0;

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    virtual bool IsTransitionReady() { return true; };

    void SetOwner(OwnerPtr owner);

    // �X�V�֐��̑O�̑J�ڔ���
    void PreTransitionJudgemen();
    // �X�V�֐��̌�̑J�ڔ���
    void PostTransitionJudgemen();

    // �J�ڔ���̃��W�b�N�����s
    // `judgemen��should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
    bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);
    
    // �J�ڃX�e�[�g�̒ǉ�
    void AddStateTransition(std::unique_ptr<StateTransitionInfo> state_transition, JudgementUpdatePhase phase);
protected:
    OwnerWPtr owner_Wptr;
private:
    std::vector<std::unique_ptr<StateTransitionInfo>> pre_update_judgement_pool;     // Update�O�ɑJ�ڔ�����s��
    std::vector<std::unique_ptr<StateTransitionInfo>> post_update_judgement_pool;    // Update��ɑJ�ڔ�����s��
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};

template <class T>
concept is_State = requires{ std::is_base_of_v<StateBase, T>; };