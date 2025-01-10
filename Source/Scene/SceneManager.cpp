#include "SceneManager.h"
#include "System/GameData.h"

#ifdef _DEBUG
#include <imgui.h>
#include "Scene/SceneTitle.h"
#include "Scene/SceneResult.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneLoading.h"
#endif // DEBUG

#include "Object\Constant\UIConstant.h"
#include "Component\FadeControllerComponent.h"

void SceneManager::Initialize()
{
    // ���ʃI�u�W�F�N�g�쐻
    this->pause_object = UIConstant::CreatePause(this->common_object_manager.Create("PauseObject"));
    this->fade_object = UIConstant::CreateFadeObject(this->common_object_manager.Create("FadeObject"));

    // �t�F�[�h�R���g���[���擾
    this->fade_controller_Wptr = this->fade_object->GetComponent<FadeControllerComponent>();
}

void SceneManager::Update(float elpsed_time)
{
    // ���ʃI�u�W�F�N�g�X�V
    this->common_object_manager.Update(elpsed_time);

    if (this->next_scene != nullptr)
    {
        Clear();

        this->current_scene = this->next_scene;
        this->next_scene = nullptr;

        if (!this->current_scene->IsReady())
        {
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
    
    if (ImGui::Begin("Common Object"))
    {
        if (ImGui::CollapsingHeader(this->pause_object->GetNameCStr()))
        {
            this->pause_object->DrawDebugGUI();
        }

        if (ImGui::CollapsingHeader(this->fade_object->GetNameCStr()))
        {
            this->fade_object->DrawDebugGUI();
        }
    }
    ImGui::End();

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