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

    // 長さを計算
    float Length() const 
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3Length(this->vector));
    }
    // 正規化
    MYVECTOR3 Normalize() const
    {
        return MYVECTOR3(DirectX::XMVector3Normalize(this->vector));
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
    // 乗算
    MYVECTOR3 const operator/(MYVECTOR3 other)
    {
        return MYVECTOR3(DirectX::XMVectorMultiply(this->vector, other.GetVector()));
    }

    // スカラー倍
    MYVECTOR3 const operator*(float scalar)
    {
        return MYVECTOR3(DirectX::XMVectorScale(this->vector, scalar));
    }

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
