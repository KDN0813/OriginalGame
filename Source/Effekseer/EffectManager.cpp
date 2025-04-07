#include "Graphics/Graphics.h"
#include "EffectManager.h"
#include "Camera\CameraManager.h"

#include "Component\CameraComponent.h"

// ������[10]
void EffectManager::Initialize()
{
    // (��)�O���t�B�b�N�X�N���X[�����炭�`��G���W���ō���Ă���]
    // (��)�`�悷�邽�߂̃f�[�^�l�܂��Ă�
    Graphics::Instance graphics = Graphics::GetInstance();

    // Effekserr�����_������[10]
    effekseerRender = EffekseerRendererDX11::Renderer::Create(graphics->GetDevice(),
        graphics->GetDeviceContext(), 2048);

    // Effekseer�}�l�[�W���[����[10]
    effekseerManager = Effekseer::Manager::Create(2048);    // �����͍ő�C���X�^���X��

    // (��)��Updf�ʂ�ɏ���

    // Effekseer�����_���̊e���ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͒�^���Ɉȉ��̐ݒ��OK)
    effekseerManager->SetSpriteRenderer(effekseerRender->CreateSpriteRenderer());
    effekseerManager->SetRibbonRenderer(effekseerRender->CreateRibbonRenderer());
    effekseerManager->SetRingRenderer(effekseerRender->CreateRingRenderer());
    effekseerManager->SetTrackRenderer(effekseerRender->CreateTrackRenderer());
    effekseerManager->SetModelRenderer(effekseerRender->CreateModelRenderer());
    // �G�t�F�N�g���ł̃��[�_�[�̐ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͒�^���Ɉȉ��̐ݒ��OK)
    effekseerManager->SetTextureLoader(effekseerRender->CreateTextureLoader());
    effekseerManager->SetModelLoader(effekseerRender->CreateModelLoader());
    effekseerManager->SetMaterialLoader(effekseerRender->CreateMaterialLoader());

    // Effekseeer�����č��W�n�Ōv�Z����
    effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

// �I����[10]
void EffectManager::Finalize()
{
    // EffekseerManager�Ȃǂ̓X�}�[�g�|�C���^�ɂ���Ĕj�������̂ŉ������Ȃ�
}

// �X�V����[10]
void EffectManager::Update(float elapsedTime)
{
    // �G�t�F�N�g�X�V����(�����ɂ�frame�̌o�ߎ��Ԃ�n��)
    effekseerManager->Update(elapsedTime * 60.0f);
}

// �`�揈��[10]
void EffectManager::Render()
{
    DirectX::XMFLOAT4X4 view{};
    DirectX::XMFLOAT4X4 projection{};
    {
        const auto& camera = CameraManager::GetInstance()->GetCamera(CAMERA_TYPE::MAIN);
        view = camera->GetViewTransform();
        projection = camera->GetProjectionTransform();
    }

    // �r���[&�v���W�F�N�V�����s���Effekseer�����_���ɐݒ�
    // reinterpret_cast:�|�C���^�[�𑼂̃|�C���^�[�^�ɕϊ�����
    effekseerRender->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRender->SetProjectionMatrix(*reinterpret_cast<const
        Effekseer::Matrix44*>(&projection));

    // Effekseer�`��J�n
    effekseerRender->BeginRendering();

    // Effekseer�`����s
    // �}�l�[�W���[�P�ʂŕ`�悷��̂ŕ`�揇�Ԃ𐧌䂷��ꍇ�̓}�l�[�W���[�𕡐��쐬���A
    // Draw()�֐������s���鏇���Ő���ł�����
    effekseerManager->Draw();

    // Effekseer�`��I��
    effekseerRender->EndRendering();
}
