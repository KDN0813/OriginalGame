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
    void ReStart() override { this->world_dirty_flag = true; this->local_dirty_flag = true;  this->param = this->default_param; };      // �p�����[�^�̏�����
    void Update(float elapsed_time)override;
    const char* GetName()const override { return "TransformComponent"; }
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::MEDIUM; };

    /**
     * ���[�J���p�����[�^�̍X�V
     * [�X�V���e]
     * �E���[�J���g�����X�t�H�[��
     */
    void UpdateLocalParam();
    /**
     * ���[���h�p�����[�^�̍X�V
     * [�X�V���e]
     * �E���[���h�g�����X�t�H�[��
     * �E���[���h�|�W�V����
     */
    void UpdateWorldParam();
    // ���[���h�|�W�V�����̍X�V
    void UpdateWorldPosition();
    void UpdateWorldPosition(MYMATRIX Parent_transform);


#pragma region set�Eget parame
    // ���[���h�p�����[�^�̎擾
    const DirectX::XMFLOAT4X4& GetWolrdTransform();
    DirectX::XMFLOAT3 GetWorldPosition();

    // ���[�J���p�����[�^�̎擾
    const DirectX::XMFLOAT4X4& GetLocalTransform();

    DirectX::XMFLOAT3 GetLocalPosition()const noexcept { return this->param.local_position; }
    void SetLocalPosition(DirectX::XMFLOAT3 pos)noexcept;
    DirectX::XMFLOAT3 AddLocalPosition(DirectX::XMFLOAT3 vec);

    DirectX::XMFLOAT3 GetLocalAngle()const noexcept { return this->param.local_angle; }
    void SetLocalAngle(DirectX::XMFLOAT3 angle)noexcept;
    
    DirectX::XMFLOAT3 GetLocalScale()const noexcept { return this->param.local_scale; }
    void SetLocalScale(DirectX::XMFLOAT3 scale)noexcept;
    
    // ���[���h�p�����[�^�̍X�V���K�v�Ȃ��Ƃ������t���O��ݒ肷��
    void SetWorldDirtyFlag() { this->world_dirty_flag = true; }
#pragma endregion set parame
private:
    Transform3DParam param;
    Transform3DParam default_param;
    DirectX::XMFLOAT4X4 local_transform{};
    DirectX::XMFLOAT4X4 world_transform{};
    DirectX::XMFLOAT3 world_position{};
    bool world_dirty_flag = true;  // ���[���h�p�����[�^�̍X�V���K�v���������t���O
    bool local_dirty_flag = true;  // ���[�J���p�����[�^�̍X�V���K�v���������t���O
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