#pragma once
#include <memory>

class Object;

class Component
{
protected:
    Component(){};
    virtual ~Component() {};

public:
    // �X�V�֐�
    virtual void Update(float elapsed_time) {};

    // ���O�擾
    virtual const char* GetName()const = 0;
    
    // �e�擾�E�ݒ�֐�
    const unsigned int GetPriority()const noexcept { return this->priority; }
    void SetOwner(std::shared_ptr<Object> owner) { this->owner = owner; }
    std::shared_ptr<Object> GetOwner() { return this->owner.lock(); }
    const unsigned int GetComponentID() { return this->component_id; }
    void SetComponentID(const int unique_id) { this->component_id = unique_id; }

private:
    std::weak_ptr<Object>	owner = {};
    unsigned int priority = -1;//�D��x �����l�͍Œ�
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

    const bool GetIsActive() { return this->is_active; }
    void SetIsActive(const bool is_active) { this->is_active = is_active; }

protected:
    bool is_active = false;
#endif // _DEBUG
};