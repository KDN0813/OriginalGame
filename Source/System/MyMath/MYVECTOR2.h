#pragma once
#include <DirectXMath.h>

class MYVECTOR2
{
public:
    MYVECTOR2() : vector(DirectX::XMVectorZero()) {}
    MYVECTOR2(float x, float y = 0.0f)
    {
        this->vector = DirectX::XMVectorSet(x, y, 0.0f, 0.0f);
    }
    MYVECTOR2(DirectX::XMFLOAT2 f2)
    {
        this->vector = DirectX::XMVectorSet(f2.x, f2.y, 0.0f, 0.0f);
    }
    MYVECTOR2(DirectX::XMVECTOR vec) : vector(vec) {}
    MYVECTOR2(const MYVECTOR2& mVec) :vector(mVec.vector) {}; // コピーコンストラクタ

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

    void ZeroClear()
    {
        vector = DirectX::XMVectorZero();
    }
    void ClearToValu(float valu)
    {
        vector = DirectX::XMVectorSet(valu, valu, 0.0f, 0.0f);
    }

    // 長さを計算
    float Length() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector2Length(this->vector));
    }
    float LengthSp() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(this->vector));
    }
    // 正規化
    MYVECTOR2 Normalize() const
    {
        return MYVECTOR2(DirectX::XMVector2Normalize(this->vector));
    }
    // 各値の絶対値を計算
    MYVECTOR2 Abs() const
    {
        return MYVECTOR2(DirectX::XMVectorAbs(this->vector));
    }
    // 数値を切り上げて整数にする
    MYVECTOR2 Ceiling() const
    {
        return MYVECTOR2(DirectX::XMVectorCeiling(this->vector));
    }
    // 数値を切り捨てて整数にする
    MYVECTOR2 Floor() const
    {
        return MYVECTOR2(DirectX::XMVectorFloor(this->vector));
    }
    // 符号を反転させる
    MYVECTOR2 Negate() const
    {
        return MYVECTOR2(DirectX::XMVectorNegate(this->vector));
    }
    // 最も近い整数に丸める
    MYVECTOR2 Round() const
    {
        return MYVECTOR2(DirectX::XMVectorRound(this->vector));
    }
    // 0.0f ～ 1.0fに納める
    MYVECTOR2 Saturate() const
    {
        return MYVECTOR2(DirectX::XMVectorSaturate(this->vector));
    }
    // 平方根を計算
    MYVECTOR2 Sqrt() const
    {
        return MYVECTOR2(DirectX::XMVectorSqrt(this->vector));
    }
    float Sum() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVectorSum(DirectX::XMVectorMultiply(this->vector, DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0))));
    }

    // クランプ
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
    // 剰余演算
    MYVECTOR2 Mod(MYVECTOR2 mVec) const
    {
        assert(mVec.LengthSp());
        return DirectX::XMVectorMod(this->vector, mVec.GetVector());
    }
    // 累乗
    MYVECTOR2 Pow(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(DirectX::XMVectorPow(this->vector, mVec.GetVector()));
    }
    // 累乗
    MYVECTOR2 Pow(float f) const
    {
        return MYVECTOR2(DirectX::XMVectorPow(this->vector, DirectX::XMVectorSet(f, f, 0.0f, 0.0f)));
    }
    // 内積計算
    MYVECTOR2 Dot(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(DirectX::XMVector2Dot(this->vector, mVec.GetVector()));
    }
    // 外積計算
    MYVECTOR2 Cross(MYVECTOR2 mVec) const
    {
        return MYVECTOR2(DirectX::XMVector2Cross(this->vector, mVec.GetVector()));
    }
    // 線形補間
    MYVECTOR2 Lerp(MYVECTOR2 mVec, float t) const
    {
        return MYVECTOR2(DirectX::XMVectorLerp(this->vector, mVec.GetVector(), t));
    }

    // 加算演算子のオーバーロード
    MYVECTOR2 const operator+(MYVECTOR2 other)
    {
        return MYVECTOR2(DirectX::XMVectorAdd(this->vector, other.GetVector()));
    }
    // 減算演算子のオーバーロード
    MYVECTOR2 const operator-(MYVECTOR2 other)
    {
        return MYVECTOR2(DirectX::XMVectorSubtract(this->vector, other.GetVector()));
    }
    // 乗算演算子のオーバーロード
    MYVECTOR2 operator*(MYVECTOR2 other)
    {
        return MYVECTOR2(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }
    // スカラー倍
    MYVECTOR2 const operator*(float scalar)
    {
        return MYVECTOR2(DirectX::XMVectorScale(this->vector, scalar));
    }
    // 除算演算子のオーバーロード
    MYVECTOR2 const operator/(MYVECTOR2 other)
    {
        assert(other.LengthSp());
        return MYVECTOR2(DirectX::XMVectorDivide(this->vector, other.GetVector()));
    }

private:
    alignas(16) DirectX::XMVECTOR vector;  // 内部でXMVECTORを保持
};

int A()
{
    MYVECTOR2 a{};
    MYVECTOR2 b{};
    MYVECTOR2 c{};
    MYVECTOR2 c = a;
    c= a + b + c;
}
