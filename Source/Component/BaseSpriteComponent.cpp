#include "BaseSpriteComponent.h"

#include "Shader\SpriteShader.h"

void BaseSpriteComponent::Start()
{
    SpriteShader::Instance manager = SpriteShader::GetInstance();
    manager->AddSprite(shared_from_this());
}