#pragma once
#include <memory>
class Object;
class CameraComponent;

using Owner = Object;
using OwnerPtr = std::shared_ptr<Owner>;

// カメラコントローラーの基底クラス
class CameraControllerBase
{
public:
    CameraControllerBase(OwnerPtr owner) : owner_Wptr(owner) {}
    virtual ~CameraControllerBase(){}

    // 更新関数
    virtual void Update(float elapsed_time) = 0;
    // 名前取得
    virtual const char* GetName() const = 0;

    std::shared_ptr<CameraComponent> GetCamera();

    const bool& GetIsActive() { return this->is_active; }
    void SetIsActive(bool is_active) { this->is_active = is_active; }
protected:
    std::weak_ptr<Owner> owner_Wptr;
    std::weak_ptr<CameraComponent> camera_Wptr;
private:
    bool is_active = true;

#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() = 0;
#endif // _DEBUG
};

