#pragma once
#include "Component.h"

class MovementComponent;

class GravityComponent : public Component
{
public:
    struct GravityParam
    {
        float gravity = -0.5f;   // �d�͉����x
        bool is_grounded = false;// �n�ʂɂ��邩
    }; 
public:
    GravityComponent(GravityParam param) : param(param), default_param(param){};

    // ���X�^�[�g����
    void ReStart() override { this->param = this->default_param; };      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time);

    // ���O�擾
    const char* GetName()const { return "GravityComponent"; };
    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::VERY_HIGH; };


    // �e��擾�E�ݒ�֐�
    float GetGravity() { return this->param.gravity; }
    bool GetIsGrounded() { return this->param.is_grounded; }
    void SetIsGrounded(bool is_grounded) { this->param.is_grounded = is_grounded; }
private:
    GravityParam param;
    GravityParam default_param;
private:
    std::weak_ptr<MovementComponent> movement_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};