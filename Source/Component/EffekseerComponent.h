#pragma once

#include <string>
#include "Component\Component.h"

class EffekseerComponent : public Component
{
public:
    struct EffectParam
    {
        std::string filename;               // �t�@�C����
        std::string key;                    // �G�t�F�N�g���Ǘ����鎞�̃L�[
        bool follow_position = true;         // �G�t�F�N�g���I�u�W�F�N�g�̈ʒu�ɒǏ]�����邩�ǂ���
        bool align_rotation = false;         // �G�t�F�N�g���I�u�W�F�N�g�̌����ɉ�]�����邩�ǂ���
    };
public:
    EffekseerComponent() {};
    ~EffekseerComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "EffekseerComponent"; };
    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    //void PlayEffect(std::string key);

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override {};
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