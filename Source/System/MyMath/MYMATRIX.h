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
    MYVECTOR3 GetTranslation()
    {
        DirectX::XMFLOAT4X4 f4x4 = GetFlaot4x4();
        MYVECTOR3 translation = MYVECTOR3(f4x4._41, f4x4._42, f4x4._43);
        return translation;
    }
    void SetMatrix(DirectX::XMMATRIX matrix) { this->matrix = matrix; }
    void SetFlaot4x4(DirectX::XMFLOAT4X4 f4x4)
    {
        this->matrix = DirectX::XMLoadFloat4x4(&f4x4);
    }
    // 単位ベクトルに設定
    void SetIdentity() { this->matrix = DirectX::XMMatrixIdentity(); }

private:
    alignas(16) DirectX::XMMATRIX matrix;
};
