#pragma once
#include "System/ClassBase/Singleton.h"
#include "Object/Object.h"

class GameObject : public Singleton<GameObject>
{
public:
    GameObject();
    ~GameObject() {};

    // Šeíİ’èEæ“¾ŠÖ”
    void SetPlayer(std::shared_ptr<Object> player) { this->player_Wptr = player; };
    void SetStage(std::shared_ptr<Object> stage) { this->stage_Wptr = stage; };
    void SetEnemy(std::shared_ptr<Object> enenmy) { this->enemy_Wptr_pool.emplace_back(enenmy); };
    std::shared_ptr<Object> GetPlayer() { return this->player_Wptr.lock(); }
    std::shared_ptr<Object> GetStage() { return this->stage_Wptr.lock(); }
    std::vector<std::shared_ptr<Object>> GetEnemyPool();
private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Object> stage_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};