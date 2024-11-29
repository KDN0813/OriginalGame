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
    MYMATRIX(const MYMATRIX& mMatrix) = default; // �R�s�[�R���X�g���N�^

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

    // �P�ʃx�N�g���ɐݒ�
    void SetIdentity() { this->matrix = DirectX::XMMatrixIdentity(); }
    // ���s�ړ��s��̐ݒ�
    void SetTranslationMatrix(float x, float y, float z)
    {
        this->matrix = DirectX::XMMatrixTranslation(x, y, z);
    }
    void SetTranslationMatrix(DirectX::XMFLOAT3 f3)
    {
        SetTranslationMatrix(f3.x, f3.y, f3.z);
    }
    // �X�P�[���s��̐ݒ�
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
    // ��]�s��(�I�C���[�e)�̐ݒ�
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
    // ��]�s��(�N�H�[�^�j�I��)�̐ݒ�
    void SetRotationQuaternion(DirectX::XMFLOAT4 f4)
    {
        this->matrix = DirectX::XMMatrixRotationQuaternion(MYVECTOR4(f4).GetVector());
    }
    // ���[�J�����W���v�Z
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

    // �t�s����擾
    MYMATRIX GetInverse(MYVECTOR3 mVec3) const
    {
        return MYMATRIX(DirectX::XMMatrixInverse(mVec3.GetVectorAddress(), this->matrix));
    }
    MYMATRIX GetInverse(DirectX::XMVECTOR* vec) const
    {
        return MYMATRIX(DirectX::XMMatrixInverse(vec, this->matrix));
    }
    // �t�s���ݒ�
    void SetInverse(MYVECTOR3 mVec3)
    {
        this->matrix = DirectX::XMMatrixInverse(mVec3.GetVectorAddress(), this->matrix);
    }
    // �������e�s���ݒ�
    void SetPerspectiveFovLH(float fovY, float aspect, float nearX, float farZ)
    {
        this->matrix = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearX, farZ);
    }
    // �J�����̃r���[�s���ݒ�
    void SetLookAtLH(MYVECTOR3 eye, MYVECTOR3 focus, MYVECTOR3 up)
    {
        this->matrix = DirectX::XMMatrixLookAtLH(eye.GetVector(), focus.GetVector(), up.GetVector());
    }
    // �s��ƃx�N�g���̏�Z
    MYVECTOR3 Vector3TransformCoord(MYVECTOR3 mVec3) const
    {
        return DirectX::XMVector3TransformCoord(mVec3.GetVector(), this->matrix);
    }
    MYVECTOR2 Vector2TransformCoord(MYVECTOR2 mVec2) const
    {
        return DirectX::XMVector2TransformCoord(mVec2.GetVector(), this->matrix);
    }
    // �s��ƃx�N�g���̏�Z(�s��̍��W�����������Ȃ�)
    MYVECTOR3 Vector3TransformNormal(MYVECTOR3 mVec3) const
    {
        return DirectX::XMVector3TransformNormal(mVec3.GetVector(), this->matrix);
    }
    // �s��ƃx�N�g���̏�Z(�s��̍��W�����������Ȃ�)
    MYVECTOR2 Vector2TransformNormal(MYVECTOR2 mVec2) const
    {
        return DirectX::XMVector2TransformNormal(mVec2.GetVector(), this->matrix);
    }

    // ��Z���Z�q�̃I�[�o�[���[�h
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
