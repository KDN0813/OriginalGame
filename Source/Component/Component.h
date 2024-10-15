#pragma once
#include <memory>

class Object;

enum COMPONENT_PRIORITY : unsigned int
{
    LOWEST = 0,         // 最低の優先度
    //VERY_LOW,           // 非常に低い優先度
    LOW,                // 低い優先度
    //BELOW_MEDIUM,       // 中程度より下の優先度
    MEDIUM,             // 中程度の優先度
    HIGH,               // 高い優先度
    VERY_HIGH,          // 非常に高い優先度
    CRITICAL,           // 最高の優先度（重要）
    DEFAULT = MEDIUM    // デフォルト優先度を中程度に設定
};

class Component
{
protected:
    Component(){};
    virtual ~Component() {};

public:
    // 開始関数
    virtual void Start() {};        // Updateが呼ばれる直前に1度だけ呼ばれる
    // 終了関数
    virtual void End() {};          // 破棄されるときに呼ばれる     
    // 更新関数
    virtual void Update(float elapsed_time) {};

    // 名前取得
    virtual const char* GetName()const = 0;

    // 優先度
    virtual const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::DEFAULT; };
    
    // 各取得・設定関数
    void SetOwner(std::shared_ptr<Object> owner) { this->owner = owner; }
    std::shared_ptr<Object> GetOwner() { return this->owner.lock(); }
    const unsigned int GetComponentID() { return this->component_id; }
    void SetComponentID(const int unique_id) { this->component_id = unique_id; }
    bool GetIsActive() { return this->is_active; }
    void SetIsActive(const bool is_active) { this->is_active = is_active; }
private:
    std::weak_ptr<Object>	owner = {};
    unsigned int component_id = {};
protected:
    bool is_active = true;

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
    /**
     * デバッグプリミティブ表示用ImGui
     */
    virtual void DrawDebugPrimitiveGUI() {};

    bool GetIsDebugPrimitive() { return this->is_draw_debug_primitive; }
    void SetIsDebugPrimitive(const bool is_draw_debug_primitive) { this->is_draw_debug_primitive = is_draw_debug_primitive; }
    virtual bool IsDebugPrimitive() { return false; }   // DebugPrimitiveが存在するか

protected:
    bool is_draw_debug_primitive = true;
#endif // _DEBUG
#ifdef RELEASE_DEBUG
public:
    // デバッグ用チート
    virtual void DebugCheat() {};
#endif // RELEASE_DEBUG

};

class DefaultComponent : public Component
{
public:
    DefaultComponent() {};
    virtual ~DefaultComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override {};

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override {};
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override {};
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() { return false; }   // DebugPrimitiveが存在するか
#endif // DEBUG
};