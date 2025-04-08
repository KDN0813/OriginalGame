#include "Graphics/Graphics.h"
#include "EffectManager.h"
#include "Camera\CameraManager.h"

#include "Component\CameraComponent.h"

void EffectManager::Initialize()
{
    Graphics::Instance graphics = Graphics::GetInstance();

    effekseer_render = EffekseerRendererDX11::Renderer::Create(graphics->GetDevice(),
        graphics->GetDeviceContext(), 2048);

    effekseer_manager = Effekseer::Manager::Create(2048);    // 引数は最大インスタンス数

    effekseer_manager->SetSpriteRenderer(effekseer_render->CreateSpriteRenderer());
    effekseer_manager->SetRibbonRenderer(effekseer_render->CreateRibbonRenderer());
    effekseer_manager->SetRingRenderer(effekseer_render->CreateRingRenderer());
    effekseer_manager->SetTrackRenderer(effekseer_render->CreateTrackRenderer());
    effekseer_manager->SetModelRenderer(effekseer_render->CreateModelRenderer());

    effekseer_manager->SetTextureLoader(effekseer_render->CreateTextureLoader());
    effekseer_manager->SetModelLoader(effekseer_render->CreateModelLoader());
    effekseer_manager->SetMaterialLoader(effekseer_render->CreateMaterialLoader());

    effekseer_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

void EffectManager::Finalize()
{
}

void EffectManager::Update(float elapsed_time)
{
    // エフェクト更新処理(引数にはframeの経過時間を渡す)
    effekseer_manager->Update(elapsed_time * 60.0f);
}

void EffectManager::Render()
{
    DirectX::XMFLOAT4X4 view{};
    DirectX::XMFLOAT4X4 projection{};
    {
        const auto& camera = CameraManager::GetInstance()->GetCamera(CAMERA_TYPE::MAIN);
        view = camera->GetViewTransform();
        projection = camera->GetProjectionTransform();
    }

    effekseer_render->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseer_render->SetProjectionMatrix(*reinterpret_cast<const
        Effekseer::Matrix44*>(&projection));

    effekseer_render->BeginRendering();

    effekseer_manager->Draw();

    effekseer_render->EndRendering();
}
