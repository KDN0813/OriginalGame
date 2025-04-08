#pragma once
#include <DirectXMath.h>

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "System\ClassBase\Singleton.h"

class EffekseerSystem : public Singleton<EffekseerSystem>
{
public:
    class EffetResourceManager
    {
    public:
        EffetResourceManager() {}
        ~EffetResourceManager() {}

        // モデルリソース読み込み
        Effekseer::EffectRef LoadEffectResource( const char* filename);

    private:
        using EffectMap = std::map<std::string, Effekseer::EffectRef>;

        EffectMap effects;
    };

public:
    EffekseerSystem() :Singleton(this) {}
    ~EffekseerSystem(){}

    void Initialize();
    void Finalize();
    void Update(float elapsed_time);
    void Render();

    EffetResourceManager& GetResourceManager() { return this->resource_manager; }
    Effekseer::ManagerRef GetEffekseerManager() { return effekseer_manager; }

    // エフェクト再生
    void PlayEffect(const char* filename, const DirectX::XMFLOAT3& position, float scale = 1.0f, DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f });
    void AllStopEffect();
    void StopEffect(Effekseer::Handle handle);
    void SetPositionEffect(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);
    void SetScaleEffect(Effekseer::Handle, const DirectX::XMFLOAT3& scale);
private:
    Effekseer::ManagerRef           effekseer_manager;
    EffekseerRenderer::RendererRef  effekseer_render;

    EffetResourceManager            resource_manager;
};