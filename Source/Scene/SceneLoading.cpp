#include "SceneLoading.h"
#include "System/Misc.h"

#include "System/GameData.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"

#include "StateMachine\LoadingIconStateDerived.h"

// シェーダー
#include "Shader/SpriteShader.h"

#include "Component/SpriteComponent.h"
#include "Component/StateMachineComponent.h"
#include "Component/Transform2DComponent.h"

void SceneLoading::Initialize()
{
    // スレッド作成
    this->thread = std::make_unique<std::thread>(LoadingThred, this);

    // オブジェクト作成
    {
        // 背景スプライト
        {
            auto back_sprite = this->object_manager.Create("Back Sprite");
            // スプライト読み込み
            {
                SpriteComponent::SpriteParam param{};
                param.filename = "Data/Sprite/TitleBack.png";
                param.color = { 0.3f,0.3f, 0.3f, 1.0f };
                auto sprite = back_sprite->AddComponent<SpriteComponent>(param);
                if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
                {
                    sprite_shader->AddSprite(sprite);
                }
            }

            // transform
            {
                Transform2DComponent::Transform2DParam paam{};
                back_sprite->AddComponent<Transform2DComponent>(paam);
            }
        }
    }

    if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get() != nullptr)
    {
        // ロード中フラグ立てる
        game_data->SetIsLoading(true);
    }
}

void SceneLoading::Finalize()
{
    this->thread->join();

    if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
    {
        // ロード中フラグ解除
        game_data->SetIsLoading(false);
    }
}

void SceneLoading::Update(float elapsed_time)
{
    this->object_manager.Update(elapsed_time);

    if (next_scene->IsReady())
    {
        if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
        {
            scene_manager->ChangeScene(next_scene);
        }
    }
}

void SceneLoading::Render()
{
    // 描画準備
    Graphics::Instance graphics = Graphics::GetInstance();
    if (!graphics.Get()) return; 
    graphics->PrepareRenderTargets();

    // 2Dスプライト描画
    if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
    {
        sprite_shader->Render();
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
