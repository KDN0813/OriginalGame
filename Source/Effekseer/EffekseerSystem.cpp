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
