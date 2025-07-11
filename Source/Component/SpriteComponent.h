#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Component/BaseSpriteComponent.h"
#include "System\MyMath\MyMathf.h"

class Transform2DComponent;

class SpriteComponent : public BaseSpriteComponent
{
public:
    SpriteComponent(const BaseSpriteComponent::SpriteParam&);
    virtual ~SpriteComponent() {};

    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "SpriteComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };
    
    // �`�揈��
    void Render(ID3D11DeviceContext* dc) override;

    void SetDisplaySize(const DirectX::XMFLOAT2 size) { this->sprite_param.display_size = size; };
    void SetDisplaySizeX(const float sizeX) { this->sprite_param.display_size.x = sizeX; };
    void SetDisplaySizeY(const float sizeY) { this->sprite_param.display_size.y = sizeY; };
    // �\���T�C�Y���X�v���C�g�T�C�Y�ɍ��킹��
    void AdjustDisplaySizeToSprite();
private:
    std::unique_ptr<Sprite> sprite;
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

