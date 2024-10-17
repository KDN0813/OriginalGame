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

	// 開始関数
	void Start()override;
	// 修了関数
	void End()override;
    // 更新関数
    void Update(float elapsed_time);
    // 名前取得
    const char* GetName()const { return "CameraComponent"; };
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOWEST; }

	// カメラコントローラーの設定
	void SetCameraController(std::unique_ptr<CameraControllerBase> camera_controller);

	/**
	 * \fn SetMainCamera
	 * \brief メインカメラに設定する
	 * 
	 */
	void SetMainCamera();

	// 指定方向を向く
	void SetLookAt(MYVECTOR3 Eye, MYVECTOR3 Focus, MYVECTOR3 Up);
	// パースペクティブ設定
	void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);
	// カメラの距離設定
	void SetRange(float range) { this->range = range; }
	// Y軸回転度設定
	void SetRotateY(float rotateY) { this->rotateY = rotateY; }
	// X軸回転度設定
	void SetRotateX(float rotateX) { this->rotateX = rotateX; }
	// プロジェクション行列設定
	void SetProjectionTransform(DirectX::XMFLOAT4X4 projection_transform) { this->projection_transform = projection_transform; }
	// メインカメラフラグの設定
	void SetIsMainCamera(bool is_main_camera) { this->is_main_camera = is_main_camera; }

	// カメラの距離取得
	float GetRange() { return this->range; }
	// Y軸回転度取得
	float GetRotateY() { return this->rotateY; }
	// X軸回転度取得
	float GetRotateX() { return this->rotateX; }
	// ビュー行列取得
	DirectX::XMFLOAT4X4 GetViewTransform() const { return this->view_transform; }
	// プロジェクション行列取得
	DirectX::XMFLOAT4X4 GetProjectionTransform() const { return this->projection_transform; }
	// 視点取得
	DirectX::XMFLOAT3 GetEye() const { return this->eye; }
	// 注視店取得
	DirectX::XMFLOAT3 GetFocus() const { return this->focus; }
	// 上方向取得
	DirectX::XMFLOAT3 GetUp() const { return this->up; }
	// 前方向取得
	DirectX::XMFLOAT3 GetForward() const { return this->forward; }
	// 右方向取得
	DirectX::XMFLOAT3 GetRight() const { return this->right; }
	// メインカメラであるか
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

