#include "PlayerComponent.h"
#include <imgui.h>
#include "Input/Input.h"
#include "Camera/CameraManager.h"
#include "Object/Object.h"
#include "System/GameData.h"
#include "Object\Constant\PlayerConstant.h"

#include "Component/CameraComponent.h"
#include "Component/DamageComponent.h"
#include "Component/MovementComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/CircleCollisionComponent.h"

#ifdef _DEBUG
#include "Shader\ParticleSystem.h"
#include "Debug\DebugManager.h"
#endif // DEBUG

#ifdef _DEBUG
float PlayerComponent::debug_move_speed = 100.0f;
#endif // DEBUG

PlayerComponent::PlayerComponent(PlayerParam param)
    :param(param), default_param(param) 
{
}

PlayerComponent::~PlayerComponent()
{
    // �̗͂������ł�����폜����
    if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
    {
        game_data->SetGameStatus(GameData::GameStatus::DEFEAT);
    }
}

void PlayerComponent::Update(float elapsed_time)
{
    // �ړ�����
    if(this->param.input_move_validity_flag)
    {
        InputMove(elapsed_time);
    }
}

void PlayerComponent::OnCollision(const std::shared_ptr<Object>& hit_object)
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& character = owner->GetComponent(this->character_Wptr);
    if (!character) return;

    const auto& defense_object = owner->FindChildObject(PlayerConstant::DEFENSE_OBJECT_NAME);
    if (!defense_object) return;
    const auto collision = defense_object->GetComponent(circle_collision_Wptr);
    if (!collision) return;


    switch (collision->GetCircleHitResult().collision_role)
    {
    case COLLISION_ROLE::ATTACKER:	// �Փ˂�^�������̏���
    {
    }
    case COLLISION_ROLE::RECEIVER:	// �Փ˂��󂯂����̏���
    {
        // Damage���󂯂�
        const auto& hit_object_damage = hit_object->GetComponent<DamageComponent>();
        if (hit_object_damage)
        {
            character->ApplyDamage(hit_object_damage->GetDamageAmount(), hit_object_damage->GetInvincibleTime());
        }
        break;
    }
    }
}

bool PlayerComponent::UseSpecialPoint(float use_point)
{
    // �g�p�ʂ����߂��Ă���ꍇ
    if (!IsUseSpecialPoint(use_point)) return false;

    this->param.special_point -= use_point;
    return true;;
}

bool PlayerComponent::IsUseSpecialPoint(float use_point)
{
    return (use_point <= this->param.special_point);
}

bool PlayerComponent::InputMove(float elapsed_time)
{
    // �ړ��x�N�g�����擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // �ړ�����
    Move(moveVec.x, moveVec.z, this->param.move_speed * this->param.move_rate);

    return (moveVec.x + moveVec.z);
}

void PlayerComponent::Move(float vx, float vz, float speed)
{
    auto owner = GetOwner();

    if (auto movement = owner->GetComponent<MovementComponent>(movement_Wptr))
    {
#ifdef _DEBUG
        if (Input::Instance input = Input::GetInstance(); input.Get())
        {
            //if (input->GetGamePad().GetTriggerL())
            //{
            //    speed = debug_move_speed;
            //}
        }
#endif // DEBUG

        movement->AddAccelerationXZ(vx * speed, vz * speed);
    }
}

DirectX::XMFLOAT3 PlayerComponent::GetMoveVec() const
{
    // ���͏����擾
    float ax{};
    float ay{};
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& game_pad = input->GetGamePad();
        ax = game_pad.GetAxisLX();
        ay = game_pad.GetAxisLY();
    }

    // �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    MYVECTOR3 Camera_right;
    MYVECTOR3 Camera_front;
    if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
    {
        std::shared_ptr<CameraComponent> camera = camera_manager->GetCurrentCamera();
        Camera_right = camera->GetRight();
        Camera_front = camera->GetForward();
    }

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
    float vec_x = ((camera_rightX * ax) + (camera_frontX * ay));
    float vec_y = 0.0f;  // Y���ɂ͈ړ����Ȃ�
    float vec_z = ((camera_rightZ * ax) + (camera_frontZ * ay));
    MYVECTOR3 Vec{ vec_x ,vec_y,vec_z };
    DirectX::XMFLOAT3 vec;
    Vec.Normalize().GetFlaot3(vec);
    return vec;
}

#ifdef _DEBUG

void PlayerComponent::DrawDebugGUI()
{
    ImGui::InputFloat("move_speed", &this->param.move_speed);
    ImGui::Checkbox("Input Move Validity Flag", &this->param.input_move_validity_flag);
    ImGui::InputFloat("Debug Move Speed", &this->debug_move_speed);
    ImGui::InputFloat("Move Rate", &this->param.move_rate);
    ImGui::InputFloat("Spin Attack Move Rate", &this->param.spin_attack_move_rate);

    ImGui::InputFloat("attack_end_point", &this->attack_end_point);
    ImGui::InputFloat("attack_combo2_end_point", &this->attack_combo2_end_point);

    ImGui::SliderFloat("Special Point", &this->param.special_point, 0, 100);
    ImGui::InputFloat("Special Point Max", &this->param.special_point_max);
    ImGui::InputFloat("Spin Attack Use Point", &this->param.spin_attack_use_point);
}

#endif // DEBUG

#ifdef RELEASE_DEBUG

#endif
