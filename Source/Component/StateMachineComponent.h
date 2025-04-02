#pragma once
#include <unordered_map>
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
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::CRITICAL; };

    // �X�e�[�g�ύX
    void ChangeState(std::string state_key);
    // �f�t�H���g�X�e�[�g�̐ݒ�
    void SetDefaultState(std::string state_key);

    // �X�e�[�g�𖼑O��������
    State* FindState(std::string state_key);
    
    // �X�e�[�g�o�^
    template<is_State State>
    void RegisterState(std::string state_key)
    {
#ifdef _DEBUG
        this->state_name_pool.emplace_back(state_key);
#endif // _DEBUG
        this->state_pool[state_key] = std::make_unique<State>();
        this->state_pool[state_key]->SetOwner(GetOwner());
    };
private:
    State* current_state = nullptr;                                         // ���݂̃X�e�[�g
    std::unordered_map<std::string, std::unique_ptr<State>> state_pool;     // �e�X�e�[�g��ێ�����z��
    std::string default_state_key;

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

