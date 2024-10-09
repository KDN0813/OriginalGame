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
    MYMATRIX(DirectX::XMMATRIX mMatrix) : matrix(mMatrix) {}
    MYMATRIX(const MYMATRIX& mMatrix) :matrix(mMatrix.matrix) {}; // �R�s�[�R���X�g���N�^

    DirectX::XMMATRIX GetMatrix() { return this->matrix; }
    DirectX::XMFLOAT4X4 GetFlaot4x4() 
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
    void SetTranslationMatrix(DirectX::XMVECTOR vec)
    {
        SetTranslationMatrix(MYVECTOR3(vec).GetFlaot3());
    }
    void SetTranslationMatrix(MYVECTOR3 mVec3)
    {
        SetTranslationMatrix(mVec3.GetFlaot3());
    }
    // ��]�s��̐ݒ�
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
    // ��]�s��(�I�C���[�e)�̐ݒ�
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

    // �t�s����擾
    MYMATRIX GetInverse(MYVECTOR3 mVec3)
    {
        return MYMATRIX(DirectX::XMMatrixInverse(mVec3.GetVectorAddress(), this->matrix));
    }
    // World�s��̐ݒ�
    void SetWorldMatrix(MYMATRIX translation, MYMATRIX rotation, MYMATRIX scale)
    {
        this->matrix = MYMATRIX(scale * rotation * translation).GetMatrix();
    }
    void SetWorldMatrix(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 scale)
    {
        MYMATRIX T, R, S;
        T.SetTranslationMatrix(position);
        R.SetRotationRollPitchYaw(angle);
        S.SetScalingMatrix(scale);
        SetWorldMatrix(T, R, S);
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
    void SetLookAtLH(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 focus, DirectX::XMFLOAT3 up)
    {
        this->matrix = DirectX::XMMatrixLookAtLH(MYVECTOR3(eye).GetVector(), MYVECTOR3(focus).GetVector(), MYVECTOR3(up).GetVector());
    }

    MYVECTOR3 GetRight()
    {
        DirectX::XMFLOAT4X4 f4x4 = GetFlaot4x4();
        return MYVECTOR3(f4x4._11, f4x4._12, f4x4._13);
    }
    MYVECTOR3 GetUp()
    {
        DirectX::XMFLOAT4X4 f4x4 = GetFlaot4x4();
        return MYVECTOR3(f4x4._21, f4x4._22, f4x4._23);
    }
    MYVECTOR3 GetForward()
    {
        DirectX::XMFLOAT4X4 f4x4 = GetFlaot4x4();
        return MYVECTOR3(f4x4._31, f4x4._32, f4x4._33);
    }
    //MYVECTOR3 GetTranslation()
    //{
    //    DirectX::XMFLOAT4X4 f4x4 = GetFlaot4x4();
    //    MYVECTOR3 translation = MYVECTOR3(f4x4._41, f4x4._42, f4x4._43);
    //    return translation;
    //}

    // ��Z���Z�q�̃I�[�o�[���[�h
    MYMATRIX operator*(MYMATRIX other)
    {
        return MYMATRIX(DirectX::XMMatrixMultiply(this->matrix, other.GetMatrix()));
    }

private:
    alignas(16) DirectX::XMMATRIX matrix;
};
