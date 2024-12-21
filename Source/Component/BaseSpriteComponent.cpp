#include "BaseSpriteComponent.h"

#include "Shader\SpriteShader.h"

void BaseSpriteComponent::Start()
{
    if (SpriteShader::Instance manager = SpriteShader::GetInstance(); manager.Get())
    {
        manager->AddSprite(shared_from_this());
    }
}
