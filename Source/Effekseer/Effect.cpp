#include "Effect.h"
#include "EffekseerManager.h"

Effect::Effect(const char* filename)
{
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    this->effekseer_effect = Effekseer::Effect::Create(effekseer_manager, (EFK_CHAR*)utf16Filename);
}

Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
    // EffekseerManager�擾
    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    // �G�t�F�N�g�Đ�
    Effekseer::Handle handle = effekseer_manager->Play(this->effekseer_effect, position.x, position.y,
        position.z);

    // �X�P�[���ݒ�
    effekseer_manager->SetScale(handle, scale, scale, scale);

    // �n���h����Ԃ�
    return handle;
}

// ��~
void Effect::Stop(Effekseer::Handle handle)
{
    // EffekseerManager�擾
    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    // �G�t�F�N�g��~
    effekseer_manager->StopEffect(handle);
}

// ���W�ݒ�
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    // EffekseerManager�擾
    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    // ���W�ݒ�
    effekseer_manager->SetLocation(handle, position.x, position.y, position.z);
}

// �X�P�[���ύX
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    // EffekseerManager�擾
    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    // �X�P�[���ݒ�
    effekseer_manager->SetScale(handle, scale.x, scale.y, scale.z);
}