#pragma once
#include <DirectXMath.h>
#include "MYVECTOR3.h"

class MYMATRIX
{
public:
    MYMATRIX() : matrix(DirectX::XMMatrixIdentity()) {}
    MYMATRIX(DirectX::XMFLOAT4X4 f4x4)
    {
        this->matrix = DirectX::XMLoadFloat4x4(&f4x4);
    }
    MYMATRIX(DirectX::XMMATRIX vec) : matrix(vec) {}
    MYMATRIX(const MYMATRIX& mMatrix) :matrix(mMatrix.matrix) {}; // コピーコンストラクタ

    DirectX::XMMATRIX GetMatrix() { return this->matrix; }
    DirectX::XMFLOAT4X4 GetFlaot4x4() 
    { 
        DirectX::XMFLOAT4X4 f4x4;
        DirectX::XMStoreFloat4x4(&f4x4, this->matrix);
        return f4x4;
    }
    MYVECTOR3 GetRight()
    {
        DirectX::XMFLOAT4X4 f4x4 = GetFlaot4x4();
        MYVECTOR3 right = MYVECTOR3(f4x4._11, f4x4._12, f4x4._13);
        return right;
    }
    MYVECTOR3 GetUp()
    {
        DirectX::XMFLOAT4X4 f4x4 = GetFlaot4x4();
        MYVECTOR3 up = MYVECTOR3(f4x4._21, f4x4._22, f4x4._23);
        return up;
    }
    MYVECTOR3 GetForward()
    {
        DirectX::XMFLOAT4X4 f4x4 = GetFlaot4x4();
        MYVECTOR3 forwrd = MYVECTOR3(f4x4._31, f4x4._32, f4x4._33);
        return forwrd;
    }

    //// 加算演算子のオーバーロード
    //MYMATRIX const operator+(MYMATRIX other)
    //{
    //    return MYMATRIX(DirectX::XMVectorAdd(matrix, other.GetVector()));
    //}
    //// 減算演算子のオーバーロード
    //MYMATRIX const operator-(MYMATRIX other)
    //{
    //    return MYMATRIX(DirectX::XMVectorSubtract(matrix, other.GetVector()));
    //}
    //// 乗算演算子のオーバーロード
    //MYMATRIX const operator*(MYMATRIX other)
    //{
    //    return MYMATRIX(DirectX::XMVectorMultiply(matrix, other.GetVector()));
    //}
    //// スカラー倍
    //MYMATRIX const operator*(float scalar)
    //{
    //    return MYMATRIX(DirectX::XMVectorScale(matrix, scalar));
    //}
    //// 除算演算子のオーバーロード
    //MYMATRIX const operator/(MYMATRIX other)
    //{
    //    assert(other.LengthSp());
    //    return MYMATRIX(DirectX::XMVectorDivide(matrix, other.GetVector()));
    //}
private:
    alignas(16) DirectX::XMMATRIX matrix;
};
