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
    // �J�����p�����[�^�쐬
    CameraComponent::CameraParam camera_param{};
    camera_param.fovY = DirectX::XMConvertToRadians(45.0f);
    // �O���t�B�b�N�X����A�X�y�N�g����v�Z
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
        // �J�����쐬
        this->camera_pool[i] = std::make_shared<CameraComponent>(camera_param);
        this->camera_pool[i]->SetCameraType(static_cast<CAMERA_TYPE>(i));   // �J�����^�C�v�ݒ�
        this->camera_pool[i]->SetIsActive(false);                           // �J�������A�N�e�B�u�ɐݒ�
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
        // �f�o�b�O�p�̃J�����̖��O��ݒ�
        this->camera_name_pool.emplace_back(magic_enum::enum_name(static_cast<CAMERA_TYPE>(i)));
#endif // DEBUG
    }
    // ���C���J�����ݒ�
    this->current_camera = this->camera_pool[static_cast<size_t>(CAMERA_TYPE::MAIN)];
    this->current_camera->SetIsActive(true);    // �J�������A�N�e�B�u�ɂ���
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
    // �f�o�b�O�p�̃J�����C���f�b�N�X�ݒ�
    this->camera_index = static_cast<int>(CAMERA_TYPE::MAIN);
    this->debug_camera_index = static_cast<int>(CAMERA_TYPE::MAIN);

    // �f�o�b�O�J�����쐬
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
    // �f�o�b�O�J�����̐؂�ւ�
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        auto mouse = input->GetMouse();
        if ((Mouse::BTN_MIDDLE & mouse.GetButtonDown()) && !(GetKeyState(VK_CONTROL) & 0x8000))
        {
            ChegeDebugCamera();
        }
    }


    // �f�o�b�O�J�����̍X�V
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

    // �J���������C���J�����ɐݒ�
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
        // ���C���J��������̏ꍇ
        return MYMATRIX();
    }

    MYMATRIX View = current_camera->GetViewTransform();
    MYMATRIX Projection = current_camera->GetProjectionTransform();
    return View * Projection;
}

DirectX::BoundingFrustum CameraManager::GetBoundingFrustum()
{
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
#if 0   // ���C���J�����ŃJ�����O���s����
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

        // �J�����؂�ւ�
        std::string now_name{};
        now_name = magic_enum::enum_name(static_cast<CAMERA_TYPE>(this->camera_index));
        if (ImGui::ComboUI("Camera Type##CameraManager", now_name, this->camera_name_pool, this->debug_camera_index))
        {
            if (this->debug_camera_index == static_cast<int>(CAMERA_TYPE::DEBUG))
            {
                // �f�o�b�O�J�����ɑJ�ڂ���ꍇ
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
        // ���̃J�����ɖ߂�
        SetCurrentCamera(static_cast<CAMERA_TYPE>(this->temp_camera_index));
    }
    else
    {
        // �f�o�b�O�J�����ɐ؂�ւ��
        this->temp_camera_index = this->camera_index;
        SetCurrentCamera(CAMERA_TYPE::DEBUG);
        // �J�����������̃J�����Ɠ����ɂ���
        this->camera_pool[this->camera_index]->SetCameraParam(this->camera_pool[this->temp_camera_index]->GetCameraParam());
    }
}

#endif // _DEBUG