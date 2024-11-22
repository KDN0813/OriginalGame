#pragma once
#include "System\ClassBase\Singleton.h"
#include <DirectXMath.h>

class LightManager : public Singleton<LightManager>
{
public:
    LightManager() :Singleton(this), light_direction({ 0.0f, -1.0f, 0.0f, 0.0f }) {};
    ~LightManager() {};

    // �e��擾�֐�
    DirectX::XMFLOAT4 GetlightDirection() { return this->light_direction; }
private:
    DirectX::XMFLOAT4 light_direction;      // ���C�g�̕���
#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};

