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

    void Initialize();
    void Finalize();
    const char* GetName()const override { return "InstanceShaderComponent"; }

    void InstancingStart();
    void InstancingAdd();
    void InstancingEnd(ID3D11DeviceContext* dc);
    /**
     * \fn IsShaderValid
     * \brief 描画に必用なパラメータを所持しているか判定。
     * 正確にはOwnerが必要なパラメータを所持しているか判定する。
     * \return 所持しているならtrue
     */
    bool IsShaderValid();

    int GetModelId();
private:
    InstancingModelShader* const shader;

private:
    std::weak_ptr<InstancingModelComponent> instancing_model_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
#endif _DEBUG
};

