#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <vector>
#ifdef _DEBUG
#include "Debug\DebugPrimitiveRenderer.h"
#endif // _DEBUG

class Object;
class ObjectManager;
class Transform3DComponent;

struct EnemyConfig;

// エネミーの生成を行うコンポーネント
class EnemySpawnerComponent : public Component
{
public:
    enum EyemyType
    {
        Normal = 0,
        Max,
    };

    struct Param
    {
        float create_cool_time_max; // 生成のクールタイム(最大値)
        float create_cool_time_min; // 生成のクールタイム(最低値)
        float create_cool_timer;    // 生成のクールタイム

        float min_spawn_dist;   // 生成範囲の最小
        float max_spawn_dist;   // 生成範囲の最小

        int min_enemies_per_spawn;  // 一度に生成する敵の最低値
        int max_enemies_per_spawn;  // 一度に生成する敵の最大値

        float spawn_offset_interval = 2.0f; // 各スポーン地点のばらけ幅
    };

public:
    EnemySpawnerComponent(Param param);
    ~EnemySpawnerComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override { this->param = this->default_param; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "EnemySpawner"; };
    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void SetChainScoreCounterObject(const std::shared_ptr<Object>& object) { this->chain_score_counter_object_Wptr = object; };
    void SetObjectManager(const std::shared_ptr<ObjectManager>& manager) { this->object_manager_Wptr = manager; }
private:
    void UpdateEnemySpawner(const std::shared_ptr<ObjectManager>& manager);
    void CreateEnemy(const std::shared_ptr<ObjectManager>& manager, const EnemyConfig& config);
    
private:
    std::weak_ptr<ObjectManager> object_manager_Wptr;
    Param param;
    Param default_param;

    std::vector<EnemyConfig> enemy_config_pool;

private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
    std::weak_ptr<Object> stage_floor_Wptr;
    std::weak_ptr<Object> stage_wall_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

    std::weak_ptr<Object> chain_score_counter_object_Wptr;
#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override;
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override;
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override;
    bool IsDebugPrimitive() override { return true; }   // DebugPrimitiveが存在するか
private:
    CylinderParam min_spawn_area;
    CylinderParam max_spawn_area;
    CylinderParam spawn_area;
    int debug_spawn_count = 0;      // ImGuiで敵を生成する時の数
#endif // DEBUG
};

