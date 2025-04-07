#pragma once
#include <DirectXMath.h>

// �G�t�F�N�V�A�����Ŏg��
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "System\ClassBase\Singleton.h"

// �G�t�F�N�g�}�l�[�W���[[10]
class EffectManager : public Singleton<EffectManager>
{
public:
    EffectManager() :Singleton(this) {}
    ~EffectManager(){}

    // ������[10]
    void Initialize();

    // �I����[10]
    void Finalize();

    // �X�V����[10]
    void Update(float elapsedTime);

    // �`�揈��[10]
    void Render();

    // Effeckseer�}�l�[�W���[�̎擾[10]
    Effekseer::ManagerRef GetEffekseerManager() { return effekseerManager; }

private:
    Effekseer::ManagerRef           effekseerManager;
    EffekseerRenderer::RendererRef  effekseerRender;
};