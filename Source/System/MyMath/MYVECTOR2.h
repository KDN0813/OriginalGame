#pragma once
#include <DirectXMath.h>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


class MYVECTOR2
{
public:
    MYVECTOR2() : vector(DirectX::XMVectorZero()) {}
    MYVECTOR2(float x, float y)
    {
        this->vector = DirectX::XMVectorSet(x, y, 0.0f, 0.0f);
    }
    MYVECTOR2(DirectX::XMFLOAT2 f2)
    {
        this->vector = DirectX::XMVectorSet(f2.x, f2.y, 0.0f, 0.0f);
    }
    MYVECTOR2(DirectX::XMVECTOR vec) : vector(vec) {}
    MYVECTOR2(const MYVECTOR2& mVec) :vector(mVec.vector) {}; // �R�s�[�R���X�g���N�^

    DirectX::XMVECTOR GetVector()
    {
        return this->vector;
    }
    DirectX::XMFLOAT2 GetFlaot2()
    {
        DirectX::XMFLOAT2 f2;
        DirectX::XMStoreFloat2(&f2, this->vector);
        return f2;
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
    void SetVector(DirectX::XMFLOAT2 f2)
    {
        this->vector = DirectX::XMLoadFloat2(&f2);
    }
    void SetX(float x)
    {
        DirectX::XMFLOAT2 f2 = GetFlaot2();
        f2.x = x;
        this->vector = DirectX::XMLoadFloat2(&f2);
    }
    void SetY(float y)
    {
        DirectX::XMFLOAT2 f2 = GetFlaot2();
        f2.y = y;
        this->vector = DirectX::XMLoadFloat2(&f2);
    }

    void AddX(float x)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, DirectX::XMVectorSet(x, 0.0f, 0.0f, 0.0f));
    }
    void AddY(float y)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, DirectX::XMVectorSet(0.0f, y, 0.0f, 0.0f));
    }
    void SubtractX(float x)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, DirectX::XMVectorSet(x, 0.0f, 0.0f, 0.0f));
    }
    void SubtractY(float y)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, DirectX::XMVectorSet(0.0f, y, 0.0f, 0.0f));
    }
    void MultiplyX(float x)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(x, 1.0f, 1.0f, 1.0f));
    }
    void MultiplyY(float y)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, y, 1.0f, 1.0f));
    }
    void DivideX(float x)
    {
        this->vector = DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(x, 1.0f, 1.0f, 1.0f));
    }
    void DivideY(float y)
    {
        this->vector = DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(1.0f, y, 1.0f, 1.0f));
    }

    void ZeroClear()
    {
        vector = DirectX::XMVectorZero();
    }
    void ClearToValu(float valu)
    {
        vector = DirectX::XMVectorSet(valu, valu, 0.0f, 0.0f);
    }

    // �������v�Z
    float Length() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector2Length(this->vector));
    }
    float LengthSq() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(this->vector));
    }
    // ���K��
    MYVECTOR2 Normalize() const
    {
        return MYVECTOR2(DirectX::XMVector2Normalize(this->vector));
    }
    // �e�l�̐�Βl���v�Z
    MYVECTOR2 Abs() const
    {
        return MYVECTOR2(DirectX::XMVectorAbs(this->vector));
    }
    // ���l��؂�グ�Đ����ɂ���
    MYVECTOR2 Ceiling() const
    {
        return MYVECTOR2(DirectX::XMVectorCeiling(this->vector));
    }
    // ���l��؂�̂ĂĐ����ɂ���
    MYVECTOR2 Floor() const
    {
        return MYVECTOR2(DirectX::XMVectorFloor(this->vector));
    }
    // �����𔽓]������
    MYVECTOR2 Negate() const
    {
        return MYVECTOR2(DirectX::XMVectorNegate(this->vector));
    }
    // �ł��߂������Ɋۂ߂�
    MYVECTOR2 Round() const
    {
        return MYVECTOR2(DirectX::XMVectorRound(this->vector));
    }
    // 0.0f �` 1.0f�ɔ[�߂�
    MYVECTOR2 Saturate() const
    {
        return MYVECTOR2(DirectX::XMVectorSaturate(this->vector));
    }
    // ���������v�Z
    MYVECTOR2 Sqrt() const
    {
        return MYVECTOR2(DirectX::XMVectorSqrt(this->vector));
    }
    float Sum() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVectorSum(DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0))));
    }

    // �N�����v
    MYVECTOR2 Clamp(MYVECTOR2 min, MYVECTOR2 max) const
    {
        return DirectX::XMVectorClamp(this->vector, min.GetVector(), max.GetVector());
    }
    MYVECTOR2 Min(MYVECTOR2 min) const
    {
        return DirectX::XMVectorMin(this->vector, min.GetVector());
    }
    MYVECTOR2 Max(MYVECTOR2 max) const
    {
        return DirectX::XMVectorMax(this->vector, max.GetVector());
    }
    // ��]���Z
    MYVECTOR2 Mod(MYVECTOR2 mVec) const
    {
        assert(mVec.LengthSq());
        return DirectX::XMVectorMod(this->vector, mVec.GetVector());
    }
    // �ݏ�
    MYVECTOR2 Pow(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(DirectX::XMVectorPow(this->vector, mVec.GetVector()));
    }
    // �ݏ�
    MYVECTOR2 Pow(float f) const
    {
        return MYVECTOR2(DirectX::XMVectorPow(this->vector, DirectX::XMVectorSet(f, f, 0.0f, 0.0f)));
    }
    // ���όv�Z
    MYVECTOR2 Dot(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(DirectX::XMVector2Dot(this->vector, mVec.GetVector()));
    }
    // �O�όv�Z
    MYVECTOR2 Cross(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(DirectX::XMVector2Cross(this->vector, mVec.GetVector()));
    }
    // ���`���
    MYVECTOR2 Lerp(MYVECTOR2 mVec, float t) const
    {
        return MYVECTOR2(DirectX::XMVectorLerp(this->vector, mVec.GetVector(), t));
    }

    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR2 const operator+(MYVECTOR2 other) const
    {
        return MYVECTOR2(DirectX::XMVectorAdd(this->vector, other.GetVector()));
    }
    MYVECTOR2 const operator+=(MYVECTOR2 other)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, other.GetVector());
        return *this;
    }
    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR2 const operator-(MYVECTOR2 other) const
    {
        return MYVECTOR2(DirectX::XMVectorSubtract(this->vector, other.GetVector()));
    }
    MYVECTOR2 const operator-=(MYVECTOR2 other)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, other.GetVector());
        return *this;
    }
    // ��Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR2 operator*(MYVECTOR2 other) const
    {
        return MYVECTOR2(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }
    MYVECTOR2 operator*=(MYVECTOR2 other)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, other.GetVector());
        return *this;
    }
    // �X�J���[�{ 
    MYVECTOR2 const operator*(float scalar) const
    {
        return MYVECTOR2(DirectX::XMVectorScale(this->vector, scalar));
    }
    MYVECTOR2 const operator*=(float scalar)
    {
        this->vector = DirectX::XMVectorScale(this->vector, scalar);
        return *this;
    }
    // ���Z���Z�q�̃I�[�o�[���[�h
    MYVECTOR2 const operator/(MYVECTOR2 other) const
    {
        assert(other.LengthSq());
        return MYVECTOR2(DirectX::XMVectorDivide(this->vector, other.GetVector()));
    }
    MYVECTOR2 const operator/=(MYVECTOR2 other)
    {
        assert(other.LengthSq());
        this->vector = DirectX::XMVectorDivide(this->vector, other.GetVector());
        return *this;
    }

private:
    alignas(16) DirectX::XMVECTOR vector;  // ������XMVECTOR��ێ�

public:
#ifdef _DEBUG
        bool InputFloat(const char* label)
        {
            bool r = false;
            DirectX::XMFLOAT2 f2 = GetFlaot2();
            r = ImGui::InputFloat4(label, &f2.x);
            if (r)
            {
                SetVector(f2);
            }
            return r;
        }
        bool DragFloat(const char* label, float rate = 1.0f)
        {
            bool r = false;
            DirectX::XMFLOAT2 f2 = GetFlaot2();
            r = ImGui::DragFloat4(label, &f2.x, rate);
            if (r)
            {
                SetVector(f2);
            }
            return r;
        }
        bool SliderFloat(const char* label, float min, float max)
        {
            bool r = false;
            DirectX::XMFLOAT2 f2 = GetFlaot2();
            r = ImGui::SliderFloat4(label, &f2.x, min, max);
            if (r)
            {
                SetVector(f2);
            }
            return r;
        }
#endif // _DEBUG
};