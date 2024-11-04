#pragma once
#include <vector>
#include <memory>
#include <string>
#include "System/Alias/OwnerAlias.h"
#include "System/MyHash.h"
#include "StateMachine/LegacyTransitionJudgement.h"

static constexpr size_t INVALID_STATE_INDEX = SIZE_MAX;   // �����ȃX�e�[�g�̃C���f�b�N�X(�X�e�[�g�̃C���f�b�N�X�̏����l�E�G���[�R�[�h�Ɏg�p����)
using StateIndex = size_t;

// (��)�X�e�[�g�̑J�ڏ��
class LegacyStateTransitionInfo
{
public:
public:
    LegacyStateTransitionInfo(std::string next_state_name, std::unique_ptr<TransitionJudgementBase>&& judgement);
    ~LegacyStateTransitionInfo() {};

    StateIndex GetNextStateIndex() { return this->next_state_index; }
    MyHash GetNextStateNameHash() { return this->next_state_name; }
    std::string GetNextStateNameStr() { return this->next_state_name.GetString(); }
    const char* GetNextStateNameCstr() { return this->next_state_name.GetString().c_str(); }
    TransitionJudgementBase* GetJudgement() { return judgement.get(); }
    void SetNextStateIndex(StateIndex index) { this->next_state_index = index; }
private:
    StateIndex next_state_index;                                // �J�ڐ�̃X�e�[�g�̃C���f�b�N�X
    MyHash next_state_name;                                // �J�ڐ�̃X�e�[�g�̖��O
    std::unique_ptr<TransitionJudgementBase> judgement;	// �J�ڔ���
};

class LegacyStateBase
{
public:
    enum class JudgementUpdatePhase
    {
        PreUpdate,      // �X�V�O�̏��
        PostUpdate,     // �X�V��̏��
    };
public:
    LegacyStateBase() = delete;
    LegacyStateBase(std::string name) :name(name) {};
    virtual ~LegacyStateBase() {};

    // �J�n�֐�
    virtual void Start() {};
    // �X�̍X�V�Ə�ԑJ�ڂ̍X�V������
    // �X�V�֐�
    virtual void Update(float elapsed_time) {};
    // �I���֐�
    virtual void End() {};

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    virtual bool IsTransitionReady() { return true; };

    void SetOwner(OwnerPtr owner);

    const char* GetNameCStr() { return name.GetString().c_str(); };
    std::string GetNameStr() { return name.GetString(); };
    MyHash GetHash() { return name; };

    // �J�ڔ���̃��W�b�N�����s
    // `judgemen��should_reverse` �t���O��true�Ȃ�A�J�ڔ��茋�ʂ𔽓]����
    bool PerformTransitionJudgement(TransitionJudgementBase* judgemen);
    
    // �J�ڃX�e�[�g�̒ǉ�
    void AddStateTransition(std::unique_ptr<LegacyStateTransitionInfo> state_transition, JudgementUpdatePhase phase);
    
    // �e��ݒ�E�擾�֐�
    const std::vector<std::unique_ptr<LegacyStateTransitionInfo>>& GetPreUpdateJudgementPool() { return this->pre_update_judgement_pool; }
    const std::vector<std::unique_ptr<LegacyStateTransitionInfo>>& GetPostUpdateJudgementPool() { return this->post_update_judgement_pool; }
    StateIndex GetStateIndex() { return this->state_index; }
    void SetStateIndex(StateIndex index) { this->state_index = index; }
protected:
    OwnerWPtr owner_Wptr;
private:
    StateIndex state_index = -1;
    MyHash name;
    std::vector<std::unique_ptr<LegacyStateTransitionInfo>> pre_update_judgement_pool;     // Update�O�ɑJ�ڔ�����s��
    std::vector<std::unique_ptr<LegacyStateTransitionInfo>> post_update_judgement_pool;    // Update��ɑJ�ڔ�����s��
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};

template <class T>
concept Legacy_is_State = requires{ std::is_base_of_v<LegacyStateBase, T>; };