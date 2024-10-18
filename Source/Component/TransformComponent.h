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

    // ���X�^�[�g����
    void ReStart() override { this->change_value = true;  this->param = this->default_param; };      // �p�����[�^�̏�����
    void Update(float elapsed_time)override;
    const char* GetName()const override { return "TransformComponent"; }
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::MEDIUM; };

    void UpdateTransform();
    const DirectX::XMFLOAT4X4& GetWolrdTransform();
    const DirectX::XMFLOAT4X4& GetLocalTransform() { return this->local_transform; }
    void SetTransform(DirectX::XMFLOAT4X4 transform) { this->local_transform = transform; }

#pragma region set�Eget parame
    DirectX::XMFLOAT3 GetLocalPosition()const noexcept { return this->param.local_position; }
    void SetLocalPosition(DirectX::XMFLOAT3 pos)noexcept { this->change_value = true; this->param.local_position = pos; }
    DirectX::XMFLOAT3 AddLocalPosition(DirectX::XMFLOAT3 vec);

    DirectX::XMFLOAT3 GetLocalAngle()const noexcept { return this->param.local_angle; }
    void SetLocalAngle(DirectX::XMFLOAT3 angle)noexcept { this->change_value = true; this->param.local_angle = angle; }
    
    DirectX::XMFLOAT3 GetLocalScale()const noexcept { return this->param.local_scale; }
    void SetLocalScale(DirectX::XMFLOAT3 scale)noexcept { this->change_value = true; this->param.local_scale = scale; }
    
    DirectX::XMFLOAT3 GetWorldPosition();
    void SetWorldPosition();
    void SetWorldPosition(std::shared_ptr<Transform3DComponent> parent_transform);

    bool GetChangeValue();
#pragma endregion set parame
private:
    Transform3DParam param;
    Transform3DParam default_param;
    DirectX::XMFLOAT4X4 local_transform{};
    DirectX::XMFLOAT4X4 world_transform{};
    DirectX::XMFLOAT3 world_position{};
    bool change_value = false;
private:
    std::weak_ptr<Transform3DComponent> parent_ransform_Wptr;

#ifdef _DEBUG
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()override;
    /**
    * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
    */
    void DrawDebugPrimitive() override;
    /**
    * �f�o�b�O�v���~�e�B�u�\���pImGui
    */
    void DrawDebugPrimitiveGUI() override;
    // �f�o�b�O�v���~�e�B�u��\������
    bool IsDebugPrimitive()override { return true; }
private:
    SphereParam sphere_world_position;
#endif // _DEBUG
};