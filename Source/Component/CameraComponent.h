#pragma once
#include <memory>
#include "System/MyMath/MYVECTOR3.h"
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
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOWEST; }

	// �J�����R���g���[���[�̐ݒ�
	void SetCameraController(std::unique_ptr<CameraControllerBase> camera_controller);

	/**
	 * \fn SetMainCamera
	 * \brief ���C���J�����ɐݒ肷��
	 * 
	 */
	void SetMainCamera();

	// �w�����������
	void SetLookAt(MYVECTOR3 Eye, MYVECTOR3 Focus, MYVECTOR3 Up);
	// �p�[�X�y�N�e�B�u�ݒ�
	void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);
	// �J�����̋����ݒ�
	void SetRange(float range) { this->range = range; }
	// Y����]�x�ݒ�
	void SetRotateY(float rotateY) { this->rotateY = rotateY; }
	// X����]�x�ݒ�
	void SetRotateX(float rotateX) { this->rotateX = rotateX; }
	// �v���W�F�N�V�����s��ݒ�
	void SetProjectionTransform(DirectX::XMFLOAT4X4 projection_transform) { this->projection_transform = projection_transform; }
	// ���C���J�����t���O�̐ݒ�
	void SetIsMainCamera(bool is_main_camera) { this->is_main_camera = is_main_camera; }

	// �J�����̋����擾
	float GetRange() { return this->range; }
	// Y����]�x�擾
	float GetRotateY() { return this->rotateY; }
	// X����]�x�擾
	float GetRotateX() { return this->rotateX; }
	// �r���[�s��擾
	DirectX::XMFLOAT4X4 GetViewTransform() const { return this->view_transform; }
	// �v���W�F�N�V�����s��擾
	DirectX::XMFLOAT4X4 GetProjectionTransform() const { return this->projection_transform; }
	// ���_�擾
	DirectX::XMFLOAT3 GetEye() const { return this->eye; }
	// �����X�擾
	DirectX::XMFLOAT3 GetFocus() const { return this->focus; }
	// ������擾
	DirectX::XMFLOAT3 GetUp() const { return this->up; }
	// �O�����擾
	DirectX::XMFLOAT3 GetForward() const { return this->forward; }
	// �E�����擾
	DirectX::XMFLOAT3 GetRight() const { return this->right; }
	// ���C���J�����ł��邩
	bool GetIsMainCamera() { return this->is_main_camera; }

private:
	DirectX::XMFLOAT3		eye = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	DirectX::XMFLOAT3		focus = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	DirectX::XMFLOAT3		right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3		up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMFLOAT3		forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

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

