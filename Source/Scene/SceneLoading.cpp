#include "SceneLoading.h"
#include "System/Misc.h"

#include "Graphics/Graphics.h"
#include "SceneManager.h"

#include "Component/SpriteComponent.h"

void SceneLoading::Initialize()
{
    // スレッド作成
    this->thread = std::make_unique<std::thread>(LoadingThred, this);

    // シェーダー作成
    {
        this->sprite_shader = std::make_unique<SpriteShader>();
    }

    // オブジェクト作成
    {
        auto object = this->object_manager.Create();
        // スプライト作成
        {
            SpriteComponent::SpriteParam param{};
            param.filename = "Data/Debug/Sprite/Title.png";
            param.color = { 0.0f,0.8f, 0.0f, 1.0f };
            auto sprite = object->AddComponent<SpriteComponent>(param);
            this->sprite_shader->AddSprite(sprite);
        }
    }
}

void SceneLoading::Finalize()
{
    this->thread->join();
}

void SceneLoading::Update(float elapsed_time)
{
    this->object_manager.Update(elapsed_time);

    if (next_scene->IsReady())
    {
        SceneManager::Instance()->ChangeScene(next_scene);
    }
}

void SceneLoading::Render()
{
    Graphics* graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics->GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics->GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics->GetDepthStencilView();

    FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 2Dスプライト描画
    {
        this->sprite_shader->Render();
    }
}

void SceneLoading::LoadingThred(SceneLoading* scene)
{
    HRESULT hr = S_OK;
    hr = CoInitialize(nullptr);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    scene->next_scene->Initialize();
    CoUninitialize();
    scene->next_scene->SetReady();
}
