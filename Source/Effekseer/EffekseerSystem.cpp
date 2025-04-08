#include "Graphics/Graphics.h"
#include "EffekseerSystem.h"
#include "Camera\CameraManager.h"
#include "System\GameTimer.h"

#include "Component\CameraComponent.h"

void EffekseerSystem::Initialize()
{
    Graphics::Instance graphics = Graphics::GetInstance();

    this->effekseer_render = EffekseerRendererDX11::Renderer::Create(graphics->GetDevice(),
        graphics->GetDeviceContext(), 2048);

    this->effekseer_manager = Effekseer::Manager::Create(2048);    // 引数は最大インスタンス数

    this->effekseer_manager->SetSpriteRenderer(this->effekseer_render->CreateSpriteRenderer());
    this->effekseer_manager->SetRibbonRenderer(this->effekseer_render->CreateRibbonRenderer());
    this->effekseer_manager->SetRingRenderer(this->effekseer_render->CreateRingRenderer());
    this->effekseer_manager->SetTrackRenderer(this->effekseer_render->CreateTrackRenderer());
    this->effekseer_manager->SetModelRenderer(this->effekseer_render->CreateModelRenderer());

    this->effekseer_manager->SetTextureLoader(this->effekseer_render->CreateTextureLoader());
    this->effekseer_manager->SetModelLoader(this->effekseer_render->CreateModelLoader());
    this->effekseer_manager->SetMaterialLoader(this->effekseer_render->CreateMaterialLoader());

    this->effekseer_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

void EffekseerSystem::Finalize()
{
}

void EffekseerSystem::Update(float elapsed_time)
{
    this->effekseer_manager->Update(1.0f);
}

void EffekseerSystem::Render()
{
    DirectX::XMFLOAT4X4 view{};
    DirectX::XMFLOAT4X4 projection{};
    {
        const auto& camera = CameraManager::GetInstance()->GetCamera(CAMERA_TYPE::MAIN);
        view = camera->GetViewTransform();
        projection = camera->GetProjectionTransform();
    }

    this->effekseer_render->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    this->effekseer_render->SetProjectionMatrix(*reinterpret_cast<const
        Effekseer::Matrix44*>(&projection));

    this->effekseer_render->BeginRendering();

    this->effekseer_manager->Draw();

    this->effekseer_render->EndRendering();
}

Effekseer::EffectRef EffekseerSystem::EffetResourceManager::LoadEffectResource(const char* filename)
{
    // エフェクト検索
    EffectMap::iterator it = this->effects.find(filename);
    // 見つかった場合
    if (it != this->effects.end())
    {
        return this->effects[filename];
    }

    // エフェクト作成
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    Effekseer::ManagerRef effekseer_manager = EffekseerSystem::GetInstance()->GetEffekseerManager();

    // マップに登録
    this->effects[filename] = Effekseer::Effect::Create(effekseer_manager, (EFK_CHAR*)utf16Filename);
    return this->effects[filename];
}

void EffekseerSystem::EffectPlay(const char* filename, const DirectX::XMFLOAT3& position, float scale)
{
    // エフェクトリソース取得
    Effekseer::EffectRef effekseer_effect = this->resource_manager.LoadEffectResource(filename);

    // エフェクト再生
    Effekseer::Handle handle = this->effekseer_manager->Play(effekseer_effect, position.x, position.y, position.z);

    // スケール設定
    this->effekseer_manager->SetScale(handle, scale, scale, scale);
}

void EffekseerSystem::EffectAllStop()
{
    this->effekseer_manager->StopAllEffects();
}

// 停止
void EffekseerSystem::EffectStop(Effekseer::Handle handle)
{
    // エフェクト停止
    this->effekseer_manager->StopEffect(handle);
}

// 座標設定
void EffekseerSystem::EffectSetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    // 座標設定
    this->effekseer_manager->SetLocation(handle, position.x, position.y, position.z);
}

// スケール変更
void EffekseerSystem::EffectSetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    // スケール設定
    this->effekseer_manager->SetScale(handle, scale.x, scale.y, scale.z);
}