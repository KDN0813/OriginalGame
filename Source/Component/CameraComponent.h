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
	void SetLookAt(MYVECTOR3 eye, MYVECTOR3 focus, MYVECTOR3 up);
	// パースペクティブ設定
	void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);
	// カメラの距離設定
	void SetRange(float range) { this->range = range; }
	// Y軸回転度設定
	void SetRotateY(float rotateY) { this->rotateY = rotateY; }
	// X軸回転度設定
	void SetRotateX(float rotateX) { this->rotateX = rotateX; }
	// プロジェクション行列設定
	void SetProjectionTransform(MYMATRIX projection_transform) { this->projection_transform = projection_transform; }
	// メインカメラフラグの設定
	void SetIsMainCamera(bool is_main_camera) { this->is_main_camera = is_main_camera; }

	// カメラの距離取得
	float GetRange() { return this->range; }
	// Y軸回転度取得
	float GetRotateY() { return this->rotateY; }
	// X軸回転度取得
	float GetRotateX() { return this->rotateX; }
	// 画角取得
	float GetFovY() { return this->fovY; }
	// アスペクト比取得
	float GetAspect() { return this->aspect; }
	// ニアクリップ値取得
	float GetNearZ() { return this->nearZ; }
	// ファークリップ値取得
	float GetFarZ() { return this->farZ; }
	// ビュー行列取得
	MYMATRIX GetViewTransform() { return this->view_transform; }
	// プロジェクション行列取得
	MYMATRIX GetProjectionTransform() { return this->projection_transform; }
	// 視点取得
	MYVECTOR3 GetEye() { return this->eye; }
	// 注視店取得
	MYVECTOR3 GetFocus() { return this->focus; }
	// 上方向取得
	MYVECTOR3 GetUp() { return this->world_transform.GetUp() ; }
	// 前方向取得
	MYVECTOR3 GetFront() { return this->world_transform.GetForward(); }
	// 右方向取得
	MYVECTOR3 GetRight() { return this->world_transform.GetRight(); }
	// ターゲット取得
	MYVECTOR3 GetTarget() { return this->target; }
	// メインカメラであるか
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

