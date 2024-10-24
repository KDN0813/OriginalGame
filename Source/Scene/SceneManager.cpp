#include "SceneManager.h"
#include "System/GameData.h"

#ifdef _DEBUG
#include <imgui.h>
#include "Scene/SceneTitle.h"
#include "Scene/SceneResult.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneLoading.h"
#endif // DEBUG


void SceneManager::Update(float elpsed_time)
{
    if (this->next_scene != nullptr)
    {
        Clear();

        this->current_scene = this->next_scene;
        this->next_scene = nullptr;

        if (!this->current_scene->IsReady())
        {
            // �Q�[����Ԃ��f�t�H���g�ɐݒ�
            GameData* game_data = GameData::Instance();
            if (game_data)
            {
                game_data->SetGameStatus(GameData::GameStatus::DEFAULT);
            }

            this->current_scene->Initialize();
        }
    }

    if (this->current_scene != nullptr)
    {
        this->current_scene->Update(elpsed_time);
    }
}

void SceneManager::Render()
{
    if (this->current_scene != nullptr)
    {
        this->current_scene->Render();
    }
}

void SceneManager::Clear()
{
    if (this->current_scene != nullptr)
    {
        this->current_scene->Finalize();
        delete this->current_scene;
        this->current_scene = nullptr;
    }
}

void SceneManager::ChangeScene(Scene* scene)
{
    this->next_scene = scene;
}

#ifdef _DEBUG

void SceneManager::DrawDebugGUI()
{
    // �V�[�����̃f�o�b�O����
    this->current_scene->DebugDrawGUI();
    
    if (ImGui::Begin("Scene Manager"))
    {
        // ���[�h�V�[���łȂ��Ȃ�\������
        if (dynamic_cast<SceneLoading*>(this->current_scene) == nullptr)
        {
            // �V�[���̃��Z�b�g
            if (ImGui::Button("ReStart"))
            {
                this->current_scene->ReStart();
            }
            // ���ڃV�[����؂�ւ���
            if(ImGui::CollapsingHeader("Chenge Scene", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (dynamic_cast<SceneTitle*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneTitle"))
                {
                    ChangeScene(new SceneTitle);
                }
                if (dynamic_cast<SceneGame*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneGame"))
                {
                    ChangeScene(new SceneGame);
                }
                if (dynamic_cast<SceneResult*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneResult"))
                {
                    ChangeScene(new SceneResult);
                }
            }
            // ���[�h������Ő؂�ւ���
            if (ImGui::CollapsingHeader("Chenge Load cene", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (dynamic_cast<SceneTitle*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneTitle##Load"))
                {
                    ChangeScene(new SceneLoading(new SceneTitle));
                }
                if (dynamic_cast<SceneGame*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneGame##Load"))
                {
                    ChangeScene(new SceneLoading(new SceneGame));
                }
                if (dynamic_cast<SceneResult*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneResult##Load"))
                {
                    ChangeScene(new SceneLoading(new SceneResult));
                }
            }
        }
    }
    ImGui::End();
}

#endif // DEBUG