#include "SceneLoading.h"
#include "System/Misc.h"

#include "System/GameData.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"

#include "StateMachine\LoadingIconStateDerived.h"

// �V�F�[�_�[
#include "Shader/SpriteShader.h"

#include "Component/SpriteComponent.h"
#include "Component/StateMachineComponent.h"
#include "Component/Transform2DComponent.h"
#include "Component\SpriteFadeControllerComponent.h"

void SceneLoading::Initialize()
{
    // �X���b�h�쐬
    this->thread = std::make_unique<std::thread>(LoadingThread, this);

    // �I�u�W�F�N�g�쐬
    {
        // �w�i�X�v���C�g
        {
            auto back_sprite = this->object_manager.Create("Back Sprite");
            // �X�v���C�g�ǂݍ���
            {
                BaseSpriteComponent::SpriteParam sprite_param{};
                sprite_param.filename = "Data/Sprite/TitleBack.png";
                sprite_param.color = { 0.3f,0.3f, 0.3f, 1.0f };
                auto sprite = back_sprite->AddComponent<SpriteComponent>(sprite_param);
            }

            // transform
            {
                Transform2DComponent::Transform2DParam paam{};
                back_sprite->AddComponent<Transform2DComponent>(paam);
            }
        }
    }

    // ���[�h���t���O���Ă�
    {
        GameData::Instance game_data = GameData::GetInstance();
        game_data->SetIsLoading(true);
    }

    // �t�F�[�h����������
    {
        SceneManager::Instance scene_manager = SceneManager::GetInstance();
        if (const auto fade_controller = scene_manager->GetFadeControlle())
            fade_controller->SetAlpha(0.0f);
    }
}

void SceneLoading::Finalize()
{
    this->thread->join();

    // ���[�h���t���O����
    GameData::Instance game_data = GameData::GetInstance();
    game_data->SetIsLoading(false);
}

void SceneLoading::Update(float elapsed_time)
{
    this->object_manager.Update(elapsed_time);

    if (next_scene->IsReady())
    {
        SceneManager::Instance scene_manager = SceneManager::GetInstance();
        scene_manager->ChangeScene(next_scene);
    }
}

void SceneLoading::Render()
{
    // �`�揀��
    Graphics::Instance graphics = Graphics::GetInstance();
    if (!graphics.Get()) return;
    graphics->PrepareRenderTargets();

    // 2D�X�v���C�g�`��
    {
        SpriteShader::Instance sprite_shader = SpriteShader::GetInstance();
        sprite_shader->Render();
    }
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
    HRESULT hr = S_OK;
    hr = CoInitialize(nullptr);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    scene->next_scene->Initialize();
    CoUninitialize();
    scene->next_scene->SetReady();
}
