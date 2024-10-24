#include <algorithm>
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // _DEBUG
#include "SpriteShader.h"
#include "Graphics/Graphics.h"

#include "Component/SpriteComponent.h"

void SpriteShader::Render()
{
    Graphics* graphics = Graphics::Instance();
    std::lock_guard<std::mutex> lock(graphics->GetInstanceMutex());
    ID3D11DeviceContext* const dc = graphics->GetDeviceContext();

    // —LŒø‚Å‚È‚¢weak_ptr‚ðœ‹Ž‚·‚é
    this->sprite_pool.erase(
        std::remove_if(this->sprite_pool.begin(), this->sprite_pool.end(),
            [](const std::weak_ptr<SpriteComponent>& weakPtr)
            {
                return weakPtr.expired();  // –³Œø‚Èweak_ptr‚©‚Ç‚¤‚©‚ðŠm”F
            }),
        this->sprite_pool.end());

    for (auto sprite_Wptr : this->sprite_pool)
    {
        auto sprite = sprite_Wptr.lock();
        if (!sprite) continue;
        if (!sprite->GetIsActive())continue;

        sprite->Render(dc);
    }
}

void SpriteShader::AddSprite(std::shared_ptr<SpriteComponent>& sprite)
{
    this->sprite_pool.emplace_back(sprite);
}

#ifdef _DEBUG

void SpriteShader::DrawDebugGUI()
{
    size_t size = this->sprite_pool.size();
    ImGui::InputSize_t("Size", size);
}

#endif // DEBUG