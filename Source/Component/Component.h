#pragma once
#include <memory>

class Object;

class Component
{
protected:
    Component() :owner(), priority(-1) {};
    virtual ~Component() {};

public:
    // 更新関数
    virtual void Update(float elapsed_time) {};

    // 名前取得
    virtual const char* GetName()const = 0;
    
    // 各取得・設定関数
    const unsigned int GetPriority()const noexcept { return this->priority; }
    void SetOwner(std::shared_ptr<Object> owner) { this->owner = owner; }
    std::shared_ptr<Object> GetOwner() { return this->owner.lock(); }

private:
    std::weak_ptr<Object>	owner;
    unsigned int priority;//優先度 初期値は最低

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    virtual void DrawDebugGUI() = 0;
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    virtual void DrawDebugPrimitive() {};

protected:
    bool is_active = false;
#endif // _DEBUG
};