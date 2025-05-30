#pragma once
#include <DirectXMath.h>

class MYVECTOR4
{
public:
    MYVECTOR4() : vector(DirectX::XMVectorZero()) {}
    MYVECTOR4(float x, float y, float z, float w)
    {
        this->vector = DirectX::XMVectorSet(x, y, z, w);
    }
    MYVECTOR4(DirectX::XMFLOAT4 f4)
    {
        this->vector = DirectX::XMVectorSet(f4.x, f4.y, f4.z, f4.w);
    }
    MYVECTOR4(DirectX::XMVECTOR vec) : vector(vec) {}
    MYVECTOR4(const MYVECTOR4& mVec) :vector(mVec.vector) {}; // コピーコンストラクタ

    DirectX::XMVECTOR GetVector() const
    {
        return this->vector;
    }
    DirectX::XMVECTOR* GetVectorAddress()
    {
        return &this->vector;
    }
    void GetFlaot4(DirectX::XMFLOAT4& f4)  const
    {
        DirectX::XMStoreFloat4(&f4, this->vector);
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
    void AddW(float w)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, w));
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
    void SubtractW(float w)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, w));
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
    void MultiplyW(float w)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, w));
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
    void DivideW(float w)
    {
        this->vector = DirectX::XMVectorDivide(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, w));
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
        return DirectX::XMVectorGetX(DirectX::XMVector4Length(this->vector));
    }
    float LengthSp() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(this->vector));
    }
    // 正規化
    MYVECTOR4 Normalize() const
    {
        return MYVECTOR4(DirectX::XMVector4Normalize(this->vector));
    }
    void NormalizeSelf()
    {
        this->vector = DirectX::XMVector4Normalize(this->vector);
    }
    // 各値の絶対値を計算
    MYVECTOR4 Abs() const
    {
        return MYVECTOR4(DirectX::XMVectorAbs(this->vector));
    }
    // 数値を切り上げて整数にする
    MYVECTOR4 Ceiling() const
    {
        return MYVECTOR4(DirectX::XMVectorCeiling(this->vector));
    }
    // 数値を切り捨てて整数にする
    MYVECTOR4 Floor() const
    {
        return MYVECTOR4(DirectX::XMVectorFloor(this->vector));
    }
    // 符号を反転させる
    MYVECTOR4 Negate() const
    {
        return MYVECTOR4(DirectX::XMVectorNegate(this->vector));
    }
    // 最も近い整数に丸める
    MYVECTOR4 Round() const
    {
        return MYVECTOR4(DirectX::XMVectorRound(this->vector));
    }
    // 0.0f 〜 1.0fに納める
    MYVECTOR4 Saturate() const
    {
        return MYVECTOR4(DirectX::XMVectorSaturate(this->vector));
    }
    // 平方根を計算
    MYVECTOR4 Sqrt() const
    {
        return MYVECTOR4(DirectX::XMVectorSqrt(this->vector));
    }
    float Sum() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVectorSum(DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0))));
    }

    // クランプ
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
    // 剰余演算
    MYVECTOR4 Mod(MYVECTOR4 mVec) const
    {
        assert(mVec.LengthSp());
        return DirectX::XMVectorMod(this->vector, mVec.GetVector());
    }
    // 累乗
    MYVECTOR4 Pow(MYVECTOR4 mVec) const
    {
        return MYVECTOR4(DirectX::XMVectorPow(this->vector, mVec.GetVector()));
    }
    // 累乗
    MYVECTOR4 Pow(float f) const
    {
        return MYVECTOR4(DirectX::XMVectorPow(this->vector, DirectX::XMVectorSet(f, f, f, 0.0f)));
    }
    // 内積計算
    float Dot(MYVECTOR4 mVec) const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector4Dot(this->vector, mVec.GetVector()));
    }
    // 線形補間
    MYVECTOR4 Lerp(MYVECTOR4 mVec, float t) const
    {
        return MYVECTOR4(DirectX::XMVectorLerp(this->vector, mVec.GetVector(), t));
    }
    // 球面線形補間
    MYVECTOR4 SLerp(MYVECTOR4 mVec, float t) const
    {
        return MYVECTOR4(DirectX::XMQuaternionSlerp(this->vector, mVec.GetVector(), t));
    }

    // 加算演算子のオーバーロード
    MYVECTOR4 const operator+(MYVECTOR4 other) const
    {
        return MYVECTOR4(DirectX::XMVectorAdd(this->vector, other.GetVector()));
    }
    MYVECTOR4 const operator+=(MYVECTOR4 other)
    {
        this->vector = DirectX::XMVectorAdd(this->vector, other.GetVector());
        return *this;
    }
    // 減算演算子のオーバーロード
    MYVECTOR4 const operator-(MYVECTOR4 other) const
    {
        return MYVECTOR4(DirectX::XMVectorSubtract(this->vector, other.GetVector()));
    }
    MYVECTOR4 const operator-=(MYVECTOR4 other)
    {
        this->vector = DirectX::XMVectorSubtract(this->vector, other.GetVector());
        return *this;
    }
    // 乗算演算子のオーバーロード
    MYVECTOR4 operator*(MYVECTOR4 other) const
    {
        return MYVECTOR4(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }
    MYVECTOR4 operator*=(MYVECTOR4 other)
    {
        this->vector = DirectX::XMVectorMultiply(this->vector, other.GetVector());
        return *this;
    }
    // スカラー倍
    MYVECTOR4 const operator*(float scalar) const
    {
        return MYVECTOR4(DirectX::XMVectorScale(this->vector, scalar));
    }
    MYVECTOR4 const operator*=(float scalar)
    {
        this->vector = DirectX::XMVectorScale(this->vector, scalar);
        return *this;
    }
    // 除算演算子のオーバーロード
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

private:
    alignas(16) DirectX::XMVECTOR vector;  // 内部でXMVECTORを保持
};