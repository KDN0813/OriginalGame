#include "CameraManager.h"
#include "Graphics/Graphics.h"

#include "Component/CameraComponent.h"

#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#include "Graphics/Graphics.h"
#endif // _DEBUG
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
#include "Input\Input.h"
#include <magic_enum.hpp>
#include "Object\Object.h"
#include "Component/CameraControllerComponent.h"
#endif // DEBUG


CameraManager::CameraManager()
    :Singleton(this)
{
    // カメラパラメータ作成
    CameraComponent::CameraParam camera_param{};
    camera_param.fovY = DirectX::XMConvertToRadians(45.0f);
    // グラフィックスからアスペクト比を計算
    if (Graphics::Instance graphics = Graphics::GetInstance(); graphics.Get())
    {
        camera_param.aspect = graphics->GetScreenWidth() / graphics->GetScreenHeight();
    }
    camera_param.nearZ = 0.1f;
    camera_param.farZ = 1000.0f;
    camera_param.range = 15.0f;
    camera_param.rotateX = 0.45f;

    this->camera_pool.resize(static_cast<size_t>(CAMERA_TYPE::MAX));
    for (size_t i = 0; i < this->camera_pool.size();++i)
    {
        // カメラ作成
        this->camera_pool[i] = std::make_shared<CameraComponent>(camera_param);
        this->camera_pool[i]->SetCameraType(static_cast<CAMERA_TYPE>(i));   // カメラタイプ設定
        this->camera_pool[i]->SetIsActive(false);                           // カメラを非アクティブに設定
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
        // デバッグ用のカメラの名前を設定
        this->camera_name_pool.emplace_back(magic_enum::enum_name(static_cast<CAMERA_TYPE>(i)));
#endif // DEBUG
    }
    // メインカメラ設定
    this->current_camera = this->camera_pool[static_cast<size_t>(CAMERA_TYPE::MAIN)];
    this->current_camera->SetIsActive(true);    // カメラをアクティブにする
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
    // デバッグ用のカメラインデックス設定
    this->camera_index = static_cast<int>(CAMERA_TYPE::MAIN);
    this->debug_camera_index = static_cast<int>(CAMERA_TYPE::MAIN);

    // デバッグカメラ作成
    {
        this->debug_camera = std::make_shared<Object>();
        this->debug_camera->SetName("Debug Camera");
        this->debug_camera->AddComponent<CameraComponent>(GetCamera(CAMERA_TYPE::DEBUG));
        this->debug_camera->AddComponent<CameraControllerDebug>();
    }
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
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
    this->camera_index = static_cast<int>(type);
    this->debug_camera_index = static_cast<int>(type);
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
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
    // デバッグカメラの切り替え
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        auto mouse = input->GetMouse();
        if ((Mouse::BTN_MIDDLE & mouse.GetButtonDown()) && !(GetKeyState(VK_CONTROL) & 0x8000))
        {
            ChegeDebugCamera();
        }
    }


    // デバッグカメラの更新
    this->debug_camera->Update(elapsed_time);
#endif // DEBUG
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

    // カメラをメインカメラに設定
    SetCurrentCamera(CAMERA_TYPE::MAIN);
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

DirectX::BoundingFrustum CameraManager::GetBoundingFrustum()
{
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
#if 0   // メインカメラでカリングを行うか
    const auto& camera = this->GetCamera(CAMERA_TYPE::MAIN);
#else
    const auto& camera = this->GetCurrentCamera();
#endif
#else
    const auto& camera = this->GetCurrentCamera();
#endif // DEBUG

    if (camera == nullptr) return DirectX::BoundingFrustum();
    return camera->GetBoundingFrustum();
}

bool CameraManager::IsAnyMeshAABBVisible(const std::vector<DirectX::BoundingBox>& box_vec)
{
    const auto bounding_frustum = this->GetBoundingFrustum();
    for (const auto& bounding_box : box_vec)
    {
        if (bounding_frustum.Intersects(bounding_box)) return true;
    }

    return false;
}

bool CameraManager::IsMeshVisible(DirectX::BoundingBox bounding_box)
{
    const auto bounding_frustum = this->GetBoundingFrustum();
    return bounding_frustum.Intersects(bounding_box);
}

#ifdef _DEBUG

void CameraManager::DrawDebugGUI()
{
    if (ImGui::Begin("CameraManager"))
    {
        if (ImGui::Button("Reset##CameraManager"))
        {
            Reset();
        }

        // カメラ切り替え
        std::string now_name{};
        now_name = magic_enum::enum_name(static_cast<CAMERA_TYPE>(this->camera_index));
        if (ImGui::ComboUI("Camera Type##CameraManager", now_name, this->camera_name_pool, this->debug_camera_index))
        {
            if (this->debug_camera_index == static_cast<int>(CAMERA_TYPE::DEBUG))
            {
                // デバッグカメラに遷移する場合
                ChegeDebugCamera();
            }
            else
            {
                SetCurrentCamera(static_cast<CAMERA_TYPE>(this->debug_camera_index));
            }
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
#endif

#if defined(_DEBUG) || defined(RELEASE_DEBUG)
void CameraManager::ChegeDebugCamera()
{
    if (this->camera_index == static_cast<int>(CAMERA_TYPE::DEBUG))
    {
        // 元のカメラに戻す
        SetCurrentCamera(static_cast<CAMERA_TYPE>(this->temp_camera_index));
    }
    else
    {
        // デバッグカメラに切り替わる
        this->temp_camera_index = this->camera_index;
        SetCurrentCamera(CAMERA_TYPE::DEBUG);
        // カメラ情報を元のカメラと同じにする
        this->camera_pool[this->camera_index]->SetCameraParam(this->camera_pool[this->temp_camera_index]->GetCameraParam());
    }
}

#endif // _DEBUG