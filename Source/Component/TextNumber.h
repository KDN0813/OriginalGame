#pragma once
#include <string>
#include "Component.h"
#include "Sprite\Sprite.h"

class TextNumber : public Component
{
public:
    struct TextParam
    {
        std::string font_name = {};
        DirectX::XMFLOAT2 pos = { 0.0f,0.0f };
        int value = 0;  // �\������l
        float font_size = 0.0f; // �P����������̃T�C�Y
        float angle = 0.0f;
        DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
        Sprite::CENTER_TYPE center_type = Sprite::CENTER_TYPE::CENTER;
    };
public:
    TextNumber(TextParam param) : param(param), default_param(param) {};
    ~TextNumber() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override {};

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };

    // �擾�E�ݒ�֐�
    int GetDrawValue()const { return this->param.value; }
    void SetDrawValue(const int v) { this->param.value = v; }

private:
    std::unique_ptr<Sprite> sprite;
    TextParam param;
    TextParam default_param;

    DirectX::XMFLOAT2 display_size = { 1.0f,1.0f };
    DirectX::XMFLOAT2 clip_pos = { 0.0f,0.0f };
    DirectX::XMFLOAT2 clip_size = { 1.0f,1.0f };

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override {};
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitive�����݂��邩
#endif // DEBUG
};

