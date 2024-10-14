#include "PlayerComponent.h"
#include <imgui.h>
#include "Input/Input.h"
#include "Camera/CameraManager.h"
#include "Object/Object.h"

#include "Component/CameraComponent.h"
#include "Component/MovementComponent.h"
#include "Component/TransformComponent.h"

void PlayerComponent::Update(float elapsed_time)
{
    InputMove(elapsed_time);

#ifdef _DEBUG
    if (GetAsyncKeyState(VK_F2) & 0x8000)
    {
        auto owner = GetOwner();
        if (auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr))
        {
            // ���_�ɖ߂�
            transform->SetPosition(DirectX::XMFLOAT3());
        }
    }

#endif // DEBUG
}

bool PlayerComponent::InputMove(float elapsed_time)
{
    // �ړ��x�N�g�����擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // �ړ�����
    Move(moveVec.x, moveVec.z, this->move_speed);
    // ��]����
    Turn(elapsed_time,moveVec.x, moveVec.z, this->turn_speed);

    return (moveVec.x + moveVec.z);
}

void PlayerComponent::Move(float vx, float vz, float speed)
{
    auto owner = GetOwner();

    if (auto movement = owner->EnsureComponentValid<MovementComponent>(movement_Wptr))
    {
        movement->AddAccelerationXZ(vx * speed, vz * speed);
    }
}

void PlayerComponent::Turn(float elapsed_time, float vx, float vz, float speed)
{
    // �i�s�x�N�g����0�ȉ��Ȃ珈�����Ȃ�
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001) return;

    auto owner = GetOwner();
    if (auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr))
    {
        speed *= elapsed_time;

        // �P�ʃx�N�g����
        vx /= length;
        vz /= length;

        // ���g�̉�]�l����O���������߂�
        DirectX::XMFLOAT3 angle = transform->GetAngle();
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);

        // ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
        float dot = (frontX * vx + frontZ * vz);

        // ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
        // �������قǁA1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
        float rot = 1.0f - dot;
        if (rot > speed) rot = speed;

        // ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
        float cross = (frontZ * vx) - (frontX * vz);

        // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
        //  ���E������s�����Ƃɂ���č��E��]��I������
        if (cross < 0.0f)
        {
            angle.y -= rot;
        }
        else
        {
            angle.y += rot;
        }

        transform->SetAngle(angle);
    }
}

DirectX::XMFLOAT3 PlayerComponent::GetMoveVec() const
{
    // ���͏����擾
    Input* input = Input::Instance();
    GamePad& game_pad = input->GetGamePad();
    float ax = game_pad.GetAxisLX();
    float ay = game_pad.GetAxisLY();

    // �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    CameraManager* camera_manager = CameraManager::Instance();
    CameraComponent* camera = camera_manager->GetMainCamera();
    MYVECTOR3 Camera_right = camera->GetRight();
    MYVECTOR3 Camera_front = camera->GetForward();

    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float camera_rightX = Camera_right.GetX();
    float camera_rightZ = Camera_right.GetZ();
    float camera_right_length = sqrtf(camera_rightX * camera_rightX + camera_rightZ * camera_rightZ);
    if (camera_right_length > 0.0f)
    {
        // �P�ʃx�N�g����
        camera_rightX /= camera_right_length;
        camera_rightZ /= camera_right_length;
    }

    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float camera_frontX = Camera_front.GetX();
    float camera_frontZ = Camera_front.GetZ();
    float camera_front_Length = sqrtf(camera_frontX * camera_frontX + camera_frontZ * camera_frontZ);
    if (camera_front_Length > 0.0f)
    {
        // �P�ʃx�N�g����
        camera_frontX /= camera_front_Length;
        camera_frontZ /= camera_front_Length;
    }

    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    // �i�s�x�N�g�����v�Z����
    float vec_x = (camera_rightX * ax) + (camera_frontX * ay);
    float vec_y = 0.0f;  // Y���ɂ͈ړ����Ȃ�
    float vec_z = (camera_rightZ * ax) + (camera_frontZ * ay);
    MYVECTOR3 Vec{ vec_x ,vec_y,vec_z };
    DirectX::XMFLOAT3 vec;
    Vec.Normalize().GetFlaot3(vec);
    return vec;
}

#ifdef _DEBUG

void PlayerComponent::DrawDebugGUI()
{
    ImGui::InputFloat("move_speed", &this->move_speed);
    ImGui::InputFloat("turn_speed", &this->turn_speed);
}

#endif // DEBUG