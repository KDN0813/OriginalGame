#include "SceneLoading.h"
#include "System/Misc.h"

#include "System/GameData.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"

#include "StateMachine\PlayerStateDerived.h"

#include "Component/SpriteComponent.h"
#include "Component/StateMachineComponent.h"

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
        // 背景スプライト
        {
            auto back_sprite = this->object_manager.Create("Back Sprite");
            // スプライト読み込み
            {
                SpriteComponent::SpriteParam param{};
                param.filename = "Data/Sprite/TitleBack.png";
                param.color = { 0.3f,0.3f, 0.3f, 1.0f };
                auto sprite = back_sprite->AddComponent<SpriteComponent>(param);
                this->sprite_shader->AddSprite(sprite);
            }
        }

        // ロード演出用スプライト
        {
            auto load_idle_sprite = this->object_manager.Create("Load Idle Sprite");
            // スプライト読み込み
            {
                SpriteComponent::SpriteParam param{};
                param.filename = "Data/Sprite/TitleBack.png";
                param.display_size = { 0.5f,0.5f };
                param.color = { 1.5f,1.5f, 1.5f, 1.0f };
                auto sprite = load_idle_sprite->AddComponent<SpriteComponent>(param);
                this->sprite_shader->AddSprite(sprite);
            }
            // 更新用ステートマシン読み込み
            {
                auto state_machine = load_idle_sprite->AddComponent<StateMachineComponent>();

                //state_machine.RegisterState();
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
