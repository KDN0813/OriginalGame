#include "SceneLoading.h"
#include "System/Misc.h"

#include "System/GameData.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"

#include "Component/SpriteComponent.h"

void SceneLoading::Initialize()
{
    // �X���b�h�쐬
    this->thread = std::make_unique<std::thread>(LoadingThred, this);

    // �V�F�[�_�[�쐬
    {
        this->sprite_shader = std::make_unique<SpriteShader>();
    }

    // �I�u�W�F�N�g�쐬
    {
        auto object = this->object_manager.Create();
        // �X�v���C�g�쐬
        {
            SpriteComponent::SpriteParam param{};
            param.filename = "Data/Debug/Sprite/Title.png";
            param.color = { 0.0f,0.8f, 0.0f, 1.0f };
            auto sprite = object->AddComponent<SpriteComponent>(param);
            this->sprite_shader->AddSprite(sprite);
        }
    }

    if (GameData* gamedata = GameData::Instance())
    {
        // ���[�h���t���O���Ă�
        gamedata->SetIsLoading(true);
    }
}

void SceneLoading::Finalize()
{
    this->thread->join();

    if (GameData* gamedata = GameData::Instance())
    {
        // ���[�h���t���O����
        gamedata->SetIsLoading(false);
    }
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
    // �`�揀��
    Graphics* graphics = Graphics::Instance();
    graphics->PrepareRenderTargets();

    // 2D�X�v���C�g�`��
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
