#pragma once
#include <d3d11.h>
#include <vector>
#include <memory>
#include "Shader/Shader.h"

class SpriteComponent;

class SpriteShader : public Shader
{
public:
    SpriteShader() {};
    ~SpriteShader() {};

    // 描画関数
    void Render() override;
    const char* GetName() { return "SpriteShader"; }

    // スプライトの追加
    void AddSprite(std::shared_ptr<SpriteComponent>& sprite);
private:
    std::vector<std::weak_ptr<SpriteComponent>> sprite_pool;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override;

#endif // _DEBUG
};

