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
    MYVECTOR3(const MYVECTOR3& my_vector) :vector(my_vector.vector) {}; // コピーコンストラクタ

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

    // 長さを計算
    float Length() const 
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3Length(this->vector));
    }
    float LengthSp() const
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(this->vector));
    }
    // 正規化
    MYVECTOR3 Normalize() const
    {
        return MYVECTOR3(DirectX::XMVector3Normalize(this->vector));
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
        // TODO エラー発生するように変更
        if (mVec.LengthSp() == 0.0f) return MYVECTOR3();

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
    MYVECTOR3 Dot(MYVECTOR3 mVec) const
    {
        return MYVECTOR3(DirectX::XMVector3Dot(this->vector, mVec.GetVector()));
    }
    // 外積計算
    MYVECTOR3 Cross(MYVECTOR3 mVec) const
    {
        return MYVECTOR3(DirectX::XMVector3Cross(this->vector, mVec.GetVector()));
    }
    // 線形補間
    MYVECTOR3 Lerp(MYVECTOR3 mVec,float t) const
    {
        return MYVECTOR3(DirectX::XMVectorLerp(this->vector, mVec.GetVector(), t));
    }

    // 加算演算子のオーバーロード
    MYVECTOR3 const operator+(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorAdd(this->vector, other.GetVector()));
    }
    // 減算演算子のオーバーロード
    MYVECTOR3 const operator-(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorSubtract(this->vector, other.GetVector()));
    }
    // 乗算演算子のオーバーロード
    MYVECTOR3 const operator*(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }
    // スカラー倍
    MYVECTOR3 const operator*(float scalar)
    {
        return MYVECTOR3(DirectX::XMVectorScale(this->vector, scalar));
    }
    // 除算演算子のオーバーロード
    MYVECTOR3 const operator/(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorDivide(this->vector, other.GetVector()));
    }

#pragma region VectorXZ
    // TODO 作成する
#pragma endregion VectorXZ
private:
    alignas(16) DirectX::XMVECTOR vector;  // 内部でXMVECTORを保持
};

class T
{
    T()
    {
        MYVECTOR3 c = vector;
    }

     MYVECTOR3 vector;
};

// XZ2軸作成