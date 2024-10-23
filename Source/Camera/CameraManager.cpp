#include "CameraManager.h"
#include <imgui.h>
#include "Input/Input.h"

#include "Component/CameraComponent.h"


#ifdef _DEBUG
#include "Graphics/Graphics.h"
#include "Component/CameraControllerComponent.h"

#endif // _DEBUG

CameraManager::CameraManager()
    :Singleton(this)
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::AddCamera(CameraComponent* camera)
{
    this->camera_vector.push_back(camera);
    if (!main_camera)
    {
        main_camera = camera;
    }
}

void CameraManager::RemoveCamera(CameraComponent* camera)
{
    auto it = std::find(this->camera_vector.begin(), this->camera_vector.end(), camera);
    if (it != camera_vector.end())
    {
        this->camera_vector.erase(it);
    }
}

void CameraManager::SetMainCamera(CameraComponent* camera)
{
#ifdef _DEBUG
    if (this->debug_flag) return;   // デバッグ中がカメラの変更を受け付けない
#endif // _DEBUG


    auto it = std::find(this->camera_vector.begin(), this->camera_vector.end(), camera);
    if (it == camera_vector.end()) return;

    this->main_camera->SetIsMainCamera(false);

    this->main_camera = camera;
    this->main_camera->SetIsMainCamera(true);
}

void CameraManager::Update(float elapsed_time)
{
    this->main_camera;
}

#ifdef _DEBUG

void CameraManager::DrawDebugGUI()
{
    if (ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar))
    {
        // メニューバー設定
        if (ImGui::BeginMenuBar())
        {
            // デバッグカメラ切り替えメニュー
            if (this->debug_camera !=nullptr && ImGui::BeginMenu("SetCamera"))
            {
                std::string label = this->debug_flag ? "normal camera" : "debug camera";

                if (ImGui::MenuItem(label.c_str()))
                {
                    this->debug_flag = !this->debug_flag;
                    CameraManager::Instance()->SetDebugCamera();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}

void CameraManager::SetDebugCamera()
{
	if (this->debug_flag)
	{
        this->main_camera->SetIsMainCamera(false);

		this->temp_camera = this->main_camera;
		this->main_camera = this->debug_camera;
        this->debug_camera->SetIsMainCamera(true);
	}
	else
	{
        this->debug_camera->SetIsMainCamera(false);

		this->main_camera = this->temp_camera;
        this->main_camera->SetIsMainCamera(true);
	}
}

#endif // _DEBUG