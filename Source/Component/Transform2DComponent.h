#pragma once
#include "Component.h"
#include <DirectXMath.h>
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG
#include "System\MyMath\MYMATRIX.h"

class Transform2DComponent : public Component
{
public:
    struct Transform2DParam
    {
        DirectX::XMFLOAT2 local_position{};
        float local_angle{};
        DirectX::XMFLOAT2 local_scale{ 1.0f,1.0f };
#ifdef _DEBUG
        bool enable_ndc_debug = true;
#endif // _DEBUG
    };
public:
    Transform2DComponent(Transform2DParam param);
    ~Transform2DComponent() {};

    // リスタート処理
    void ReStart() override;
    void Update(float elapsed_time)override;
    const char* GetName()const override { return "Transform2DComponent"; }
    const PRIORITY GetPriority()const noexcept { return PRIORITY::MEDIUM; };

    /**
     * ローカルパラメータの更新
     * [更新内容]
     * ・ローカルトランスフォーム
     */
    void UpdateLocalParam();
    /**
     * ワールドパラメータの更新
     * [更新内容]
     * ・ワールドトランスフォーム
     * ・ワールドポジション
     */
    void UpdateWorldParam();

#pragma region set・get parame
    // ワールドパラメータの取得
    const DirectX::XMFLOAT4X4& GetWolrdTransform();
    DirectX::XMFLOAT2 GetWorldPosition();
    DirectX::XMFLOAT2 GetWorldScale();
    float GetWorldAngle();

    // ローカルパラメータの取得
    const DirectX::XMFLOAT4X4& GetLocalTransform();

    DirectX::XMFLOAT2 GetLocalPosition()const noexcept { return this->param.local_position; }
    void SetLocalPosition(DirectX::XMFLOAT2 pos)noexcept;
    DirectX::XMFLOAT2 AddLocalPosition(DirectX::XMFLOAT2 vec);

    float GetLocalAngle()const noexcept { return this->param.local_angle; }
    void SetLocalAngle(float angle)noexcept;

    DirectX::XMFLOAT2 GetLocalScale()const noexcept { return this->param.local_scale; }
    void SetLocalScale(DirectX::XMFLOAT2 scale)noexcept;

private:
    // ワールドパラメータの更新が必要なことを示すフラグを設定する
    void SetWorldDirtyFlag() { this->world_dirty_flag = true; }
    // 引数のオブジェクトのdirty_flagを立てる
    void SetDirtyFlag(const std::shared_ptr<Object>&);
#pragma endregion set parame
private:
    Transform2DParam param;
    Transform2DParam default_param;
    DirectX::XMFLOAT4X4 local_transform{};
    DirectX::XMFLOAT4X4 world_transform{};
    DirectX::XMFLOAT2 world_position{};
    DirectX::XMFLOAT2 world_scale{};
    float world_angle{};
    bool world_dirty_flag = true;  // ワールドパラメータの更新が必要かを示すフラグ
    bool local_dirty_flag = true;  // ローカルパラメータの更新が必要かを示すフラグ
private:
    std::weak_ptr<Transform2DComponent> parent_ransform_Wptr;

#ifdef _DEBUG
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
    /**
    * デバックの情報を3D画面上に出力する関数
    */
    void DrawDebugPrimitive() override;
    /**
    * デバッグプリミティブ表示用ImGui
    */
    void DrawDebugPrimitiveGUI() override;
    // デバッグプリミティブを表示する
    bool IsDebugPrimitive()override { return true; }
private:
    SphereParam sphere_world_position;
#endif // _DEBUG
};