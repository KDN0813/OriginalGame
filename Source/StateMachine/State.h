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
    void SetNextStateIndex(StateIndex index) { this->next_state_index = index; }
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

    // �J�n�֐�
    virtual void Start() {};
    // �X�̍X�V�Ə�ԑJ�ڂ̍X�V������
    // �X�V�֐�
    virtual void Update(float elapsed_time) {};
    // �I���֐�
    virtual void End() {};

    virtual const char* Name() = 0;

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    virtual bool IsTransitionReady() { return true; };

    void SetOwner(OwnerPtr owner);

    // �J�ڔ���̃��W�b�N�����s
    // `judgemen��should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
    bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);
    
    // �J�ڃX�e�[�g�̒ǉ�
    void AddStateTransition(std::unique_ptr<StateTransitionInfo> state_transition, JudgementUpdatePhase phase);
    
    // �e��ݒ�E�擾�֐�
    const std::vector<std::unique_ptr<StateTransitionInfo>>& GetPreUpdateJudgementPool() { return this->pre_update_judgement_pool; }
    const std::vector<std::unique_ptr<StateTransitionInfo>>& GetPostUpdateJudgementPool() { return this->post_update_judgement_pool; }
    StateIndex GetStateIndex() { return this->state_index; }
    void SetStateIndex(StateIndex index) { this->state_index = index; }
protected:
    OwnerWPtr owner_Wptr;
private:
    StateIndex state_index = -1;
    std::vector<std::unique_ptr<StateTransitionInfo>> pre_update_judgement_pool;     // Update�O�ɑJ�ڔ�����s��
    std::vector<std::unique_ptr<StateTransitionInfo>> post_update_judgement_pool;    // Update��ɑJ�ڔ�����s��
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};

template <class T>
concept is_State = requires{ std::is_base_of_v<StateBase, T>; };