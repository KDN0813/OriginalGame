#include "GridObjectManager.h"

#include "Component\EnemyComponent.h"

#ifdef _DEBUG
#include "Debug\DebugManager.h"
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG


GridObjectManager::GridObjectManager()
:Singleton(this) 
{
    // ステージのサイズ設定(ステージは正方形
    this->STAGE_RAGE = EnemyComponent::DEFAULT_TERRITORY_RENGR;
    
    // 1つのエリアのサイズ
    this->AREA_RAGE;

    // 一列に何個のエリアがあるか
    this->CELL_MAX = static_cast<int>(this->STAGE_RAGE / this->AREA_RAGE);

    // エリア毎に格納するデータのコンテナのサイズ確保
    this->grid_data_pool.resize(this->CELL_MAX * this->CELL_MAX);

    const float HALF_STAGE_RAGE = this->STAGE_RAGE * 0.5f;
        
    // 左上の位置
    this->min_erea_pos = { HALF_STAGE_RAGE ,0.0f,HALF_STAGE_RAGE };
    // 右下の位置
    this->max_erea_pos = { -HALF_STAGE_RAGE ,0.0f,-HALF_STAGE_RAGE };
}

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

    for (int i = 0; i < this->CELL_MAX * this->CELL_MAX; ++i)
    {
        float x = static_cast<float>(i % this->CELL_MAX);
        float z = static_cast<float>(i / this->CELL_MAX);
        DirectX::XMFLOAT3 pos = { this->max_erea_pos.x + x * this->AREA_RAGE,0.0f,this->max_erea_pos.z + z * this->AREA_RAGE };
        debug_primitive_render->DrawBox(pos, {}, { this->AREA_RAGE ,this->AREA_RAGE ,this->AREA_RAGE }, { 1.0f,1.0f, 1.0f, 1.0f });
    }
}

#endif // _DEBUG