#include <imgui.h>//[org]
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

// �X�V����[02]
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    // �J�����̉�]���x[02]
    float speed = rollSpeed * elapsedTime;
    
    // �X�e�B�b�N���͒l�ɍ��킹��X����Y������][02]
    angle.x += ay * speed;
    angle.y += ax * speed;
    
    // X���̃J�����̉�]�𐧌�[02]
    if (angle.x > maxAngleX) angle.x = maxAngleX;
    if (angle.x < minAngleX) angle.x = minAngleX;
    
    // Y���̉�]�n��-3.14�`3.14�ɏI��ۗp�ɂ���[02]
    if (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
    if (angle.y > DirectX::XM_PI) angle.y -= DirectX::XM_2PI;

    // �J������]�l����]�s��ɕϊ�[02]
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O���x�N�g�������o��[02]
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // �����_������̃x�N�g�������Ɉ�藣�ꂽ�J�������_�����߂�[02]
    DirectX::XMFLOAT3 eye;

    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // �J�����̎��_�ƒ����_��ݒ�[02]
    Camera::Intance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

// �f�o�b�O�pGUI�`��[org]
void CameraController::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // �p�x[org]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle2", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // �^�[�Q�b�g[org]
            ImGui::InputFloat3("Target", &target.x);
            // ��]���x[org]
            float b = DirectX::XMConvertToDegrees(rollSpeed);
            ImGui::InputFloat("rollSpeed", &b);
            rollSpeed = DirectX::XMConvertToRadians(b);
            // range[org]
            ImGui::InputFloat("range", &range);
            // minAngleX[org]
            float c = DirectX::XMConvertToDegrees(minAngleX);
            ImGui::InputFloat("minAngleX", &c);
            minAngleX = DirectX::XMConvertToRadians(c);
            // maxAngleX[org]
            float d = DirectX::XMConvertToDegrees(maxAngleX);
            ImGui::InputFloat("maxAngleX", &d);
            maxAngleX = DirectX::XMConvertToRadians(d);
        };
    }
    ImGui::End();
}
