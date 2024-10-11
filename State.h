#pragma once

class StateBase
{
public:
    StateBase(){}
    virtual ~StateBase() {};

    virtual void Start() {};

    virtual void Update(float elapsed_time) = 0;

    virtual const char* Name() = 0;
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};
