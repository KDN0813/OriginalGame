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
    // EffekseerManager取得
    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    // エフェクト再生
    Effekseer::Handle handle = effekseer_manager->Play(this->effekseer_effect, position.x, position.y,
        position.z);

    // スケール設定
    effekseer_manager->SetScale(handle, scale, scale, scale);

    // ハンドルを返す
    return handle;
}

// 停止
void Effect::Stop(Effekseer::Handle handle)
{
    // EffekseerManager取得
    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    // エフェクト停止
    effekseer_manager->StopEffect(handle);
}

// 座標設定
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    // EffekseerManager取得
    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    // 座標設定
    effekseer_manager->SetLocation(handle, position.x, position.y, position.z);
}

// スケール変更
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    // EffekseerManager取得
    Effekseer::ManagerRef effekseer_manager = EffekseerManager::GetInstance()->GetEffekseerManager();

    // スケール設定
    effekseer_manager->SetScale(handle, scale.x, scale.y, scale.z);
}