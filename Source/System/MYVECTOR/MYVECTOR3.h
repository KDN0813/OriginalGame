#pragma once
#include <DirectXMath.h>
#include "MYVECTOR2.h"

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
    void ClearToValu(float valu)
    {
        vector = DirectX::XMVectorSet(valu, valu, valu, 0.0f);
    }

    // �������v�Z
    float Length() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3Length(this->vector));
    }
    float LengthSp() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(this->vector));
    }
    // ���K��
    MYVECTOR3 Normalize() const
    {
        return MYVECTOR3(DirectX::XMVector3Normalize(this->vector));
    }
    // �e�l�̐�Βl���v�Z
    MYVECTOR3 Abs() const
    {
        return MYVECTOR3(DirectX::XMVectorAbs(this->vector));
    }
    // ���l��؂�グ�Đ����ɂ���
    MYVECTOR3 Ceiling() const
    {
        return MYVECTOR3(DirectX::XMVectorCeiling(this->vector));
    }
    // ���l��؂�̂ĂĐ����ɂ���
    MYVECTOR3 Floor() const
    {
        return MYVECTOR3(DirectX::XMVectorFloor(this->vector));
    }
    // �����𔽓]������
    MYVECTOR3 Negate() const
    {
        return MYVECTOR3(DirectX::XMVectorNegate(this->vector));
    }
    // �ł��߂������Ɋۂ߂�
    MYVECTOR3 Round() const
    {
        return MYVECTOR3(DirectX::XMVectorRound(this->vector));
    }
    // 0.0f �` 1.0f�ɔ[�߂�
    MYVECTOR3 Saturate() const
    {
        return MYVECTOR3(DirectX::XMVectorSaturate(this->vector));
    }
    // ���������v�Z
    MYVECTOR3 Sqrt() const
    {
        return MYVECTOR3(DirectX::XMVectorSqrt(this->vector));
    }
    float Sum() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVectorSum(DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0))));
    }

    // �N�����v
    MYVECTOR3 Clamp(MYVECTOR3 min, MYVECTOR3 max) const
    {
        return DirectX::XMVectorClamp(this->vector, min.GetVector(), max.GetVector());
    }
    MYVECTOR3 Min(MYVECTOR3 min) const
    {
        return DirectX::XMVectorMin(this->vector, min.GetVector());
    }
    MYVECTOR3 Max(MYVECTOR3 max) const
    {
        return DirectX::XMVectorMax(this->vector, max.GetVector());
    }
    // ��]���Z
    MYVECTOR3 Mod(MYVECTOR3 mVec) const
    {
        // TODO �G���[��������悤�ɕύX
        if (mVec.LengthSp() == 0.0f) return MYVECTOR3();

        return DirectX::XMVectorMod(this->vector, mVec.GetVector());
    }
    // �ݏ�
    MYVECTOR3 Pow(MYVECTOR3 mVec) const
    {
        return MYVECTOR3(DirectX::XMVectorPow(this->vector, mVec.GetVector()));
    }
    // �ݏ�
    MYVECTOR3 Pow(float f) const
    {
        return MYVECTOR3(DirectX::XMVectorPow(this->vector, DirectX::XMVectorSet(f, f, f, 0.0f)));
    }
    // ���όv�Z
    MYVECTOR3 Dot(MYVECTOR3 mVec) const
    {
        return MYVECTOR3(DirectX::XMVector3Dot(this->vector, mVec.GetVector()));
    }
    // �O�όv�Z
    MYVECTOR3 Cross(MYVECTOR3 mVec) const
    {
        return MYVECTOR3(DirectX::XMVector3Cross(this->vector, mVec.GetVector()));
    }
    // ���`���
    MYVECTOR3 Lerp(MYVECTOR3 mVec, float t) const
    {
        return MYVECTOR3(DirectX::XMVectorLerp(this->vector, mVec.GetVector(), t));
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
    // ��Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR3 const operator*(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }
    // �X�J���[�{
    MYVECTOR3 const operator*(float scalar)
    {
        return MYVECTOR3(DirectX::XMVectorScale(this->vector, scalar));
    }
    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR3 const operator/(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorDivide(this->vector, other.GetVector()));
    }

#pragma region VectorXZ
    // TODO �쐬����
    
    MYVECTOR2 GetVecXZ()
    {
        return MYVECTOR2(GetX(), GetZ());
    }
    DirectX::XMFLOAT2 GetFlaot2XZ()
    {
        return GetVecXZ().GetFlaot2();
    }

    // �������v�Z
    float LengthXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Length();
    };
    float LengthSpXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).LengthSp();
    }
    // ���K��
    MYVECTOR2 NormalizeXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Normalize();
    }
    // �e�l�̐�Βl���v�Z
    MYVECTOR2 AbsXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Abs();
    }
    // ���l��؂�グ�Đ����ɂ���
    MYVECTOR2 CeilingXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Ceiling();
    }
    // ���l��؂�̂ĂĐ����ɂ���
    MYVECTOR2 FloorXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Floor();
    }
    // �����𔽓]������
    MYVECTOR2 NegateXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Negate();
    }
    // �ł��߂������Ɋۂ߂�
    MYVECTOR2 RoundXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Round();
    }
    // 0.0f �` 1.0f�ɔ[�߂�
    MYVECTOR2 SaturateXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Saturate();
    }
    // ���������v�Z
    MYVECTOR2 SqrtXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Sqrt();
    }
    float SumXZ() const
    {
        return MYVECTOR2(GetX(), GetZ()).Sum();
    }

    // �N�����v
    MYVECTOR2 ClampXZ(MYVECTOR2 min, MYVECTOR2 max) const
    {
        return MYVECTOR2(GetX(), GetZ()).Clamp(min, max);
    }
    MYVECTOR2 MinXZ(MYVECTOR2 min) const
    {
        return MYVECTOR2(GetX(), GetZ()).Min(min);
    }
    MYVECTOR2 MaxXZ(MYVECTOR2 max) const
    {
        return MYVECTOR2(GetX(), GetZ()).Max(max);
    }
    // ��]���Z
    MYVECTOR2 ModXZ(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(GetX(), GetZ()).Max(mVec);
    }
    // �ݏ�
    MYVECTOR2 PowXZ(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(GetX(), GetZ()).Pow(mVec);
    }
    // �ݏ�
    MYVECTOR2 PowXZ(float f) const
    {
        return MYVECTOR2(GetX(), GetZ()).Pow(f);
    }
    // ���όv�Z
    MYVECTOR2 DotXZ(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(GetX(), GetZ()).Dot(mVec);
    }
    // �O�όv�Z
    MYVECTOR2 CrossXZ(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(GetX(), GetZ()).Cross(mVec);
    }
    // ���`���
    MYVECTOR2 LerpXZ(MYVECTOR2 mVec, float t) const
    {
        return MYVECTOR2(GetX(), GetZ()).Lerp(mVec, t);
    }
#pragma endregion VectorXZ
private:
    alignas(16) DirectX::XMVECTOR vector;  // ������XMVECTOR��ێ�
};