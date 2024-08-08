#pragma once

class GameObject;

class Component
{
protected:
    Component() :owner(nullptr), priority(-1) {};

public:
    /**
     * �X�V�֐�
     */
    virtual void Update(float elapsedTime) {};

    // �e�擾�E�ݒ�֐�
    const unsigned int GetPriority()const noexcept { return priority; }
    void SetOwner(GameObject* owner) { this->owner = owner; }

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
    GameObject* owner;
    unsigned int priority;//�D��x �����l�͍Œ�
};
