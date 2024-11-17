#pragma once
#include "Component.h"
#include <DirectXMath.h>
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG


class Transform3DComponent : public Component
{
public:
    struct Transform3DParam
    {
        DirectX::XMFLOAT3 local_position{};
        DirectX::XMFLOAT3 local_angle{};
        DirectX::XMFLOAT3 local_scale{};
    };
public:
    Transform3DComponent(Transform3DParam param);
    ~Transform3DComponent() {};

    // リスタート処理
    void ReStart() override { this->world_dirty_flag = true; this->local_dirty_flag = true;  this->param = this->default_param; };      // パラメータの初期化
    void Update(float elapsed_time)override;
    const char* GetName()const override { return "TransformComponent"; }
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::MEDIUM; };

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
    // ワールドポジションの更新
    void UpdateWorldPosition();
    void UpdateWorldPosition(MYMATRIX Parent_transform);


#pragma region set・get parame
    // ワールドパラメータの取得
    const DirectX::XMFLOAT4X4& GetWolrdTransform();
    DirectX::XMFLOAT3 GetWorldPosition();

    // ローカルパラメータの取得
    const DirectX::XMFLOAT4X4& GetLocalTransform();

    DirectX::XMFLOAT3 GetLocalPosition()const noexcept { return this->param.local_position; }
    void SetLocalPosition(DirectX::XMFLOAT3 pos)noexcept;
    DirectX::XMFLOAT3 AddLocalPosition(DirectX::XMFLOAT3 vec);

    DirectX::XMFLOAT3 GetLocalAngle()const noexcept { return this->param.local_angle; }
    void SetLocalAngle(DirectX::XMFLOAT3 angle)noexcept;
    
    DirectX::XMFLOAT3 GetLocalScale()const noexcept { return this->param.local_scale; }
    void SetLocalScale(DirectX::XMFLOAT3 scale)noexcept;
    
    // ワールドパラメータの更新が必要なことを示すフラグを設定する
    void SetWorldDirtyFlag() { this->world_dirty_flag = true; }
#pragma endregion set parame
private:
    Transform3DParam param;
    Transform3DParam default_param;
    DirectX::XMFLOAT4X4 local_transform{};
    DirectX::XMFLOAT4X4 world_transform{};
    DirectX::XMFLOAT3 world_position{};
    bool world_dirty_flag = true;  // ワールドパラメータの更新が必要かを示すフラグ
    bool local_dirty_flag = true;  // ローカルパラメータの更新が必要かを示すフラグ
private:
    std::weak_ptr<Transform3DComponent> parent_ransform_Wptr;

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