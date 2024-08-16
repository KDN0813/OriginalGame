#pragma once
#include <D3D11.h>
#include "Component.h"
#include <memory>

class InstancingModelShader;
class InstancingModelComponent;
class Transform3DComponent;

class InstancingModelShaderComponent : public Component
{
public:
    InstancingModelShaderComponent(InstancingModelShader* const shader);

    const char* GetName()const override { return "InstanceShaderComponent"; }

    void InstancingStart();
    void InstancingAdd();
    void InstancingEnd(ID3D11DeviceContext* dc);
private:
    InstancingModelShader* const shader;

private:
    std::weak_ptr<InstancingModelComponent> instancing_model_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()override;
#endif _DEBUG
};

