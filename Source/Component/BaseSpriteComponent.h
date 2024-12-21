#pragma once
#include <d3d11.h>
#include "Component\Component.h"

// 2Dスプライトコンポーネントの基底クラス
class BaseSpriteComponent : public Component , public std::enable_shared_from_this<BaseSpriteComponent>
{
public:
    BaseSpriteComponent() :draw_priority(PRIORITY::DEFAULT) {};
    BaseSpriteComponent(PRIORITY priority) :draw_priority(priority) {};
public:
    // 開始関数
    virtual void Start() override;

    // 描画処理
    virtual void Render(ID3D11DeviceContext* dc) = 0;

protected:
    PRIORITY draw_priority;
};