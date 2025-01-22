#include "GridObjectManager.h"

#include "System\MyMath\MYVECTOR3.h"
#include "Object\Object.h"

#include "Component\EnemyComponent.h"
#include "Component\TransformComponent.h"

#ifdef _DEBUG
#include "Debug\DebugManager.h"
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG


GridObjectManager::GridObjectManager()
:Singleton(this) 
{
    // �X�e�[�W�̃T�C�Y�ݒ�(�X�e�[�W�͐����`
    this->STAGE_RAGE = EnemyComponent::DEFAULT_TERRITORY_RENGR;
    
    // 1�̃G���A�̃T�C�Y
    this->AREA_RAGE;

    // ���ɉ��̃G���A�����邩
    this->CELL_MAX = static_cast<int>(this->STAGE_RAGE / this->AREA_RAGE);

    // �G���A���Ɋi�[����f�[�^�̃R���e�i�̃T�C�Y�m��
    this->grid_data_pool.resize(this->CELL_MAX * this->CELL_MAX);

    const float HALF_STAGE_RAGE = this->STAGE_RAGE * 0.5f;
        
    float OFFSET = this->HALF_AREA_RAGE;  // ���L�̃R�[�h��HALF_STAGE_RAGE�������ƒ��S�̈ʒu�ɂȂ�̂�
    OFFSET = 0.0f;  // ���L�̃R�[�h��HALF_STAGE_RAGE�������ƒ��S�̈ʒu�ɂȂ�̂�
    // ����̈ʒu
    this->min_erea_pos = { -HALF_STAGE_RAGE - OFFSET ,0.0f,-HALF_STAGE_RAGE - OFFSET };
    // �E���̈ʒu
    this->max_erea_pos = { +HALF_STAGE_RAGE + OFFSET ,0.0f,+HALF_STAGE_RAGE + OFFSET };
}

bool GridObjectManager::Check(std::shared_ptr<Object> object, std::shared_ptr<Transform3DComponent> transform)
{
    if (!object || !transform) return false;

    const MYVECTOR3 Min = this->min_erea_pos;   // �E����W
    const MYVECTOR3 Max = this->max_erea_pos;   // �������W
    const MYVECTOR3 WorldPos = MYVECTOR3(transform->GetWorldPosition()); 
    const MYVECTOR3 Pos = WorldPos - Min;   // �����0.0�Ƃ����ʒu�ɕύX����

    DirectX::XMFLOAT3 pos{};
    Pos.GetFlaot3(pos);

    if (pos.x < 0.0f || pos.z < 0.0f) return false;

    int x = static_cast<int>(pos.x) / static_cast<int>(this->AREA_RAGE);
    int z = static_cast<int>(pos.z) / static_cast<int>(this->AREA_RAGE);

    int index = z * CELL_MAX + x;

    if (this->grid_data_pool.size() < index) return false;

    this->grid_data_pool[index].object = object;
    return true;
}
#ifdef _DEBUG

void GridObjectManager::DrawDebugGUI()
{
    if(ImGui::Begin("GridObjectManager"))
    {
        ImGui::InputFloat3("min_erea_pos", &min_erea_pos.x);
    }
    ImGui::End();
}

void GridObjectManager::DrawDebugPrimitive()
{
    DebugManager::Instance debug_manager = DebugManager::GetInstance();
    const auto& debug_primitive_render = debug_manager->GetDebugPrimitiveRenderer();
    if (!debug_primitive_render) return;

    //for (int i = 0; i < 2; ++i)
    for (int i = 0; i < this->CELL_MAX * this->CELL_MAX; ++i)
    {
        float x = static_cast<float>(i % this->CELL_MAX);
        float z = static_cast<float>(i / this->CELL_MAX);
        DirectX::XMFLOAT3 pos = { this->min_erea_pos.x + this->HALF_AREA_RAGE + x * this->AREA_RAGE,0.0f,this->min_erea_pos.z + this->HALF_AREA_RAGE  + z * this->AREA_RAGE };
        
        // �G���A���ɃI�u�W�F�N�g�����݂��邩
        bool is_object = (this->grid_data_pool[i].object.lock() != nullptr);
        DirectX::XMFLOAT4 color = is_object ? DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) : DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        debug_primitive_render->DrawBox(pos, {}, { this->HALF_AREA_RAGE ,this->HALF_AREA_RAGE ,this->HALF_AREA_RAGE }, color);
    }
}

#endif // _DEBUG