#pragma once
#include <memory>
class Object;
class ModelComponent;

using Owner = Object;
using OwnerPtr = std::shared_ptr<Owner>;

// カメラの個別更新挙動を定義する基底クラス
class CameraUpdateBehaviorBase
{
public:
    CameraUpdateBehaviorBase(OwnerPtr owner) : owner_Wptr(owner) {}
    virtual ~CameraUpdateBehaviorBase(){}

    // 更新関数
    virtual void Update(float elapsed_time) = 0;
    // 名前取得
    virtual const char* GetName() const = 0;

    const bool& GetIsActive() { return this->is_active; }
    void SetIsActive(bool is_active) { this->is_active = is_active; }
protected:
    std::weak_ptr<Owner> owner_Wptr;
private:
    bool is_active = true;

#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

