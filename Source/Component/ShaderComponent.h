#pragma once
#include <D3D11.h>
#include "Component.h"
#include <memory>

class InstancingModelShader;
class InstancingModelComponent;

class InstancingModelShaderComponent : public Component
{
public:
    InstancingModelShaderComponent(InstancingModelShader* const shader);

    const char* GetName()const override { return "InstanceShaderComponent"; }

    void InstancingStart();
    void InstancingAdd();
    void InstancingEnd();
private:
    InstancingModelShader* const shader;

private:
    std::weak_ptr<InstancingModelComponent> instancing_model_Wptr;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()override;
#endif _DEBUG
};

