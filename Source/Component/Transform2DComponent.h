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

    // ���X�^�[�g����
    void ReStart() override;
    void Update(float elapsed_time)override;
    const char* GetName()const override { return "Transform2DComponent"; }
    const PRIORITY GetPriority()const noexcept { return PRIORITY::MEDIUM; };

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

#pragma region set�Eget parame
    // ���[���h�p�����[�^�̎擾
    const DirectX::XMFLOAT4X4& GetWolrdTransform();
    DirectX::XMFLOAT2 GetWorldPosition();
    DirectX::XMFLOAT2 GetWorldScale();
    float GetWorldAngle();

    // ���[�J���p�����[�^�̎擾
    const DirectX::XMFLOAT4X4& GetLocalTransform();

    DirectX::XMFLOAT2 GetLocalPosition()const noexcept { return this->param.local_position; }
    void SetLocalPosition(DirectX::XMFLOAT2 pos)noexcept;
    DirectX::XMFLOAT2 AddLocalPosition(DirectX::XMFLOAT2 vec);

    float GetLocalAngle()const noexcept { return this->param.local_angle; }
    void SetLocalAngle(float angle)noexcept;

    DirectX::XMFLOAT2 GetLocalScale()const noexcept { return this->param.local_scale; }
    void SetLocalScale(DirectX::XMFLOAT2 scale)noexcept;

private:
    // ���[���h�p�����[�^�̍X�V���K�v�Ȃ��Ƃ������t���O��ݒ肷��
    void SetWorldDirtyFlag() { this->world_dirty_flag = true; }
    // �����̃I�u�W�F�N�g��dirty_flag�𗧂Ă�
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
    bool world_dirty_flag = true;  // ���[���h�p�����[�^�̍X�V���K�v���������t���O
    bool local_dirty_flag = true;  // ���[�J���p�����[�^�̍X�V���K�v���������t���O
private:
    std::weak_ptr<Transform2DComponent> parent_ransform_Wptr;

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