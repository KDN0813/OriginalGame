#pragma once
#include <memory>

class Object;

class Component
{
protected:
    Component() :owner(), priority(-1) {};
    virtual ~Component() {};

public:
    /**
     * �X�V�֐�
     */
    virtual void Update(float elapsedTime) {};

    // �e�擾�E�ݒ�֐�
    const unsigned int GetPriority()const noexcept { return this->priority; }
    void SetOwner(std::shared_ptr<Object> owner) { this->owner = owner; }
    std::shared_ptr<Object> GetActor() { return this->owner.lock(); }

#ifdef _DEBUG
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    virtual void DrawDebugGUI() {};
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    virtual void DrawDebugPrimitive() {};
#endif // _DEBUG
private:
    std::weak_ptr<Object>	owner;
    unsigned int priority;//�D��x �����l�͍Œ�
};
