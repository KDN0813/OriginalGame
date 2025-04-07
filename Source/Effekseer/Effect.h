#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>

// �G�t�F�N�g[10]
class Effect
{
public:
    Effect(const char* filename);
    ~Effect() {};

    // �Đ�[10]
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f);

    // ��~[10]
    void Stop(Effekseer::Handle handle);

    // ���W�ݒ�[10]
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    // �X�P�[���ݒ�[10]
    void SetScale(Effekseer::Handle, const DirectX::XMFLOAT3& scale);

private:
    // [10]
    Effekseer::EffectRef effekseerEffect;
};

