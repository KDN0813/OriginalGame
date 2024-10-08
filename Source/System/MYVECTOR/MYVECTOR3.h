#pragma once
#include <DirectXMath.h>

class MYVECTOR3 
{
public:
    MYVECTOR3() : vector(DirectX::XMVectorZero()) {}
    MYVECTOR3(float x, float y = 0.0f, float z = 0.0f)
    {
        this->vector = DirectX::XMVectorSet(x, y, z, 0.0f);
    }
    MYVECTOR3(DirectX::XMFLOAT3 f3)
    {
        this->vector = DirectX::XMVectorSet(f3.x, f3.y, f3.z, 0.0f);
    }
    MYVECTOR3(DirectX::XMVECTOR vec) : vector(vec) {}
    MYVECTOR3(const MYVECTOR3& my_vector) :vector(my_vector.vector) {}; // �R�s�[�R���X�g���N�^

    DirectX::XMVECTOR GetVector()
    {
        return this->vector;
    }
    DirectX::XMFLOAT3 GetFlaot3()
    {
        DirectX::XMFLOAT3 f3;
        DirectX::XMStoreFloat3(&f3, this->vector);
        return f3;
    }
    float GetX() const 
    {
        return DirectX::XMVectorGetX(this->vector);
    }
    float GetY() const 
    {
        return DirectX::XMVectorGetY(this->vector);
    }
    float GetZ() const 
    {
        return DirectX::XMVectorGetZ(this->vector);
    }
    void SetVector(DirectX::XMVECTOR vec)
    {
        this->vector = vec;
    }
    void SetVector(DirectX::XMFLOAT3 f3)
    {
        this->vector = DirectX::XMLoadFloat3(&f3);
    }
    void SetX(float x)
    {
        DirectX::XMFLOAT3 f3 = GetFlaot3();
        f3.x = x;
        this->vector = DirectX::XMLoadFloat3(&f3);
    }
    void SetY(float y)
    {
        DirectX::XMFLOAT3 f3 = GetFlaot3();
        f3.y = y;
        this->vector = DirectX::XMLoadFloat3(&f3);
    }
    void SetZ(float z)
    {
        DirectX::XMFLOAT3 f3 = GetFlaot3();
        f3.z = z;
        this->vector = DirectX::XMLoadFloat3(&f3);
    }
    void ZeroClear()
    {
        vector = DirectX::XMVectorZero();
    }

    // �������v�Z
    float Length() const 
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3Length(this->vector));
    }
    // ���K��
    MYVECTOR3 Normalize() const
    {
        return MYVECTOR3(DirectX::XMVector3Normalize(this->vector));
    }

    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR3 const operator+(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorAdd(this->vector, other.GetVector()));
    }
    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR3 const operator-(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorSubtract(this->vector, other.GetVector()));
    }
    // ��Z
    MYVECTOR3 const operator/(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }

    // �X�J���[�{
    MYVECTOR3 const operator*(float scalar)
    {
        return MYVECTOR3(DirectX::XMVectorScale(this->vector, scalar));
    }

private:
    alignas(16) DirectX::XMVECTOR vector;  // ������XMVECTOR��ێ�
};

class T
{
    T()
    {
        MYVECTOR3 c = vector;
    }

     MYVECTOR3 vector;
};
