#pragma once

#include <DirectXMath.h>

// TODO (06/25)モデル描画するための簡易バージョン
// 後に複数のカメラを使い分けれるよう改造する
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
     * 指定方向を向く
     * 
     * \param eye カメラの視点
     * \param focus カメラの注視点
     * \param up カメラの上方向
     */
    void SetLookAt(
        const DirectX::XMFLOAT3& eye
        , const DirectX::XMFLOAT3& focus
        , const DirectX::XMFLOAT3& up
    );

    /**
     * プロジェクション行列を作成
     * 
     * \param fovY カメラの視野角
     * \param aspect アスペクト比(画面の縦横比)
     * \param nearX クリップ距離(近)
     * \param farZ クリップ距離(遠)
     */
    void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);
    
    // 各種取得・設定関数
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
