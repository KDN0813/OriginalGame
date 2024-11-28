#pragma once
#include "System\ClassBase\Singleton.h"
#include <DirectXMath.h>
#include "Shader\LightParam.h"

class LightManager : public Singleton<LightManager>
{
public:
    LightManager() :Singleton(this), direction_light({ { 0.0f, -1.0f, 0.0f, 0.0f } ,{1.0f,1.0f,1.0f,1.0f} }) ,ambient_color({ 1.0f, 1.0f, 1.0f, 1.0f }){};
    ~LightManager() {};

    // ŠeŽíŽæ“¾ŠÖ”
    DirectionalLights GetLightDirection() { return this->direction_light; }
    DirectX::XMFLOAT4 GetAmbientColor() { return this->ambient_color; }
private:
    DirectionalLights direction_light;      // ƒ‰ƒCƒg‚Ì•ûŒü
    DirectX::XMFLOAT4 ambient_color;
#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};