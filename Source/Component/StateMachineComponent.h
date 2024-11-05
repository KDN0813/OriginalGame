#pragma once
#include <vector>
#include "Component.h"
#include "StateMachine/StateBase.h"

template <class T>
concept is_State = requires{ std::is_base_of_v<State, T>; };

// �X�e�[�g�}�V��
// �����ŏ�����ێ����A�Ɨ����ď�ԑJ�ڂ��s��
class StateMachineComponent : public Component
{
public:
    StateMachineComponent();
    ~StateMachineComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;
    // ���O�擾
    const char* GetName()const  override { return "StateMachine"; };
    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // �X�e�[�g�ύX
    void ChangeState(State::ChangeState& chage_state);
    // �f�t�H���g�X�e�[�g�̐ݒ�
    void SetDefaultState(std::string default_name);

    // �X�e�[�g�𖼑O��������
    State* FindState(MyHash name);
    // �X�e�[�g�̃C���f�b�N�X�𖼑O��������
    State::StateIndex FindStateIndex(MyHash name);
    
    // �X�e�[�g�o�^
    template<is_State State>
    void RegisterState()
    {
        std::unique_ptr<State> state = std::make_unique<State>();
        state->SetOwner(GetOwner());
        state->SetStateIndex(this->state_pool.size());
#ifdef _DEBUG
        this->state_name_pool.emplace_back(state->GetHash().GetString());
#endif // _DEBUG
        this->state_pool.emplace_back(std::move(state));
    };
private:
    State* current_state = nullptr;                     // ���݂̃X�e�[�g
    std::vector<std::unique_ptr<State>> state_pool;     // �e�X�e�[�g��ێ�����z��
    State::ChangeState default_state;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitive�����݂��邩
private:
    std::vector<std::string> state_name_pool;                   // �X�e�[�g�̖��O�̔z��
#endif // DEBUG
};

