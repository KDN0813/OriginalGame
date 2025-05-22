#pragma once
#include "Component.h"
#include <vector>

class Object;
class ObjectManager;
class Transform3DComponent;

// エネミーの生成を行うコンポーネント
class EnemySpawnerComponent : public Component
{
public:
    struct Param
    {
        float create_cool_time_max; // 生成のクールタイム(最大値)
        float create_cool_time_min; // 生成のクールタイム(最低値)
        float create_cool_timer;    // 生成のクールタイム
    };

public:
    EnemySpawnerComponent(Param param);
    ~EnemySpawnerComponent() {};

    // 開始関数
    void Start()  override {};
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

    void SetObjectManager(const std::shared_ptr<ObjectManager>& manager) { this->object_manager_Wptr = manager; }
private:
    void AddCreateEnemy(const std::shared_ptr<ObjectManager>& manager);
    
private:
    std::weak_ptr<ObjectManager> object_manager_Wptr;
    Param param;
    Param default_param;

private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
    std::weak_ptr<Object> stage_floor_Wptr;
    std::weak_ptr<Object> stage_wall_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override;
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override {};
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitiveが存在するか
#endif // DEBUG
};

