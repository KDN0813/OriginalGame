#pragma once
#include <d3d11.h>
#include "Component\Component.h"

// 2Dスプライトコンポーネントの基底クラス
class BaseSpriteComponent : public Component , public std::enable_shared_from_this<BaseSpriteComponent>
{
public:
    // 開始関数
    virtual void Start() override;

    // 描画処理
    virtual void Render(ID3D11DeviceContext* dc) = 0;
};