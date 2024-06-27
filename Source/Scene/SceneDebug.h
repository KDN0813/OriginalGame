#pragma once

#include "Scene/Scene.h"
#include <vector>
#include "Camera/CameraController.h"
#include "Debug/DebugCereal.h"
#include "Debug/DebugObject.h"

class SceneDebug : public Scene
{
public:
    SceneDebug();
    ~SceneDebug() override {};

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;

#ifdef _DEBUG
    void DrawImGui();
#endif // _DEBUG
private:
    CameraController cameraController;
    std::vector<std::unique_ptr<DebugObject>> objects;
    DebugObject stage;

    size_t obj_max;
    Microsoft::WRL::ComPtr<ID3D11Buffer> inputBuffer;  // インスタンスごとの行列を保持するバッファ 
    std::vector<DirectX::XMFLOAT4X4> inputData;
};

