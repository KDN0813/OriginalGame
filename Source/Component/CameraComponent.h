#pragma once
#include <memory>
#include "System/MyMath/MYVECTOR3.h"
#include "Component.h"

class CameraManager;
class Transform3DComponent;

class CameraComponent : public Component
{
public:
	struct CameraParam
	{
		DirectX::XMFLOAT3		eye = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		DirectX::XMFLOAT3		focus = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3		right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3		up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		DirectX::XMFLOAT3		forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		float					fovY = DirectX::XMConvertToRadians(45);
		float					aspect = 16.0f / 9.0f;
		float					nearZ = 0.1f;
		float					farZ = 1000.0f;
		float					range = 1.0f;
		float					rotateY = 0.0f;
		float					rotateX = 0.0f;
	};
public:
    CameraComponent(CameraParam camera_param,CameraManager* camera_manager);

	// �J�n�֐�
	void Start()override;
	// �C���֐�
	void End()override;
	// ���X�^�[�g����
	void ReStart();
    // �X�V�֐�
    void Update(float elapsed_time);
    // ���O�擾
    const char* GetName()const { return "CameraComponent"; };
	// �D��x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOWEST; }

	/**
	 * \fn SetMainCamera
	 * \brief ���C���J�����ɐݒ肷��
	 * 
	 */
	void SetMainCamera();

	// ���_�ݒ�
	void SetEye(DirectX::XMFLOAT3 eye) { this->change_value = true; this->camera_param.eye = eye; }
	// �����X�ݒ�
	void SetFocus(DirectX::XMFLOAT3 focus) { this->change_value = true; this->camera_param.focus = focus; }
	// �E�����ݒ�
	void  SetRight(DirectX::XMFLOAT3 right) { this->change_value = true; this->camera_param.right = right; }
	// ������ݒ�
	void SetUp(DirectX::XMFLOAT3 up) { this->change_value = true; this->camera_param.up = up; }
	// �O�����ݒ�
	void SetForward(DirectX::XMFLOAT3 forward) { this->change_value = true; this->camera_param.forward = forward; }
	// ��p�ݒ�
	void SetFovY(float fovY) { this->change_value = true; this->camera_param.fovY = fovY; }
	// �A�X�y�N�g��ݒ�
	void SetAspect(float aspect) { this->change_value = true; this->camera_param.aspect = aspect; }
	// �j�A�N���b�v�l�ݒ�
	void SetNearZ(float nearZ) { this->change_value = true; this->camera_param.nearZ = nearZ; }
	// �t�@�[�N���b�v�l�ݒ�
	void SetFarZ(float farZ) { this->change_value = true; this->camera_param.farZ = farZ; }
	// �J�����̋����ݒ�
	void SetRange(float range) { this->change_value = true; this->camera_param.range = range; }
	// Y����]�x�ݒ�
	void SetRotateY(float rotateY) { this->change_value = true; this->camera_param.rotateY = rotateY; }
	// X����]�x�ݒ�
	void SetRotateX(float rotateX) { this->change_value = true; this->camera_param.rotateX = rotateX; }
	// ���C���J�����t���O�̐ݒ�
	void SetIsMainCamera(bool is_main_camera) { this->is_main_camera = is_main_camera; }

	// �r���[�s��擾
	DirectX::XMFLOAT4X4 GetViewTransform() const { return this->view_transform; }
	// �v���W�F�N�V�����s��擾
	DirectX::XMFLOAT4X4 GetProjectionTransform() const { return this->projection_transform; }
	// ���_�擾
	DirectX::XMFLOAT3 GetEye() const { return this->camera_param.eye; }
	// �����X�擾
	DirectX::XMFLOAT3 GetFocus() const { return this->camera_param.focus; }
	// �E�����擾
	DirectX::XMFLOAT3 GetRight() const { return this->camera_param.right; }
	// ������擾
	DirectX::XMFLOAT3 GetUp() const { return this->camera_param.up; }
	// �O�����擾
	DirectX::XMFLOAT3 GetForward() const { return this->camera_param.forward; }
	// ��p�擾
	float GetFovY() { return this->camera_param.fovY; }
	// �A�X�y�N�g��擾
	float GetAspect() { return this->camera_param.aspect; }
	// �j�A�N���b�v�l�擾
	float GetNearZ() { return this->camera_param.nearZ; }
	// �t�@�[�N���b�v�l�擾
	float GetFarZ() { return this->camera_param.farZ; }
	// �J�����̋����擾
	float GetRange() { return this->camera_param.range; }
	// Y����]�x�擾
	float GetRotateY() { return this->camera_param.rotateY; }
	// X����]�x�擾
	float GetRotateX() { return this->camera_param.rotateX; }
	// ���C���J�����ł��邩
	bool GetIsMainCamera() { return this->is_main_camera; }
private:
	// �w�����������
	void SetLookAt(MYVECTOR3 Eye, MYVECTOR3 Focus, MYVECTOR3 Up);
	// �p�[�X�y�N�e�B�u�ݒ�
	void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);

private:
	CameraParam camera_param;
	CameraParam default_param;
	DirectX::XMFLOAT4X4	view_transform = {};
	DirectX::XMFLOAT4X4	projection_transform = {};

	bool change_value = true;
	bool is_main_camera = false;
	CameraManager*const	camera_manager;
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};

