#pragma once
#include "System/ClassBase/Singleton.h"
#include "Object/Object.h"

class Transform3DComponent;

// ゲーム内の重要なオブジェクトを登録・参照するためのクラス
class GameObjectRegistry : public Singleton<GameObjectRegistry>
{
public:
    GameObjectRegistry();
    ~GameObjectRegistry() {};

    // 更新処理
    // 主に削除されたエネミーをリストから消す処理
    void Update();

    // 各種設定・取得関数
    void SetPlayer(std::shared_ptr<Object> player) { this->player_Wptr = player; };
    void SetStageFloor(std::shared_ptr<Object> stage) { this->stage_floor_Wptr = stage; };
    void SetStageWall(std::shared_ptr<Object> stage) { this->stage_wall_Wptr = stage; };
    void SetEnemy(std::shared_ptr<Object> enemy) { this->enemy_Wptr_pool.emplace_back(enemy); };
    std::shared_ptr<Object> GetPlayer() const { return this->player_Wptr.lock(); }
    std::shared_ptr<Object> GetStageFloor() const { return this->stage_floor_Wptr.lock(); }
    std::shared_ptr<Object> GetStageWall() const { return this->stage_wall_Wptr.lock(); }
    const std::vector<std::weak_ptr<Object>>& GetEnemyWptPool() const { return this->enemy_Wptr_pool; };
    
    // エネミーの全削除
    void EnemyAllClear();
    // 追加で敵を生成する
    void AddCreateEnemy(float elapsed_time,ObjectManager& object_manager);
    void AddCreateEnemy(ObjectManager& object_manager);
private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
    std::weak_ptr<Object> stage_floor_Wptr;
    std::weak_ptr<Object> stage_wall_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

    float create_enemy_cool_time = 0.0f;    // 敵生成のクールタイム
#ifdef _DEBUG
    bool is_create_enemy = false;           // 敵を生成するフラグ
#else
    bool is_create_enemy = true;            // 敵を生成するフラグ
#endif // _DEBUG

#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};