#pragma once

class Scene
{
public:
    Scene(){}
    virtual ~Scene(){}

    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void Update(float elapsed_time) = 0;
    virtual void Render() = 0;
    virtual void ReStart() {};

    bool IsReady() const { return ready; }
    void SetReady() { ready = true; }
private:
    bool ready = false;

#ifdef _DEBUG
public:
    virtual void DebugDrawGUI() = 0;

#endif // DEBUG
};