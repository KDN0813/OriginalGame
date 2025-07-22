#include "BaseSpriteComponent.h"

#include "Shader\SpriteShader.h"

void BaseSpriteComponent::Start()
{
    SpriteShader::Instance manager = SpriteShader::GetInstance();
    manager->AddSprite(shared_from_this());
}

DirectX::XMFLOAT2 BaseSpriteComponent::CalcModifiedPos(const DirectX::XMFLOAT2 display_pos)
{
    return 
    {
        display_pos.x + this->sprite_param.offset.x,
        display_pos.y + this->sprite_param.offset.y
    };
}
