#pragma once
#include <DirectXMath.h>
#include "MYVECTOR3.h"
#include "MYVECTOR4.h"

class MYMATRIX
{
public:
    MYMATRIX() : matrix(DirectX::XMMatrixIdentity()) {}
    MYMATRIX(const DirectX::XMFLOAT4X4 f4x4)
    {
        this->matrix = DirectX::XMLoadFloat4x4(&f4x4);
    }
    MYMATRIX(const DirectX::XMMATRIX mMatrix) : matrix(mMatrix) {}
    MYMATRIX(const MYMATRIX& mMatrix) = default; // コピーコンストラクタ

    DirectX::XMMATRIX GetMatrix() const { return this->matrix; }
    void GetFlaot4x4(DirectX::XMFLOAT4X4& f4x4) const
    { 
        DirectX::XMStoreFloat4x4(&f4x4, this->matrix);
    }    
    MYVECTOR3 GetRight()
    {
        return MYVECTOR3(this->matrix.r[0]);
    }
    MYVECTOR3 GetUp()
    {
        return MYVECTOR3(this->matrix.r[1]);
    }
    MYVECTOR3 GetForward()
    {
        return MYVECTOR3(this->matrix.r[2]);
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
    // スケール行列の設定
    void SetScalingMatrix(float x, float y, float z)
    {
        this->matrix = DirectX::XMMatrixScaling(x, y, z);
    }
    void SetScalingMatrix(DirectX::XMFLOAT3 f3)
    {
        SetScalingMatrix(f3.x, f3.y, f3.z);
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
    void RotationZ(float angle)
    {
        this->matrix = DirectX::XMMatrixRotationZ(angle);
    }
    // 回転行列(クォータニオン)の設定
    void SetRotationQuaternion(DirectX::XMFLOAT4 f4)
    {
        this->matrix = DirectX::XMMatrixRotationQuaternion(MYVECTOR4(f4).GetVector());
    }
    // ローカル座標を計算
    void SetLocalMatrix(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 rotation, DirectX::XMFLOAT3 translation)
    {
        MYMATRIX S, R, T, W;
        S.SetScalingMatrix(scale);
        R.SetRotationQuaternion(rotation);
        T.SetTranslationMatrix(translation);
        this->matrix = (S * R * T).GetMatrix();
    }
    void SetLocalMatrix(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rooll_pitch_yaw, DirectX::XMFLOAT3 translation)
    {
        MYMATRIX S, R, T, W;
        S.SetScalingMatrix(scale);
        R.SetRotationRollPitchYaw(rooll_pitch_yaw);
        T.SetTranslationMatrix(translation);
        this->matrix = (S * R * T).GetMatrix();
    }
    void SetLocalMatrix(DirectX::XMFLOAT2 scale, float angle, DirectX::XMFLOAT2 translation)
    {
        MYMATRIX S, R, T, W;
        S.SetScalingMatrix({ scale.x,scale.y,1.0f });
        R.RotationZ(angle);
        T.SetTranslationMatrix({ translation.x,translation.y ,0.0f });
        this->matrix = (S * R * T).GetMatrix();
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
    // 行列とベクトルの乗算
    MYVECTOR3 Vector3TransformCoord(MYVECTOR3 mVec3) const
    {
        return DirectX::XMVector3TransformCoord(mVec3.GetVector(), this->matrix);
    }
    MYVECTOR2 Vector2TransformCoord(MYVECTOR2 mVec2) const
    {
        return DirectX::XMVector2TransformCoord(mVec2.GetVector(), this->matrix);
    }
    // 行列とベクトルの乗算(行列の座標情報を加味しない)
    MYVECTOR3 Vector3TransformNormal(MYVECTOR3 mVec3) const
    {
        return DirectX::XMVector3TransformNormal(mVec3.GetVector(), this->matrix);
    }
    // 行列とベクトルの乗算(行列の座標情報を加味しない)
    MYVECTOR2 Vector2TransformNormal(MYVECTOR2 mVec2) const
    {
        return DirectX::XMVector2TransformNormal(mVec2.GetVector(), this->matrix);
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
