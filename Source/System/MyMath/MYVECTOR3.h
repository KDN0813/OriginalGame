#pragma once
#include "System/MyMath/MYVECTOR2.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


class MYVECTOR3
{
public:
    MYVECTOR3() : vector(DirectX::XMVectorZero()) {}
    MYVECTOR3(float x, float y, float z)
    {
        this->vector = DirectX::XMVectorSet(x, y, z, 0.0f);
    }
    MYVECTOR3(DirectX::XMFLOAT3 f3)
    {
        this->vector = DirectX::XMVectorSet(f3.x, f3.y, f3.z, 0.0f);
    }
    MYVECTOR3(DirectX::XMVECTOR vec) : vector(vec) {}
    MYVECTOR3(const MYVECTOR3& mVec) :vector(mVec.vector) {}; // コピーコンストラクタ

    DirectX::XMVECTOR GetVector() const
    {
        return this->vector;
    }
    DirectX::XMVECTOR* GetVectorAddress()
    {
        return &this->vector;
    }
    void GetFlaot3(DirectX::XMFLOAT3& f3) const
    {
        DirectX::XMStoreFloat3(&f3, this->vector);
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

    void AddX(float x)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, DirectX::XMVectorSet(x, 0.0f, 0.0f, 0.0f));
    }
    void AddY(float y)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, DirectX::XMVectorSet(0.0f, y, 0.0f, 0.0f));
    }
    void AddZ(float z)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, DirectX::XMVectorSet(0.0f, 0.0f, z, 0.0f));
    }
    void AddXZ(float x, float z)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, DirectX::XMVectorSet(x, 0.0f, z, 0.0f));
    }
    void SubtractX(float x)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, DirectX::XMVectorSet(x, 0.0f, 0.0f, 0.0f));
    }
    void SubtractY(float y)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, DirectX::XMVectorSet(0.0f, y, 0.0f, 0.0f));
    }
    void SubtractZ(float z)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, DirectX::XMVectorSet(0.0f, 0.0f, z, 0.0f));
    }
    void SubtractXZ(float x, float z)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, DirectX::XMVectorSet(x, 0.0f, z, 0.0f));
    }
    void MultiplyX(float x)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(x, 1.0f, 1.0f, 1.0f));
    }
    void MultiplyY(float y)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, y, 1.0f, 1.0f));
    }
    void MultiplyZ(float z)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, z, 1.0f));
    }
    void MultiplyXZ(float x, float z)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(x, 1.0f, z, 1.0f));
    }
    void DivideX(float x)
    {
        this->vector = DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(x, 1.0f, 1.0f, 1.0f));
    }
    void DivideY(float y)
    {
        this->vector = DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(1.0f, y, 1.0f, 1.0f));
    }
    void DivideZ(float z)
    {
        this->vector = DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, z, 1.0f));
    }
    void DivideXZ(float x, float z)
    {
        this->vector = DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(x, 1.0f, z, 1.0f));
    }

    void ZeroClear()
    {
        vector = DirectX::XMVectorZero();
    }
    void ClearToValu(float valu)
    {
        vector = DirectX::XMVectorSet(valu, valu, valu, 0.0f);
    }

    // 長さを計算
    float Length() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3Length(this->vector));
    }
    float LengthSq() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(this->vector));
    }
    // 正規化
    MYVECTOR3 Normalize() const
    {
        return MYVECTOR3(DirectX::XMVector3Normalize(this->vector));
    }
    void NormalizeSelf()
    {
        this->vector = DirectX::XMVector3Normalize(this->vector);
    }
    // 各値の絶対値を計算
    MYVECTOR3 Abs() const
    {
        return MYVECTOR3(DirectX::XMVectorAbs(this->vector));
    }
    // 数値を切り上げて整数にする
    MYVECTOR3 Ceiling() const
    {
        return MYVECTOR3(DirectX::XMVectorCeiling(this->vector));
    }
    // 数値を切り捨てて整数にする
    MYVECTOR3 Floor() const
    {
        return MYVECTOR3(DirectX::XMVectorFloor(this->vector));
    }
    // 符号を反転させる
    MYVECTOR3 Negate() const
    {
        return MYVECTOR3(DirectX::XMVectorNegate(this->vector));
    }
    // 最も近い整数に丸める
    MYVECTOR3 Round() const
    {
        return MYVECTOR3(DirectX::XMVectorRound(this->vector));
    }
    // 0.0f ～ 1.0fに納める
    MYVECTOR3 Saturate() const
    {
        return MYVECTOR3(DirectX::XMVectorSaturate(this->vector));
    }
    // 平方根を計算
    MYVECTOR3 Sqrt() const
    {
        return MYVECTOR3(DirectX::XMVectorSqrt(this->vector));
    }
    float Sum() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVectorSum(DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0))));
    }

    // クランプ
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
    // 剰余演算
    MYVECTOR3 Mod(MYVECTOR3 mVec) const
    {
        assert(mVec.LengthSq());
        return DirectX::XMVectorMod(this->vector, mVec.GetVector());
    }
    // 累乗
    MYVECTOR3 Pow(MYVECTOR3 mVec) const
    {
        return MYVECTOR3(DirectX::XMVectorPow(this->vector, mVec.GetVector()));
    }
    // 累乗
    MYVECTOR3 Pow(float f) const
    {
        return MYVECTOR3(DirectX::XMVectorPow(this->vector, DirectX::XMVectorSet(f, f, f, 0.0f)));
    }
    // 内積計算
    float Dot(MYVECTOR3 mVec) const
    {
       return DirectX::XMVectorGetX(DirectX::XMVector3Dot(this->vector, mVec.GetVector()));
    }
    MYVECTOR3 DotVec3(MYVECTOR3 mVec) const
    {
        return DirectX::XMVector3Dot(this->vector, mVec.GetVector());
    }
    // 外積計算
    MYVECTOR3 Cross(MYVECTOR3 mVec) const
    {
        return MYVECTOR3(DirectX::XMVector3Cross(this->vector, mVec.GetVector()));
    }
    // 線形補間
    MYVECTOR3 Lerp(MYVECTOR3 mVec, float t) const
    {
        return MYVECTOR3(DirectX::XMVectorLerp(this->vector, mVec.GetVector(), t));
    }
    // 球面線形補間
    MYVECTOR3 SLerp(MYVECTOR3 mVec, float t) const
    {
        return MYVECTOR3(DirectX::XMQuaternionSlerp(this->vector, mVec.GetVector(), t));
    }

    // 加算演算子のオーバーロード
    MYVECTOR3 const operator+(MYVECTOR3 other) const
    {
        return MYVECTOR3(DirectX::XMVectorAdd(this->vector, other.GetVector()));
    }
    MYVECTOR3 const operator+=(MYVECTOR3 other)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, other.GetVector());
        return *this;
    }
    // 減算演算子のオーバーロード
    MYVECTOR3 const operator-(MYVECTOR3 other) const
    {
        return MYVECTOR3(DirectX::XMVectorSubtract(this->vector, other.GetVector()));
    }
    MYVECTOR3 const operator-=(MYVECTOR3 other)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, other.GetVector());
        return *this;
    }
    // 乗算演算子のオーバーロード
    MYVECTOR3 operator*(MYVECTOR3 other) const
    {
        return MYVECTOR3(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }
    MYVECTOR3 operator*=(MYVECTOR3 other)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, other.GetVector());
        return *this;
    }
    // スカラー倍
    MYVECTOR3 const operator*(float scalar) const
    {
        return MYVECTOR3(DirectX::XMVectorScale(this->vector, scalar));
    }
    MYVECTOR3 const operator*=(float scalar)
    {
        this->vector = DirectX::XMVectorScale(this->vector, scalar);
        return *this;
    }
    // 除算演算子のオーバーロード
    MYVECTOR3 const operator/(MYVECTOR3 other) const
    {
        assert(other.LengthSq());
        return MYVECTOR3(DirectX::XMVectorDivide(this->vector, other.GetVector()));
    }
    MYVECTOR3 const operator/(float f) const
    {
        assert(f != 0.0f);
        return MYVECTOR3(DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(f, f, f, 0.0f)));
    }
    MYVECTOR3 const operator/=(MYVECTOR3 other)
    {
        assert(other.LengthSq());
        this->vector = DirectX::XMVectorDivide(this->vector, other.GetVector());
        return *this;
    }
    MYVECTOR3 const operator/=(float f)
    {
        assert(f != 0.0f);
        this->vector = DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(f, f, f, 0.0f));
        return *this;
    }

#pragma region VectorXZ    
    MYVECTOR3 GetMyVectorXZ()
    {
        return MYVECTOR3(GetX(), 0.0f, GetZ());
    }
    MYVECTOR2 GetMyVector2XZ()
    {
        return MYVECTOR2(GetX(), GetZ());
    }
    DirectX::XMVECTOR GetVectorXZ()
    {
        return GetMyVectorXZ().GetVector();
    }
    void GetFlaot3XZ(DirectX::XMFLOAT3 f3)
    {
        return GetMyVectorXZ().GetFlaot3(f3);
    }
    float LengthXZ()
    {
        return GetMyVectorXZ().Length();
    }    
    float LengthXZSq()
    {
        return GetMyVectorXZ().LengthSq();
    }
#pragma endregion VectorXZ
private:
    alignas(16) DirectX::XMVECTOR vector;  // 内部でXMVECTORを保持
};