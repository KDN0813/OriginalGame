#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Component.h"
#include "Camera/CameraController.h"

class CameraManager;
class Transform3DComponent;

class CameraComponent : public Component
{
public:
    CameraComponent(CameraManager* camera_manager);

	// �J�n�֐�
	void Start()override;
	// �C���֐�
	void End()override;
    // �X�V�֐�
    void Update(float elapsed_time);
    // ���O�擾
    const char* GetName()const { return "CameraComponent"; };
	// �D��x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }

	// �J�����R���g���[���[�̐ݒ�
	void SetCameraController(std::unique_ptr<CameraControllerBase> camera_controller);

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
	// �J�����̋����ݒ�
	void SetRange(float range) { this->range = range; }
	// Y����]�x�ݒ�
	void SetRotateY(float rotateY) { this->rotateY = rotateY; }
	// X����]�x�ݒ�
	void SetRotateX(float rotateX) { this->rotateX = rotateX; }
	// �v���W�F�N�V�����s��ݒ�
	void SetProjectionTransform(DirectX::XMFLOAT4X4	projection_transform) { this->projection_transform = projection_transform; }
	// ���C���J�����t���O�̐ݒ�
	void SetIsMainCamera(bool is_main_camera) { this->is_main_camera = is_main_camera; }

	// �J�����̋����擾
	float GetRange() const { return this->range; }
	// Y����]�x�擾
	float GetRotateY() const { return this->rotateY; }
	// X����]�x�擾
	float GetRotateX() const { return this->rotateX; }
	// ��p�擾
	float GetFovY() const { return this->fovY; }
	// �A�X�y�N�g��擾
	float GetAspect() const { return this->aspect; }
	// �j�A�N���b�v�l�擾
	float GetNearZ() const { return this->nearZ; }
	// �t�@�[�N���b�v�l�擾
	float GetFarZ() const { return this->farZ; }
	// �r���[�s��擾
	const DirectX::XMFLOAT4X4& GetViewTransform() const { return this->view_transform; }
	// �v���W�F�N�V�����s��擾
	const DirectX::XMFLOAT4X4& GetProjectionTransform() const { return this->projection_transform; }
	// ���_�擾
	const DirectX::XMFLOAT3& GetEye() const { return this->eye; }
	// �����X�擾
	const DirectX::XMFLOAT3& GetFocus() const { return this->focus; }
	// ������擾
	const DirectX::XMFLOAT3& GetUp() const { return this->up; }
	// �O�����擾
	const DirectX::XMFLOAT3& GetFront() const { return this->front; }
	// �E�����擾
	const DirectX::XMFLOAT3& GetRight() const { return this->right; }
	// ���C���J�����ł��邩
	const bool& GetIsMainCamera() { return this->is_main_camera; }

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

	float range = 1.0f;
	float rotateY = 0.0f;
	float rotateX = 0.0f;

	bool is_main_camera = false;
	CameraManager*const	camera_manager;
	std::unique_ptr<CameraControllerBase> camera_controller;
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};

