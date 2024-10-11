#pragma once
#include <memory>

class Object;

enum COMPONENT_PRIORITY : unsigned int
{
    LOWEST = 0,         // �Œ�̗D��x
    //VERY_LOW,           // ���ɒႢ�D��x
    LOW,                // �Ⴂ�D��x
    //BELOW_MEDIUM,       // �����x��艺�̗D��x
    MEDIUM,             // �����x�̗D��x
    HIGH,               // �����D��x
    VERY_HIGH,          // ���ɍ����D��x
    CRITICAL,           // �ō��̗D��x�i�d�v�j
    DEFAULT = MEDIUM    // �f�t�H���g�D��x�𒆒��x�ɐݒ�
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

    // ���O�擾
    virtual const char* GetName()const = 0;

    // �D��x
    virtual const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::DEFAULT; };
    
    // �e�擾�E�ݒ�֐�
    void SetOwner(std::shared_ptr<Object> owner) { this->owner = owner; }
    std::shared_ptr<Object> GetOwner() { return this->owner.lock(); }
    const unsigned int GetComponentID() { return this->component_id; }
    void SetComponentID(const int unique_id) { this->component_id = unique_id; }

private:
    std::weak_ptr<Object>	owner = {};
    unsigned int component_id = {};

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

    bool GetIsActive() { return this->is_active; }
    void SetIsActive(const bool is_active) { this->is_active = is_active; }
    bool GetIsDebugPrimitive() { return this->is_draw_debug_primitive; }
    void SetIsDebugPrimitive(const bool is_draw_debug_primitive) { this->is_draw_debug_primitive = is_draw_debug_primitive; }
    virtual bool IsDebugPrimitive() { return false; }   // DebugPrimitive�����݂��邩

protected:
    bool is_active = true;
    bool is_draw_debug_primitive = true;
#endif // _DEBUG
};