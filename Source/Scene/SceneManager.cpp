#include "SceneManager.h"

// �X�V����[10]
void SceneManager::Update(float elpsedTime)
{
    if (nextScene != nullptr)
    {
        Clear();

        // �V�����V�[����ݒ�
        currentScene = nextScene;
        nextScene = nullptr;

        // �V�[������������[11]
        // =(��)�������������Ă��Ȃ��Ȃ珉��������
        if (!currentScene->IsReady())
        {
            currentScene->Initialize();
        }
    }

    if (currentScene != nullptr)
    {
        currentScene->Update(elpsedTime);
    }
}

// �`�揈��[10]
void SceneManager::Render()
{
    if (currentScene != nullptr)
    {
        currentScene->Render();
    }
}

// �V�[���N���A[10]
void SceneManager::Clear()
{
    if (currentScene != nullptr)
    {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}

// �V�[���؂�ւ�[10]
void SceneManager::ChangeScene(Scene* scene)
{
    // �V�����V�[����ݒ�
    nextScene = scene;
}
