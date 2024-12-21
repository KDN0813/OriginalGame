#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Component/BaseSpriteComponent.h"
#include "Sprite/Sprite.h"
#include "System\MyMath\MyMathf.h"

class Transform2DComponent;

class SpriteComponent : public BaseSpriteComponent
{
public:
    struct SpriteParam
    {
        std::string filename = {};
        DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
        DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
        DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
        Sprite::CENTER_TYPE center_type = Sprite::CENTER_TYPE::TOP_LEFT;
    };
public:
    SpriteComponent(const SpriteParam&);
    virtual ~SpriteComponent() {};

    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override { this->param = this->default_param; };      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "SpriteComponent"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };
    
    // �`�揈��
    void Render(ID3D11DeviceContext* dc) override;

private:
    std::unique_ptr<Sprite> sprite;
    SpriteParam param;
    SpriteParam default_param;
private:
    std::weak_ptr<Transform2DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() { return false; }   // DebugPrimitive�����݂��邩

private:
    std::vector<std::string> center_type_name_pool;
#endif // DEBUG
};

