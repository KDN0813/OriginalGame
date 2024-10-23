#pragma once
#include "System/ClassBase/Singleton.h"

class Audio : public Singleton<Audio>
{
public:
    Audio();
    ~Audio() {};

private:

#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};

