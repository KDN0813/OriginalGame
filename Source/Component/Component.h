#pragma once
#include <memory>
#include "System/Misc.h"

class Object;

enum PRIORITY : unsigned int
{
    LOWEST = 0,         // �Œ�̗D��x
    //VERY_LOW,           // ���ɒႢ�D��x
    LOW,                // �Ⴂ�D��x
    //BELOW_MEDIUM,       // �����x��艺�̗D��x
    MEDIUM,             // �����x�̗D��x
    HIGH,               // �����D��x
    VERY_HIGH,          // ���ɍ����D��x
    CRITICAL,           // �ō��̗D��x�i�d�v�j
    DEFAULT = MEDIUM       // �f�t�H���g�D��x�𒆒��x�ɐݒ�
};

class Component
{
protected:
    Component(){};
    virtual ~Component() {};

public:
    // �J�n�֐�
    virtual void Start() {};        // Update���Ă΂�钼�O��1�x�����Ă΂��
    // �I���֐�
    virtual void End() {};          // �j�������Ƃ��ɌĂ΂��     
    // �X�V�֐�
    virtual void Update(float elapsed_time) {};
    // ���X�^�[�g����
    virtual void ReStart() {};      // �p�����[�^�̏�����

    // ���O�擾
    virtual const char* GetName()const = 0;

    // �D��x
    virtual const PRIORITY GetPriority()const noexcept { return PRIORITY::DEFAULT; };
    
    // ���I�u�W�F�N�g�ɐڐG�������̏���
    virtual void OnCollision(const std::shared_ptr<Object>& hit_object) {};

    // �e�擾�E�ݒ�֐�
    void SetOwner(std::shared_ptr<Object> owner) { this->owner_Wptr = owner; }
    std::shared_ptr<Object> GetOwner() const 
    {
        std::shared_ptr<Object> owner = this->owner_Wptr.lock();
        _ASSERT_EXPR_W((owner != nullptr) , L"���L�҂̃|�C���^��null�ł�");
        return owner;
    }
    const unsigned int GetComponentID() { return this->component_id; }
    void SetComponentID(const int unique_id) { this->component_id = unique_id; }
    bool GetIsActive() { return this->is_active; }
    void SetIsActive(const bool is_active) 
    {
        this->is_active = is_active; 
    }
    bool IsOwner() const { return (this->owner_Wptr.lock() != nullptr); }   // ���L�҂����݂��邩
private:
    std::weak_ptr<Object>	owner_Wptr = {};
    unsigned int component_id = {};
protected:
    bool is_active = true;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    virtual void DrawDebugGUI() = 0;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    virtual void DrawDebugPrimitive() {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    virtual void DrawDebugPrimitiveGUI() {};

    bool GetIsDebugPrimitive() { return this->is_draw_debug_primitive; }
    void SetIsDebugPrimitive(const bool is_draw_debug_primitive) { this->is_draw_debug_primitive = is_draw_debug_primitive; }
    virtual bool IsDebugPrimitive() { return false; }   // DebugPrimitive�����݂��邩

protected:
    bool is_draw_debug_primitive = true;
#endif // _DEBUG
#ifdef RELEASE_DEBUG
public:
    // �f�o�b�O�p�`�[�g
    virtual void DebugCheat() {};
#endif // RELEASE_DEBUG

};

class DefaultComponent : public Component
{
public:
    DefaultComponent() {};
    ~DefaultComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override{};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override {};

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

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