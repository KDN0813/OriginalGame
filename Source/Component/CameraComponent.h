#pragma once
#include "Component.h"
#include <DirectXMath.h>

class CameraManager;
class Transform3DComponent;

class CameraComponent : public Component
{
public:
    CameraComponent(CameraManager* camera_manager);
    ~CameraComponent();

public:
    // �X�V�֐�
    void Update(float elapsed_time);
    // ���O�擾
    const char* GetName()const { return "CameraComponent"; };
    
	/**
	 * \fn SetMainCamera
	 * \brief ���C���J�����ɐݒ肷��
	 * 
	 */
	void SetMainCamera();

	// �w�����������
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);
	// �p�[�X�y�N�e�B�u�ݒ�
	void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);
	// ��p�擾
	float GetFovY() const { return fovY; }
	// �A�X�y�N�g��擾
	float GetAspect() const { return aspect; }
	// �j�A�N���b�v�l�擾
	float GetNearZ() const { return nearZ; }
	// �t�@�[�N���b�v�l�擾
	float GetFarZ() const { return farZ; }
	// �r���[�s��擾
	const DirectX::XMFLOAT4X4& GetViewTransform() const { return view_transform; }
	// �v���W�F�N�V�����s��擾
	const DirectX::XMFLOAT4X4& GetProjectionTransform() const { return projection_transform; }
	// ���_�擾
	const DirectX::XMFLOAT3& GetEye() const { return eye; }
	// �����X�擾
	const DirectX::XMFLOAT3& GetFocus() const { return focus; }
	// ������擾
	const DirectX::XMFLOAT3& GetUp() const { return up; }
	// �O�����擾
	const DirectX::XMFLOAT3& GetFront() const { return front; }
	// �E�����擾
	const DirectX::XMFLOAT3& GetRight() const { return right; }
private:
	float					fovY = DirectX::XMConvertToRadians(45);
	float					aspect = 16.0f / 9.0f;
	float					nearZ = 0.1f;
	float					farZ = 1000.0f;

	DirectX::XMFLOAT3		eye = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	DirectX::XMFLOAT3		focus = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3		up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMFLOAT3		front = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT3		right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

	DirectX::XMFLOAT4X4		view_transform = {};
	DirectX::XMFLOAT4X4		projection_transform = {};

	DirectX::XMFLOAT3		range = {};

	CameraManager*const	camera_manager;
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};
