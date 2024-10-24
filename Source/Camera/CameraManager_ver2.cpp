#include "CameraManager_ver2.h"
#include "Graphics/Graphics.h"

#include "Component/CameraComponent_ver2.h"

#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#include <magic_enum.hpp>
#include "Object/Object.h"
#include "Graphics/Graphics.h"
#endif // _DEBUG

CameraManager_ver2::CameraManager_ver2()
    :Singleton(this)
{
    // グラフィックス取得
    Graphics* graphics = Graphics::Instance();
    // カメラパラメータ作成
    CameraComponent_ver2::CameraParam camera_param{};
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
        this->camera_pool[i] = std::make_shared<CameraComponent_ver2>(camera_param);
#ifdef _DEBUG
        // デバッグ用のカメラの名前を設定
        this->camera_name_pool.emplace_back(magic_enum::enum_name(static_cast<CAMERA_TYPE>(i)));
#endif // DEBUG
    }
    // メインカメラ設定
    this->main_camera = this->camera_pool[static_cast<size_t>(CAMERA_TYPE::MAIN)];
#ifdef _DEBUG
    // デバッグ用のカメラインデックス設定
    this->camera_index = static_cast<int>(CAMERA_TYPE::MAIN);
#endif // DEBUG
}

CameraManager_ver2::~CameraManager_ver2()
{
}

void CameraManager_ver2::SetMainCamera(CAMERA_TYPE type)
{
    assert(!IsErrorType(type));
    if (IsErrorType(type)) return;
    this->main_camera = this->camera_pool[static_cast<size_t>(type)];
#ifdef _DEBUG
    this->camera_index = static_cast<int>(type);
#endif // DEBUG
}

std::shared_ptr<CameraComponent_ver2> CameraManager_ver2::GetCamera(CAMERA_TYPE type)
{
    assert(!IsErrorType(type));
    if (IsErrorType(type)) return nullptr;

    return this->camera_pool[static_cast<size_t>(type)];
}

void CameraManager_ver2::Update(float elapsed_time)
{
}

bool CameraManager_ver2::IsErrorType(CAMERA_TYPE type)
{
    return (type < CAMERA_TYPE::NONE || CAMERA_TYPE::MAX <= type);
}

DirectX::XMFLOAT4X4 CameraManager_ver2::GetViewProjection()
{
    DirectX::XMFLOAT4X4 float4x4{};
    GetViewProjectionMatrix().GetFlaot4x4(float4x4);
    return float4x4;
}

MYMATRIX CameraManager_ver2::GetViewProjectionMatrix()
{
    if (!this->main_camera)
    {
        // メインカメラが空の場合
        return MYMATRIX();
    }

    MYMATRIX View = main_camera->GetViewTransform();
    MYMATRIX Projection = main_camera->GetProjectionTransform();
    return View * Projection;
}

#ifdef _DEBUG

void CameraManager_ver2::DrawDebugGUI()
{
    if (ImGui::Begin("CameraManager", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::CollapsingHeader("Main Camera"))
        {
            // カメラ切り替え
            std::string now_name{};
            now_name = magic_enum::enum_name(static_cast<CAMERA_TYPE>(this->camera_index));
            if (ImGui::ComboUI("MainCamera##CameraManager", now_name, this->camera_name_pool, this->camera_index))
            {
                this->main_camera = this->camera_pool[static_cast<size_t>(this->camera_index)];
            }

            if (this->main_camera)
            {
                std::string owner_name = "Eroor";
                auto owner = this->main_camera->GetOwner();
                if (owner)
                {
                    owner_name = owner->GetName();
                }
                ImGui::InputTextString("Owner##CameraManager", owner_name);
                this->main_camera->DrawDebugGUI();
            }
        }
    }
    ImGui::End();
}

#endif // _DEBUG