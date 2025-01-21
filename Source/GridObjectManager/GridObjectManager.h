#pragma once

#include "System\ClassBase\Singleton.h"

class GridObjectManager : public Singleton<GridObjectManager>
{
public:
    static constexpr float AREA_RAGE = 5.0f;    // エリアサイズ
public:
    GridObjectManager() :Singleton(this) {}
    ~GridObjectManager() {};

private:

#ifdef _DEBUG
public:
    void DrawDebugGUI();
    void DrawDebugPrimitive();

#endif // _DEBUG
};
