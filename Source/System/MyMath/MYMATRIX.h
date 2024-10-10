#pragma once
#include <DirectXMath.h>
#include "MYVECTOR3.h"
#include "MYVECTOR4.h"

class MYMATRIX
{
public:
    MYMATRIX() : matrix(DirectX::XMMatrixIdentity()) {}
    MYMATRIX(DirectX::XMFLOAT4X4 f4x4)
    {
        this->matrix = DirectX::XMLoadFloat4x4(&f4x4);
    }
    MYMATRIX(DirectX::XMMATRIX mMatrix) : matrix(mMatrix) {}
    MYMATRIX(const MYMATRIX& mMatrix) :matrix(mMatrix.matrix) {}; // コピーコンストラクタ

    DirectX::XMMATRIX GetMatrix() const { return this->matrix; }
    DirectX::XMFLOAT4X4 GetFlaot4x4() const 
    { 
        DirectX::XMFLOAT4X4 f4x4;
        DirectX::XMStoreFloat4x4(&f4x4, this->matrix);
        return f4x4;
    }    
    void SetMatrix(DirectX::XMMATRIX matrix) { this->matrix = matrix; }
    void SetMatrix(DirectX::XMFLOAT4X4 f4x4)
    {
        this->matrix = DirectX::XMLoadFloat4x4(&f4x4);
    }
    // 単位ベクトルに設定
    void SetIdentity() { this->matrix = DirectX::XMMatrixIdentity(); }
    // 平行移動行列の設定
    void SetTranslationMatrix(float x, float y, float z)
    {
        this->matrix = DirectX::XMMatrixTranslation(x, y, z);
    }
    void SetTranslationMatrix(DirectX::XMFLOAT3 f3)
    {
        SetTranslationMatrix(f3.x, f3.y, f3.z);
    }
    void SetTranslationMatrix(DirectX::XMVECTOR vec)
    {
        SetTranslationMatrix(MYVECTOR3(vec).GetFlaot3());
    }
    void SetTranslationMatrix(MYVECTOR3 mVec3)
    {
        SetTranslationMatrix(mVec3.GetFlaot3());
    }
    // スケール行列の設定
    void SetScalingMatrix(float x, float y, float z)
    {
        this->matrix = DirectX::XMMatrixScaling(x, y, z);
    }
    void SetScalingMatrix(DirectX::XMFLOAT3 f3)
    {
        SetScalingMatrix(f3.x, f3.y, f3.z);
    }
    void SetScalingMatrix(DirectX::XMVECTOR vec)
    {
        SetScalingMatrix(MYVECTOR3(vec).GetFlaot3());
    }
    void SetScalingMatrix(MYVECTOR3 mVec3)
    {
        SetScalingMatrix(mVec3.GetFlaot3());
    }
    void SetScalingMatrix(float f)
    {
        SetScalingMatrix(f, f, f);
    }
    // 回転行列(オイラー各)の設定
    void SetRotationRollPitchYaw(float roll, float pitch, float yaw)
    {
        this->matrix = DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
    }
    void SetRotationRollPitchYaw(DirectX::XMFLOAT3 f3)
    {
        SetRotationRollPitchYaw(f3.x, f3.y, f3.z);
    }
    void SetRotationRollPitchYaw(DirectX::XMVECTOR vec)
    {
        SetRotationRollPitchYaw(MYVECTOR3(vec).GetFlaot3());
    }
    void SetRotationRollPitchYaw(MYVECTOR3 mVec3)
    {
        SetRotationRollPitchYaw(mVec3.GetFlaot3());
    }
    // 回転行列(クォータニオン)の設定
    void SetRotationRollPitchYaw(DirectX::XMFLOAT4 f4)
    {
        this->matrix = DirectX::XMMatrixRotationQuaternion(MYVECTOR4(f4).GetVector());
    }
    void SetRotationQuaternion(DirectX::XMVECTOR vec)
    {
        this->matrix = DirectX::XMMatrixRotationQuaternion(vec);
    }
    void SetRotationQuaternion(MYVECTOR4 mVec4)
    {
        this->matrix = DirectX::XMMatrixRotationQuaternion(mVec4.GetVector());
    }

    // 逆行列を取得
    MYMATRIX GetInverse(MYVECTOR3 mVec3) const
    {
        return MYMATRIX(DirectX::XMMatrixInverse(mVec3.GetVectorAddress(), this->matrix));
    }
    MYMATRIX GetInverse(DirectX::XMVECTOR* vec) const
    {
        return MYMATRIX(DirectX::XMMatrixInverse(vec, this->matrix));
    }
    // ローカル行列の設定
    void SetLocalMatrix(MYVECTOR3 scale, MYVECTOR4 rotation, MYVECTOR3 translation)
    {
        MYMATRIX S, R, T;
        S.SetScalingMatrix(scale);
        R.SetRotationQuaternion(rotation);
        T.SetTranslationMatrix(translation);
        this->matrix = (S * R * T).GetMatrix();
    }
    // ローカル行列の設定
    void SetLocalMatrix(MYVECTOR3 scale, MYVECTOR3 rooll_pitch_yaw, MYVECTOR3 translation)
    {
        MYMATRIX S, R, T;
        S.SetScalingMatrix(scale);
        R.SetRotationRollPitchYaw(rooll_pitch_yaw);
        T.SetTranslationMatrix(translation);
        this->matrix = (S * R * T).GetMatrix();
    }
    // 逆行列を設定
    void SetInverse(MYVECTOR3 mVec3)
    {
        this->matrix = DirectX::XMMatrixInverse(mVec3.GetVectorAddress(), this->matrix);
    }
    // 透視投影行列を設定
    void SetPerspectiveFovLH(float fovY, float aspect, float nearX, float farZ)
    {
        this->matrix = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearX, farZ);
    }
    // カメラのビュー行列を設定
    void SetLookAtLH(MYVECTOR3 eye, MYVECTOR3 focus, MYVECTOR3 up)
    {
        this->matrix = DirectX::XMMatrixLookAtLH(eye.GetVector(), focus.GetVector(), up.GetVector());
    }
    void SetLookAtLH(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 focus, DirectX::XMFLOAT3 up)
    {
        this->matrix = DirectX::XMMatrixLookAtLH(MYVECTOR3(eye).GetVector(), MYVECTOR3(focus).GetVector(), MYVECTOR3(up).GetVector());
    }

    // 行列とベクトルの乗算
    MYVECTOR3 Vector3TransformCoord(MYVECTOR3 mVec3) const
    {
        return DirectX::XMVector3TransformCoord(mVec3.GetVector(), this->matrix);
    }
    // 行列とベクトルの乗算(行列の座標情報を加味しない)
    MYVECTOR3 Vector3TransformNormal(MYVECTOR3 mVec3) const
    {
        return DirectX::XMVector3TransformNormal(mVec3.GetVector(), this->matrix);
    }

    // 乗算演算子のオーバーロード
    MYMATRIX operator*(MYMATRIX other) const
    {
        return MYMATRIX(DirectX::XMMatrixMultiply(this->matrix, other.GetMatrix()));
    }
    MYMATRIX operator*=(MYMATRIX other)
    {
        this->matrix = DirectX::XMMatrixMultiply(this->matrix, other.GetMatrix());
        return *this;
    }
private:
    alignas(16) DirectX::XMMATRIX matrix;
};
