#pragma once
#include <memory>

class Object;

class Component
{
protected:
    Component() :owner(), priority(-1) {};
    virtual ~Component() {};

public:
    // �X�V�֐�
    virtual void Update(float elapsed_time) {};

    // ���O�擾
    virtual const char* GetName()const = 0;
    
    /**
     * \fn
     * \brief �K�v�ȃR���|�[�l���g��weak_ptr�Ŏ擾����B
     * �R���X�g���N�^�Ŏ擾����ƒǉ����������邽�ߒǉ����I����Ă���
     * ���̊֐����Ăяo��
     */
    virtual void GetWeakComponent() {};
    
    // �e�擾�E�ݒ�֐�
    const unsigned int GetPriority()const noexcept { return this->priority; }
    void SetOwner(std::shared_ptr<Object> owner) { this->owner = owner; }
    std::shared_ptr<Object> GetOwner() { return this->owner.lock(); }

private:
    std::weak_ptr<Object>	owner;
    unsigned int priority;//�D��x �����l�͍Œ�

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

protected:
    bool is_active = false;
#endif // _DEBUG
};
