#include "GridObjectManager.h"

#include "Component\EnemyComponent.h"

#ifdef _DEBUG
#include "Debug\DebugManager.h"
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG


#ifdef _DEBUG

void GridObjectManager::DrawDebugGUI()
{
    if(ImGui::Begin("GridObjectManager"))
    {

    }
    ImGui::End();
}

void GridObjectManager::DrawDebugPrimitive()
{
    DebugManager::Instance debug_manager = DebugManager::GetInstance();
    const auto& debug_primitive_render = debug_manager->GetDebugPrimitiveRenderer();
    if (!debug_primitive_render) return;

    for (float x = EnemyComponent::DEFAULT_TERRITORY_RENGR; x > -EnemyComponent::DEFAULT_TERRITORY_RENGR; x -= AREA_RAGE)
    {
        for (float z = EnemyComponent::DEFAULT_TERRITORY_RENGR; z > -EnemyComponent::DEFAULT_TERRITORY_RENGR; z -= AREA_RAGE)
        {
            debug_primitive_render->DrawBox({ x,-2.0f,z }, {}, { AREA_RAGE ,AREA_RAGE ,AREA_RAGE }, { 1.0f,1.0f, 1.0f, 1.0f });
        }
    }
}

#endif // _DEBUG