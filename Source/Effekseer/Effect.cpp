#include "Effect.h"
#include "EffectManager.h"

// �R���X�g���N�^[10]
Effect::Effect(const char* filename)
{
    // �G�t�F�N�g��ǂݍ��݂���O�Ƀ��b�N����
    // ���}���`�X���b�h��Effect���쐬�����DeviceContex�𓯎��A�N�Z�X����
    // �t���[�Y����\���������Ŕr���I���䂷��
    // �f�X�g���N�^�Ń��b�N����������

    // Effekseer�̃��\�[�X��ǂݍ���
    // Effeksser��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ����ߕ����R�[�h�ϊ����K�v
    char16_t utf16Filename[256];    // (��)�ϊ����filename
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    // Effekseer::Manager���擾
    Effekseer::ManagerRef effekseerManager = EffectManager::GetInstance()->GetEffekseerManager();

    // Effekseer�G�t�F�N�g��ǂݍ���
    effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
}

// �Đ�[10]
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
    // EffekseerManager�擾
    Effekseer::ManagerRef effekseerManager = EffectManager::GetInstance()->GetEffekseerManager();

    // �G�t�F�N�g�Đ�
    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y,
        position.z);

    // �X�P�[���ݒ�
    effekseerManager->SetScale(handle, scale, scale, scale);

    // �n���h����Ԃ�
    return handle;
}

// ��~[10]
void Effect::Stop(Effekseer::Handle handle)
{
    // EffekseerManager�擾
    Effekseer::ManagerRef effekseerManager = EffectManager::GetInstance()->GetEffekseerManager();

    // �G�t�F�N�g��~
    effekseerManager->StopEffect(handle);
}

// ���W�ݒ�
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    // EffekseerManager�擾
    Effekseer::ManagerRef effekseerManager = EffectManager::GetInstance()->GetEffekseerManager();

    // ���W�ݒ�
    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

// �X�P�[���ύX
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    // EffekseerManager�擾
    Effekseer::ManagerRef effekseerManager = EffectManager::GetInstance()->GetEffekseerManager();

    // �X�P�[���ݒ�
    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}