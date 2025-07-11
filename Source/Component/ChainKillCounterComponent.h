#pragma once
#include "Component.h"
#include <functional>

// �A�����j���𐧌䂷��R���|�[�l���g
class ChainKillCounterComponent : public Component
{
public:
    struct Param
    {
        int chain_kill_count = 0;           // �A�����j��
        float chain_kill_timer = 0.0f;      // �A�����j�̎c��P�\����
        float chain_kill_timer_max = 0.0f;  // �A�����j�̗P�\���Ԃ̍ő�l
        bool is_reset = false;              // �J�E���g���Z�b�g��1�x�������s�����悤���䂷��t���O
    };
public:
    ChainKillCounterComponent(Param param) : param(param) {};
    ~ChainKillCounterComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "ChainKillCounterComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void CountReset();

    void SetOnKillCountAdded(const std::function<void(int)>& function) { this->onKillCountAdded = function; };  // �L���J�E���g�̑���
    void SetOnChainKillEnded(const std::function<void()>& function) { this->onKillCountReset = function; };     // �L���J�E���g�I��

    // �A�����j�������Z����
    void AddChainKill();
private:
    Param param;

    std::function<void(int)> onKillCountAdded;
    std::function<void()> onKillCountReset;
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

