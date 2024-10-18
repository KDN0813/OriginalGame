#pragma once
#include "Component.h"
#include "Sprite/Sprite.h"

class SpriteComponent : public Component
{
public:
    struct SpriteParam
    {
        const char* filename = nullptr;
        DirectX::XMFLOAT2 display_pos = { 0.0f,0.0f };
        DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
        DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
        DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };
        float angle = 0.0f;
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
    };
public:
    SpriteComponent(const SpriteParam&);
    virtual ~SpriteComponent() {};

    // �J�n�֐�
    void Start()  override {};
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
    void Render(ID3D11DeviceContext* dc);
private:
    std::unique_ptr<Sprite> sprite;
    SpriteParam param;
    SpriteParam default_param;

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
#endif // DEBUG
};

