#pragma once
#include "System/ClassBase/Singleton.h"
#include "Object/Object.h"

class GameObject : public Singleton<GameObject>
{
public:
    GameObject();
    ~GameObject() {};

    // 更新処理
    // 主に削除されたエネミーをリストから消す処理
    void Update();

    // 各種設定・取得関数
    void SetPlayer(std::shared_ptr<Object> player) { this->player_Wptr = player; };
    void SetStageFoor(std::shared_ptr<Object> stage) { this->stage_foor_Wptr = stage; };
    void SetStageWall(std::shared_ptr<Object> stage) { this->stage_wall_Wptr = stage; };
    void SetEnemy(std::shared_ptr<Object> enenmy) { this->enemy_Wptr_pool.emplace_back(enenmy); };
    std::shared_ptr<Object> GetPlayer() const { return this->player_Wptr.lock(); }
    std::shared_ptr<Object> GetStageFoor() const { return this->stage_foor_Wptr.lock(); }
    std::shared_ptr<Object> GetStageWall() const { return this->stage_wall_Wptr.lock(); }
    const std::vector<std::weak_ptr<Object>>& GetEnemyWptPool() const { return this->enemy_Wptr_pool; };
private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Object> stage_foor_Wptr;
    std::weak_ptr<Object> stage_wall_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};