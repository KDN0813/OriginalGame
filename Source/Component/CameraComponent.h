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

	// 開始関数
	void Start()override;
	// 修了関数
	void End()override;
    // 更新関数
    void Update(float elapsed_time);
    // 名前取得
    const char* GetName()const { return "CameraComponent"; };
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }

	// カメラコントローラーの設定
	void SetCameraController(std::unique_ptr<CameraControllerBase> camera_controller);

	/**
	 * \fn SetMainCamera
	 * \brief メインカメラに設定する
	 * 
	 */
	void SetMainCamera();

	// 指定方向を向く
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);
	// パースペクティブ設定
	void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);
	// カメラの距離設定
	void SetRange(float range) { this->range = range; }
	// Y軸回転度設定
	void SetRotateY(float rotateY) { this->rotateY = rotateY; }
	// X軸回転度設定
	void SetRotateX(float rotateX) { this->rotateX = rotateX; }
	// プロジェクション行列設定
	void SetProjectionTransform(DirectX::XMFLOAT4X4	projection_transform) { this->projection_transform = projection_transform; }
	// メインカメラフラグの設定
	void SetIsMainCamera(bool is_main_camera) { this->is_main_camera = is_main_camera; }

	// カメラの距離取得
	float GetRange() const { return this->range; }
	// Y軸回転度取得
	float GetRotateY() const { return this->rotateY; }
	// X軸回転度取得
	float GetRotateX() const { return this->rotateX; }
	// 画角取得
	float GetFovY() const { return this->fovY; }
	// アスペクト比取得
	float GetAspect() const { return this->aspect; }
	// ニアクリップ値取得
	float GetNearZ() const { return this->nearZ; }
	// ファークリップ値取得
	float GetFarZ() const { return this->farZ; }
	// ビュー行列取得
	const DirectX::XMFLOAT4X4& GetViewTransform() const { return this->view_transform; }
	// プロジェクション行列取得
	const DirectX::XMFLOAT4X4& GetProjectionTransform() const { return this->projection_transform; }
	// 視点取得
	const DirectX::XMFLOAT3& GetEye() const { return this->eye; }
	// 注視店取得
	const DirectX::XMFLOAT3& GetFocus() const { return this->focus; }
	// 上方向取得
	const DirectX::XMFLOAT3& GetUp() const { return this->up; }
	// 前方向取得
	const DirectX::XMFLOAT3& GetFront() const { return this->front; }
	// 右方向取得
	const DirectX::XMFLOAT3& GetRight() const { return this->right; }
	// メインカメラであるか
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

