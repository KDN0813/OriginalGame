#pragma once
#include "System\ClassBase\Singleton.h"
#include <DirectXMath.h>

class LightManager : public Singleton<LightManager>
{
public:
    LightManager() :Singleton(this), light_direction({ 0.0f, -1.0f, 0.0f, 0.0f }) {};
    ~LightManager() {};

    // ŠeŽíŽæ“¾ŠÖ”
    DirectX::XMFLOAT4 GetlightDirection() { return this->light_direction; }
private:
    DirectX::XMFLOAT4 light_direction;      // ƒ‰ƒCƒg‚Ì•ûŒü
#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};

