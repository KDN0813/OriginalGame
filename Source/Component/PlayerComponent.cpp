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
    // 移動ベクトルを取得
    MYVECTOR3 moveVec = GetMoveVec();

    // 移動処理
    Move(moveVec.GetX(), moveVec.GetZ(), this->move_speed);
    // 回転処理
    Turn(elapsed_time,moveVec.GetX(), moveVec.GetZ(), this->turn_speed);

    return (moveVec.LengthXZSq());
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
    // 進行ベクトルが0以下なら処理しない
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001) return;

    auto owner = GetOwner();
    if (auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr))
    {
        speed *= elapsed_time;

        // 単位ベクトル化
        vx /= length;
        vz /= length;

        // 自身の回転値から前方向を求める
        MYVECTOR3 angle = transform->GetAngle();
        float frontX = sinf(angle.GetY());
        float frontZ = cosf(angle.GetY());

        // 回転角を求めるため、2つの単位ベクトルの内積を計算する
        float dot = (frontX * vx + frontZ * vz);

        // 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
        // 小さいほど、1.0に近づくという性質を利用して回転速度を調整する
        float rot = 1.0f - dot;
        if (rot > speed) rot = speed;

        // 左右判定を行うために2つの単位ベクトルの外積を計算する
        float cross = (frontZ * vx) - (frontX * vz);

        // 2Dの外積値が正の場合か負の場合によって左右判定が行える
        //  左右判定を行うことによって左右回転を選択する
        if (cross < 0.0f)
        {
            angle.SubtractY(rot);
        }
        else
        {
            angle.AddY(rot);
        }

        transform->SetAngle(angle.GetVector());
    }
}

MYVECTOR3 PlayerComponent::GetMoveVec() const
{
    // 入力情報を取得
    Input* input = Input::Instance();
    GamePad& game_pad = input->GetGamePad();
    float ax = game_pad.GetAxisLX();
    float ay = game_pad.GetAxisLY();

    // カメラ方向とスティックの入力値によって進行方向を計算する
    CameraManager* camera_manager = CameraManager::Instance();
    CameraComponent* camera = camera_manager->GetMainCamera();
    MYVECTOR3 camera_right = camera->GetRight();
    MYVECTOR3 camera_front = camera->GetFront();

    // 移動ベクトルはXZ平面に水平なベクトルになるようにする

    // カメラ右方向ベクトルをXZ単位ベクトルに変換
    float camera_rightX = camera_right.GetX();
    float camera_rightZ = camera_right.GetZ();
    float camera_right_length = sqrtf(camera_rightX * camera_rightX + camera_rightZ * camera_rightZ);
    if (camera_right_length > 0.0f)
    {
        // 単位ベクトル化
        camera_rightX /= camera_right_length;
        camera_rightZ /= camera_right_length;
    }

    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    float camera_frontX = camera_front.GetX();
    float camera_frontZ = camera_front.GetZ();
    float camera_front_Length = sqrtf(camera_frontX * camera_frontX + camera_frontZ * camera_frontZ);
    if (camera_front_Length > 0.0f)
    {
        // 単位ベクトル化
        camera_frontX /= camera_front_Length;
        camera_frontZ /= camera_front_Length;
    }

    // スティックの水平入力値をカメラ右方向に反映し、
    // スティックの垂直入力値をカメラ前方向に反映し、
    // 進行ベクトルを計算する
    float vec_x = (camera_rightX * ax) + (camera_frontX * ay);
    float vec_y = 0.0f;  // Y軸には移動しない
    float vec_z = (camera_rightZ * ax) + (camera_frontZ * ay);
    MYVECTOR3 vec{ vec_x ,vec_y,vec_z };
    //正規化
    float length = vec.LengthXZ();
    if (length > 0.0f)
    {
        vec.DivideXZ(length, length);
    }
    return vec;
}

#ifdef _DEBUG

void PlayerComponent::DrawDebugGUI()
{
    ImGui::InputFloat("move_speed", &this->move_speed);
    ImGui::InputFloat("turn_speed", &this->turn_speed);
}

#endif // DEBUG