#pragma once
#include <memory>
#include "System/MyMath/MYMATRIX.h"
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
	void SetLookAt(MYVECTOR3 eye, MYVECTOR3 focus, MYVECTOR3 up);
	// �p�[�X�y�N�e�B�u�ݒ�
	void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);
	// �J�����̋����ݒ�
	void SetRange(float range) { this->range = range; }
	// Y����]�x�ݒ�
	void SetRotateY(float rotateY) { this->rotateY = rotateY; }
	// X����]�x�ݒ�
	void SetRotateX(float rotateX) { this->rotateX = rotateX; }
	// �v���W�F�N�V�����s��ݒ�
	void SetProjectionTransform(MYMATRIX projection_transform) { this->projection_transform = projection_transform; }
	// ���C���J�����t���O�̐ݒ�
	void SetIsMainCamera(bool is_main_camera) { this->is_main_camera = is_main_camera; }

	// �J�����̋����擾
	float GetRange() { return this->range; }
	// Y����]�x�擾
	float GetRotateY() { return this->rotateY; }
	// X����]�x�擾
	float GetRotateX() { return this->rotateX; }
	// ��p�擾
	float GetFovY() { return this->fovY; }
	// �A�X�y�N�g��擾
	float GetAspect() { return this->aspect; }
	// �j�A�N���b�v�l�擾
	float GetNearZ() { return this->nearZ; }
	// �t�@�[�N���b�v�l�擾
	float GetFarZ() { return this->farZ; }
	// �r���[�s��擾
	MYMATRIX GetViewTransform() { return this->view_transform; }
	// �v���W�F�N�V�����s��擾
	MYMATRIX GetProjectionTransform() { return this->projection_transform; }
	// ���_�擾
	MYVECTOR3 GetEye() { return this->eye; }
	// �����X�擾
	MYVECTOR3 GetFocus() { return this->focus; }
	// ������擾
	MYVECTOR3 GetUp() { return this->world_transform.GetUp() ; }
	// �O�����擾
	MYVECTOR3 GetFront() { return this->world_transform.GetForward(); }
	// �E�����擾
	MYVECTOR3 GetRight() { return this->world_transform.GetRight(); }
	// �^�[�Q�b�g�擾
	MYVECTOR3 GetTarget() { return this->target; }
	// ���C���J�����ł��邩
	bool GetIsMainCamera() { return this->is_main_camera; }

private:
	float					fovY = DirectX::XMConvertToRadians(45);
	float					aspect = 16.0f / 9.0f;
	float					nearZ = 0.1f;
	float					farZ = 1000.0f;

	MYVECTOR3		eye = MYVECTOR3(0.0f, 0.0f, -1.0f);
	MYVECTOR3		focus = MYVECTOR3(0.0f, 0.0f, 0.0f);
	MYVECTOR3		target = MYVECTOR3(0.0f, 0.0f, 0.0f);

	MYMATRIX		world_transform = {};
	MYMATRIX		view_transform = {};
	MYMATRIX		projection_transform = {};

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

