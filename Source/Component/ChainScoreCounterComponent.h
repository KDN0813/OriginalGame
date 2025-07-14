#pragma once
#include "Component.h"
#include <functional>

// �A���擾�X�R�A�𐧌䂷��R���|�[�l���g
class ChainScoreCounterComponent : public Component
{
public:
    struct Param
    {
        int chain_score = 0;           
        float chain_timer = 0.0f;      
        float chain_timer_max = 0.0f;  
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

    void SetOnScoreAdded(const std::function<void(int)>& function) { this->OnScoreAdded = function; };   
    void SetOnScoreChainStart(const std::function<void()>& function) { this->OnScoreChainStart = function; };
    void SetOnScoreChainEnd(const std::function<void()>& function) { this->OnScoreChainEnd = function; };

    // �X�R�A�����Z����
    void AddChain(int add_score);
private:
    Param param;

    std::function<void(int)> OnScoreAdded;
    std::function<void()> OnScoreChainStart;
    std::function<void()> OnScoreChainEnd;
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

