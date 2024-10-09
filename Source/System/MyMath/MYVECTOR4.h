#pragma once
#include <DirectXMath.h>

class MYVECTOR4
{
public:
    MYVECTOR4() : vector(DirectX::XMVectorZero()) {}
    MYVECTOR4(float x, float y = 0.0f, float z = 0.0f, float w = 0.0f)
    {
        this->vector = DirectX::XMVectorSet(x, y, z, w);
    }
    MYVECTOR4(DirectX::XMFLOAT4 f4)
    {
        this->vector = DirectX::XMVectorSet(f4.x, f4.y, f4.z, f4.w);
    }
    MYVECTOR4(DirectX::XMVECTOR vec) : vector(vec) {}
    MYVECTOR4(const MYVECTOR4& mVec) :vector(mVec.vector) {}; // �R�s�[�R���X�g���N�^

    DirectX::XMVECTOR GetVector()
    {
        return this->vector;
    }
    DirectX::XMVECTOR* GetVectorAddress()
    {
        return &this->vector;
    }
    DirectX::XMFLOAT4 GetFlaot4()
    {
        DirectX::XMFLOAT4 f4;
        DirectX::XMStoreFloat4(&f4, this->vector);
        return f4;
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
    float GetW() const
    {
        return DirectX::XMVectorGetW(this->vector);
    }
    void SetVector(DirectX::XMVECTOR vec)
    {
        this->vector = vec;
    }
    void SetVector(DirectX::XMFLOAT4 f4)
    {
        this->vector = DirectX::XMLoadFloat4(&f4);
    }
    void SetX(float x)
    {
        DirectX::XMFLOAT4 f4 = GetFlaot4();
        f4.x = x;
        this->vector = DirectX::XMLoadFloat4(&f4);
    }
    void SetY(float y)
    {
        DirectX::XMFLOAT4 f4 = GetFlaot4();
        f4.y = y;
        this->vector = DirectX::XMLoadFloat4(&f4);
    }
    void SetZ(float z)
    {
        DirectX::XMFLOAT4 f4 = GetFlaot4();
        f4.z = z;
        this->vector = DirectX::XMLoadFloat4(&f4);
    }
    void SetW(float w)
    {
        DirectX::XMFLOAT4 f4 = GetFlaot4();
        f4.w = w;
        this->vector = DirectX::XMLoadFloat4(&f4);
    }

    void ZeroClear()
    {
        vector = DirectX::XMVectorZero();
    }
    void ClearToValu(float valu)
    {
        vector = DirectX::XMVectorSet(valu, valu, valu, 0.0f);
    }

    // �������v�Z
    float Length() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector4Length(this->vector));
    }
    float LengthSp() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(this->vector));
    }
    // ���K��
    MYVECTOR4 Normalize() const
    {
        return MYVECTOR4(DirectX::XMVector4Normalize(this->vector));
    }
    // �e�l�̐�Βl���v�Z
    MYVECTOR4 Abs() const
    {
        return MYVECTOR4(DirectX::XMVectorAbs(this->vector));
    }
    // ���l��؂�グ�Đ����ɂ���
    MYVECTOR4 Ceiling() const
    {
        return MYVECTOR4(DirectX::XMVectorCeiling(this->vector));
    }
    // ���l��؂�̂ĂĐ����ɂ���
    MYVECTOR4 Floor() const
    {
        return MYVECTOR4(DirectX::XMVectorFloor(this->vector));
    }
    // �����𔽓]������
    MYVECTOR4 Negate() const
    {
        return MYVECTOR4(DirectX::XMVectorNegate(this->vector));
    }
    // �ł��߂������Ɋۂ߂�
    MYVECTOR4 Round() const
    {
        return MYVECTOR4(DirectX::XMVectorRound(this->vector));
    }
    // 0.0f �` 1.0f�ɔ[�߂�
    MYVECTOR4 Saturate() const
    {
        return MYVECTOR4(DirectX::XMVectorSaturate(this->vector));
    }
    // ���������v�Z
    MYVECTOR4 Sqrt() const
    {
        return MYVECTOR4(DirectX::XMVectorSqrt(this->vector));
    }
    float Sum() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVectorSum(DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0))));
    }

    // �N�����v
    MYVECTOR4 Clamp(MYVECTOR4 min, MYVECTOR4 max) const
    {
        return DirectX::XMVectorClamp(this->vector, min.GetVector(), max.GetVector());
    }
    MYVECTOR4 Min(MYVECTOR4 min) const
    {
        return DirectX::XMVectorMin(this->vector, min.GetVector());
    }
    MYVECTOR4 Max(MYVECTOR4 max) const
    {
        return DirectX::XMVectorMax(this->vector, max.GetVector());
    }
    // ��]���Z
    MYVECTOR4 Mod(MYVECTOR4 mVec) const
    {
        assert(mVec.LengthSp());
        return DirectX::XMVectorMod(this->vector, mVec.GetVector());
    }
    // �ݏ�
    MYVECTOR4 Pow(MYVECTOR4 mVec) const
    {
        return MYVECTOR4(DirectX::XMVectorPow(this->vector, mVec.GetVector()));
    }
    // �ݏ�
    MYVECTOR4 Pow(float f) const
    {
        return MYVECTOR4(DirectX::XMVectorPow(this->vector, DirectX::XMVectorSet(f, f, f, 0.0f)));
    }
    // ���όv�Z
    float Dot(MYVECTOR4 mVec) const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector4Dot(this->vector, mVec.GetVector()));
    }
    // ���`���
    MYVECTOR4 Lerp(MYVECTOR4 mVec, float t) const
    {
        return MYVECTOR4(DirectX::XMVectorLerp(this->vector, mVec.GetVector(), t));
    }
    // ���ʐ��`���
    MYVECTOR4 SLerp(MYVECTOR4 mVec, float t) const
    {
        return MYVECTOR4(DirectX::XMQuaternionSlerp(this->vector, mVec.GetVector(), t));
    }

    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR4 const operator+(MYVECTOR4 other) const
    {
        return MYVECTOR4(DirectX::XMVectorAdd(this->vector, other.GetVector()));
    }
    MYVECTOR4 const operator+=(MYVECTOR4 other)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, other.GetVector());
        return *this;
    }
    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR4 const operator-(MYVECTOR4 other) const
    {
        return MYVECTOR4(DirectX::XMVectorSubtract(this->vector, other.GetVector()));
    }
    MYVECTOR4 const operator-=(MYVECTOR4 other)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, other.GetVector());
        return *this;
    }
    // ��Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR4 operator*(MYVECTOR4 other) const
    {
        return MYVECTOR4(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }
    MYVECTOR4 operator*=(MYVECTOR4 other)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, other.GetVector());
        return *this;
    }
    // �X�J���[�{
    MYVECTOR4 const operator*(float scalar) const
    {
        return MYVECTOR4(DirectX::XMVectorScale(this->vector, scalar));
    }
    MYVECTOR4 const operator*=(float scalar)
    {
        this->vector = DirectX::XMVectorScale(this->vector, scalar);
        return *this;
    }
    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR4 const operator/(MYVECTOR4 other) const
    {
        assert(other.LengthSp());
        return MYVECTOR4(DirectX::XMVectorDivide(this->vector, other.GetVector()));
    }
    MYVECTOR4 const operator/=(MYVECTOR4 other)
    {
        assert(other.LengthSp());
        this->vector = DirectX::XMVectorDivide(this->vector, other.GetVector());
        return *this;
    }

#pragma region VectorXZ    
#pragma endregion VectorXZ
private:
    alignas(16) DirectX::XMVECTOR vector;  // ������XMVECTOR��ێ�
};