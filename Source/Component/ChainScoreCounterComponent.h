#pragma once
#include "Component.h"
#include <functional>
#include <vector>

// �A���X�R�A�𐧌䂷��R���|�[�l���g
class ChainScoreCounterComponent : public Component
{
public:
    

    struct Param
    {
        int chain_score = 0;           
        float chain_timer = 0.0f;      
        float chain_timer_max = 0.0f;  // �A���̗P�\����
    };
public:
    ChainScoreCounterComponent(Param param) : param(param) {};
    ~ChainScoreCounterComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "ChainScoreCounterComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void ChainEnd();

    void AddOnScoreAdded(const std::function<void(int)>& function) { this->on_score_added_pool.emplace_back(function); };
    void AddOnScoreChainStart(const std::function<void()>& function) { this->on_score_chain_start_pool.emplace_back(function); };
    void AddOnScoreChainEnd(const std::function<void()>& function) { this->on_score_chain_end_pool.emplace_back(function); };

    // �X�R�A�����Z����
    void AddChain(int add_score);
private:
    Param param;

    std::vector<std::function<void(int)>> on_score_added_pool;        // �A���X�R�A�����Z���ꂽ���̃R�[���o�b�N�ϐ�
    std::vector<std::function<void()>> on_score_chain_start_pool;     // �A�����J�n�������̃R�[���o�b�N�ϐ�
    std::vector<std::function<void()>> on_score_chain_end_pool;       // �A�����I���������̃R�[���o�b�N�ϐ�
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
#endif // DEBUG
};

