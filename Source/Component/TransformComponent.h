#pragma once
#include "Component.h"
#include "System/MyMath/MYMATRIX.h"

class Transform3DComponent : public Component
{
public:
    Transform3DComponent() {};
    ~Transform3DComponent() {};

    void Update(float elapsed_time)override;
    const char* GetName()const override { return "TransformComponent"; }
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::MEDIUM; };

    MYMATRIX GetTransform() { return this->transform; }
    DirectX::XMFLOAT4X4 GetTransformF() { return this->transform.GetFlaot4x4(); }
    void SetTransform(MYMATRIX transform) { this->transform = transform; }

#pragma region set・get parame
    MYVECTOR3 GetPosition()const noexcept { return this->position; }
    void SetPosition(MYVECTOR3 pos)noexcept { this->change_value = true; this->position = pos; }
    MYVECTOR3 AddPosition(MYVECTOR3 vec);

    MYVECTOR3 GetAngle()const noexcept { return this->angle; }
    void SetAngle(MYVECTOR3 angle)noexcept { this->change_value = true; this->angle = angle; }
    
    MYVECTOR3 GetScale()const noexcept { return this->scale; }
    void SetScale(MYVECTOR3 scale)noexcept { this->change_value = true; this->scale = scale; }
    
#pragma endregion set parame
private:
    MYMATRIX transform{};
    MYVECTOR3 position{};
    MYVECTOR3 angle{};
    MYVECTOR3 scale{};
    bool change_value = false;
#ifdef _DEBUG
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()override;
#endif // _DEBUG
};