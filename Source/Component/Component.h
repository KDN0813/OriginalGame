#pragma once

class GameObject;

class Component
{
protected:
    Component() :owner(nullptr), priority(-1) {};

public:
    /**
     * 更新関数
     */
    virtual void Update(float elapsedTime) {};

    // 各取得・設定関数
    const unsigned int GetPriority()const noexcept { return priority; }
    void SetOwner(GameObject* owner) { this->owner = owner; }

#ifdef _DEBUG
    /**
     * デバックの情報を2D画面に出力する関数
     */
    virtual void DrawDebugGUI() {};
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    virtual void DrawDebugPrimitive() {};
#endif // _DEBUG
private:
    GameObject* owner;
    unsigned int priority;//優先度 初期値は最低
};
