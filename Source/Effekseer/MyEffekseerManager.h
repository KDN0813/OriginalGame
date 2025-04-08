#pragma once
#include <DirectXMath.h>

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "System\ClassBase\Singleton.h"

class MyEffekseerManager : public Singleton<MyEffekseerManager>
{
public:
    MyEffekseerManager() :Singleton(this) {}
    ~MyEffekseerManager(){}

    void Initialize();

    void Finalize();

    void Update(float elapsed_time);

    void Render();

    Effekseer::ManagerRef GetEffekseerManager() { return effekseer_manager; }

private:
    Effekseer::ManagerRef           effekseer_manager;
    EffekseerRenderer::RendererRef  effekseer_render;
};