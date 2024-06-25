#pragma once
#include <DirectXMath.h>

// �J�����R���g���[���[[02]
class CameraController
{
public:
    //[02]
    CameraController(){}
    ~CameraController(){}

    // �X�V����[02]
    void Update(float elapsedTime);

    // �^�[�Q�b�g�ʒu�ݒ�[02]
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    // �f�o�b�O�pGUI�`��[org]
    void DrawDebugGUI();

    // ���N��Ԃ��擾[13]
    int GetHealth()const { return health; }

    // �ő匒�N��Ԃ��擾[13]
    int GetMaxHealth() const { return maxHealth; }


protected:

    // [13]
    int health                  = 5;
    int maxHealth               = 5;
private:
    //[02]
    DirectX::XMFLOAT3 target    = { 0,0,0 };
    DirectX::XMFLOAT3 angle     = { 0,0,0 };
    float rollSpeed             = DirectX::XMConvertToRadians(90);
    float range                 = 10.0f;

    float maxAngleX             = DirectX::XMConvertToRadians(45);
    float minAngleX             = DirectX::XMConvertToRadians(-45);
};

