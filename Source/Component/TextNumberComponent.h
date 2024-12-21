#pragma once
#include <string>
#include <vector>
#include "Component/BaseSpriteComponent.h"
#include "Sprite\Sprite.h"

class Transform2DComponent;

class TextNumberComponent : public BaseSpriteComponent
{
public:
    const float NUMBER_COUNT = 10;                  // �����̐�(0�`9�܂ł̐���)
    const float FONT_WIGTH = 1.0f / NUMBER_COUNT;   // �t�H���g�̕�
public:
    struct TextParam
    {
        std::string font_name = {};
        int value = 0;  // �\������l
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
        PRIORITY draw_priority;
        Sprite::CENTER_TYPE center_type = Sprite::CENTER_TYPE::TOP_LEFT;
    };
public:
    TextNumberComponent(TextParam param) : BaseSpriteComponent(param.draw_priority), param(param), default_param(param) {};
    ~TextNumberComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "TextNumberComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // �`��֐�
    void Render(ID3D11DeviceContext* dc) override;

    // �擾�E�ݒ�֐�
    int GetDrawValue()const { return this->param.value; }
    void SetDrawValue(const int v) { this->param.value = (std::max)(v, 0); }

private:
    std::unique_ptr<Sprite> sprite;
    TextParam param;
    TextParam default_param;

    DirectX::XMFLOAT2 font_draw_size = {};  // �P����������̕`��T�C�Y
    DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
    DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
    DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };

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
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitive�����݂��邩

private:
    std::vector<std::string> center_type_name_pool;
#endif // DEBUG
};

