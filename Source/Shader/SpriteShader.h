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

    // �`��֐�
    void Render() override;
    const char* GetName() { return "SpriteShader"; }

    // �X�v���C�g�̒ǉ�
    void AddSprite(std::shared_ptr<SpriteComponent>& sprite);
private:
    std::vector<std::weak_ptr<SpriteComponent>> sprite_pool;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override;

#endif // _DEBUG
};

