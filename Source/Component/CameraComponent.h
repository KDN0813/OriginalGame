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

	// 開始関数
	void Start()override;
	// 修了関数
	void End()override;
	// リスタート処理
	void ReStart();
    // 更新関数
    void Update(float elapsed_time);
    // 名前取得
    const char* GetName()const { return "CameraComponent"; };
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOWEST; }

	/**
	 * \fn SetMainCamera
	 * \brief メインカメラに設定する
	 * 
	 */
	void SetMainCamera();

	// 視点設定
	void SetEye(DirectX::XMFLOAT3 eye) { this->change_value = true; this->camera_param.eye = eye; }
	// 注視店設定
	void SetFocus(DirectX::XMFLOAT3 focus) { this->change_value = true; this->camera_param.focus = focus; }
	// 右方向設定
	void  SetRight(DirectX::XMFLOAT3 right) { this->change_value = true; this->camera_param.right = right; }
	// 上方向設定
	void SetUp(DirectX::XMFLOAT3 up) { this->change_value = true; this->camera_param.up = up; }
	// 前方向設定
	void SetForward(DirectX::XMFLOAT3 forward) { this->change_value = true; this->camera_param.forward = forward; }
	// 画角設定
	void SetFovY(float fovY) { this->change_value = true; this->camera_param.fovY = fovY; }
	// アスペクト比設定
	void SetAspect(float aspect) { this->change_value = true; this->camera_param.aspect = aspect; }
	// ニアクリップ値設定
	void SetNearZ(float nearZ) { this->change_value = true; this->camera_param.nearZ = nearZ; }
	// ファークリップ値設定
	void SetFarZ(float farZ) { this->change_value = true; this->camera_param.farZ = farZ; }
	// カメラの距離設定
	void SetRange(float range) { this->change_value = true; this->camera_param.range = range; }
	// Y軸回転度設定
	void SetRotateY(float rotateY) { this->change_value = true; this->camera_param.rotateY = rotateY; }
	// X軸回転度設定
	void SetRotateX(float rotateX) { this->change_value = true; this->camera_param.rotateX = rotateX; }
	// メインカメラフラグの設定
	void SetIsMainCamera(bool is_main_camera) { this->is_main_camera = is_main_camera; }

	// ビュー行列取得
	DirectX::XMFLOAT4X4 GetViewTransform() const { return this->view_transform; }
	// プロジェクション行列取得
	DirectX::XMFLOAT4X4 GetProjectionTransform() const { return this->projection_transform; }
	// 視点取得
	DirectX::XMFLOAT3 GetEye() const { return this->camera_param.eye; }
	// 注視店取得
	DirectX::XMFLOAT3 GetFocus() const { return this->camera_param.focus; }
	// 右方向取得
	DirectX::XMFLOAT3 GetRight() const { return this->camera_param.right; }
	// 上方向取得
	DirectX::XMFLOAT3 GetUp() const { return this->camera_param.up; }
	// 前方向取得
	DirectX::XMFLOAT3 GetForward() const { return this->camera_param.forward; }
	// 画角取得
	float GetFovY() { return this->camera_param.fovY; }
	// アスペクト比取得
	float GetAspect() { return this->camera_param.aspect; }
	// ニアクリップ値取得
	float GetNearZ() { return this->camera_param.nearZ; }
	// ファークリップ値取得
	float GetFarZ() { return this->camera_param.farZ; }
	// カメラの距離取得
	float GetRange() { return this->camera_param.range; }
	// Y軸回転度取得
	float GetRotateY() { return this->camera_param.rotateY; }
	// X軸回転度取得
	float GetRotateX() { return this->camera_param.rotateX; }
	// メインカメラであるか
	bool GetIsMainCamera() { return this->is_main_camera; }
private:
	// 指定方向を向く
	void SetLookAt(MYVECTOR3 Eye, MYVECTOR3 Focus, MYVECTOR3 Up);
	// パースペクティブ設定
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

