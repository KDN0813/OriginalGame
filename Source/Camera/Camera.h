#pragma once

#include <DirectXMath.h>

// TODO (06/25)���f���`�悷�邽�߂̊ȈՃo�[�W����
// ��ɕ����̃J�������g���������悤��������
class Camera 
{
private:
    Camera();
    ~Camera(){}

public:
    static Camera& Intance()
    {
        static Camera camera;
        return camera;
    }

    /**
     * �w�����������
     * 
     * \param eye �J�����̎��_
     * \param focus �J�����̒����_
     * \param up �J�����̏����
     */
    void SetLookAt(
        const DirectX::XMFLOAT3& eye
        , const DirectX::XMFLOAT3& focus
        , const DirectX::XMFLOAT3& up
    );

    /**
     * �v���W�F�N�V�����s����쐬
     * 
     * \param fovY �J�����̎���p
     * \param aspect �A�X�y�N�g��(��ʂ̏c����)
     * \param nearX �N���b�v����(��)
     * \param farZ �N���b�v����(��)
     */
    void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);
    
    // �e��擾�E�ݒ�֐�
    const DirectX::XMFLOAT4X4& GetView()const { return view; }
    const DirectX::XMFLOAT4X4& getProjection()const { return projection; }
    const DirectX::XMFLOAT3& GetEye()const { return eye; }
    const DirectX::XMFLOAT3& GetFocus()const { return focus; }
    const DirectX::XMFLOAT3& GetUp()const { return up; }
    const DirectX::XMFLOAT3& GetFront()const { return front; }
    const DirectX::XMFLOAT3& GetRIght()const { return right; }
private:
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
    DirectX::XMFLOAT3 eye;
    DirectX::XMFLOAT3 focus;
    DirectX::XMFLOAT3 up;
    DirectX::XMFLOAT3 front;
    DirectX::XMFLOAT3 right;
};
