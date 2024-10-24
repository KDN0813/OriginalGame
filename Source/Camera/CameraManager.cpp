#include "CameraManager.h"
#include "Graphics/Graphics.h"

#include "Component/CameraComponent.h"

#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#include <magic_enum.hpp>
#include "Object/Object.h"
#include "Graphics/Graphics.h"
#endif // _DEBUG

CameraManager::CameraManager()
    :Singleton(this)
{
    // グラフィックス取得
    Graphics* graphics = Graphics::Instance();
    // カメラパラメータ作成
    CameraComponent::CameraParam camera_param{};
    camera_param.fovY = DirectX::XMConvertToRadians(45.0f);
    camera_param.aspect = graphics->GetScreenWidth() / graphics->GetScreenHeight();
    camera_param.nearZ = 0.1f;
    camera_param.farZ = 1000.0f;
    camera_param.range = 10.0f;
    camera_param.rotateX = 0.4f;

    this->camera_pool.resize(static_cast<size_t>(CAMERA_TYPE::MAX));
    for (size_t i = 0; i < this->camera_pool.size();++i)
    {
        // カメラ作成
        this->camera_pool[i] = std::make_shared<CameraComponent>(camera_param);
        this->camera_pool[i]->SetCameraType(static_cast<CAMERA_TYPE>(i));   // カメラタイプ設定
        this->camera_pool[i]->SetIsActive(false);                           // カメラを非アクティブに設定
#ifdef _DEBUG
        // デバッグ用のカメラの名前を設定
        this->camera_name_pool.emplace_back(magic_enum::enum_name(static_cast<CAMERA_TYPE>(i)));
#endif // DEBUG
    }
    // メインカメラ設定
    this->current_camera = this->camera_pool[static_cast<size_t>(CAMERA_TYPE::MAIN)];
    this->current_camera->SetIsActive(true);    // カメラをアクティブにする
#ifdef _DEBUG
    // デバッグ用のカメラインデックス設定
    this->camera_index = static_cast<int>(CAMERA_TYPE::MAIN);
#endif // DEBUG
}

CameraManager::~CameraManager()
{
}

void CameraManager::SetCurrentCamera(CAMERA_TYPE type)
{
    assert(!IsErrorType(type));
    if (IsErrorType(type)) return;
    this->current_camera->SetIsActive(false);
    this->current_camera = this->camera_pool[static_cast<size_t>(type)];
    this->current_camera->SetIsActive(true);
#ifdef _DEBUG
    this->camera_index = static_cast<int>(type);
#endif // DEBUG
}

std::shared_ptr<CameraComponent> CameraManager::GetCamera(CAMERA_TYPE type)
{
    assert(!IsErrorType(type));
    if (IsErrorType(type)) return nullptr;

    return this->camera_pool[static_cast<size_t>(type)];
}

void CameraManager::Update(float elapsed_time)
{
}

bool CameraManager::IsErrorType(CAMERA_TYPE type)
{
    return (type < CAMERA_TYPE::NONE || CAMERA_TYPE::MAX <= type);
}

void CameraManager::Reset()
{
    for (const auto& camera : this->camera_pool)
    {
        camera->ReStart();
    }
}

DirectX::XMFLOAT4X4 CameraManager::GetViewProjection()
{
    DirectX::XMFLOAT4X4 float4x4{};
    GetViewProjectionMatrix().GetFlaot4x4(float4x4);
    return float4x4;
}

MYMATRIX CameraManager::GetViewProjectionMatrix()
{
    if (!this->current_camera)
    {
        // メインカメラが空の場合
        return MYMATRIX();
    }

    MYMATRIX View = current_camera->GetViewTransform();
    MYMATRIX Projection = current_camera->GetProjectionTransform();
    return View * Projection;
}

#ifdef _DEBUG

void CameraManager::DrawDebugGUI()
{
    if (ImGui::Begin("CameraManager", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::Button("Reset##CameraManager"))
        {
            Reset();
        }

        // カメラ切り替え
        std::string now_name{};
        now_name = magic_enum::enum_name(static_cast<CAMERA_TYPE>(this->camera_index));
        if (ImGui::ComboUI("Camera Type##CameraManager", now_name, this->camera_name_pool, this->camera_index))
        {
            SetCurrentCamera(static_cast<CAMERA_TYPE>(this->camera_index));
        }
        if (ImGui::CollapsingHeader("Target Camera"))
        {
            if (this->current_camera)
            {
                std::string owner_name = "Eroor";
                auto owner = this->current_camera->GetOwner();
                if (owner)
                {
                    owner_name = owner->GetName();
                }
                ImGui::InputTextString("Owner##CameraManager", owner_name);
                this->current_camera->DrawDebugGUI();
            }
        }
    }
    ImGui::End();
}

#endif // _DEBUG