#pragma once
#include <DirectXMath.h>

// エフェクシア実装で使う
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "System\ClassBase\Singleton.h"

// エフェクトマネージャー[10]
class EffectManager : public Singleton<EffectManager>
{
public:
    EffectManager() :Singleton(this) {}
    ~EffectManager(){}

    // 初期化[10]
    void Initialize();

    // 終了化[10]
    void Finalize();

    // 更新処理[10]
    void Update(float elapsedTime);

    // 描画処理[10]
    void Render();

    // Effeckseerマネージャーの取得[10]
    Effekseer::ManagerRef GetEffekseerManager() { return effekseerManager; }

private:
    Effekseer::ManagerRef           effekseerManager;
    EffekseerRenderer::RendererRef  effekseerRender;
};