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
}

bool PlayerComponent::InputMove(float elapsed_time)
{
    // �ړ��x�N�g�����擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // �ړ�����
    Move(moveVec.x, moveVec.z, this->move_speed);
    // ��]����
    Turn(elapsed_time,moveVec.x, moveVec.z, this->move_speed);

    return (moveVec.x != 0 || moveVec.y != 0 || moveVec.z != 0);
}

void PlayerComponent::Move(float vx, float vz, float speed)
{
    auto owner = GetOwner();

    if (auto movement = owner->GetComponent<MovementComponent>(movement_Wptr))
    {
        movement->SetVelocityX(vx);
        movement->SetVelocityZ(vz);
        movement->SetMoveSpeed(speed);
    }
}

void PlayerComponent::Turn(float elapsed_time, float vx, float vz, float speed)
{
    // �i�s�x�N�g����0�ȉ��Ȃ珈�����Ȃ�
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001) return;

    auto owner = GetOwner();
    if (auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr))
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
        float rot = 1.0 - dot;
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
    const DirectX::XMFLOAT3& camera_right = camera->GetRight();
    const DirectX::XMFLOAT3& camera_front = camera->GetFront();

    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float camera_rightX = camera_right.x;
    float camera_rightZ = camera_right.z;
    float camera_right_length = sqrtf(camera_rightX * camera_rightX + camera_rightZ * camera_rightZ);
    if (camera_right_length > 0.0f)
    {
        // �P�ʃx�N�g����
        camera_rightX /= camera_right_length;
        camera_rightZ /= camera_right_length;
    }

    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float camera_frontX = camera_front.x;
    float camera_frontZ = camera_front.z;
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
    DirectX::XMFLOAT3 vec{};
    vec.x = (camera_rightX * ax) + (camera_frontX * ay);
    vec.z = (camera_rightZ * ax) + (camera_frontZ * ay);
    //���K��
    float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
    if (length > 0.0f)
    {
        vec.x /= length;
        vec.z /= length;
    }

    // Y���ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}

#ifdef _DEBUG

void PlayerComponent::DrawDebugGUI()
{
    ImGui::InputFloat("move_speed", &this->move_speed);
    ImGui::InputFloat("turn_speed", &this->turn_speed);
}

#endif // DEBUG