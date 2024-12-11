#include "Shader.h"
#include "Shader\TextNumberShader.h"

void TextNumberShader::Render()
{
}

void TextNumberShader::AddSprite(std::shared_ptr<TextNumberComponent>& sprite)
{
    this->sprite_pool.push_back(sprite);
}

#ifdef _DEBUG

void TextNumberShader::DrawDebugGUI()
{
}

#endif // DEBUG