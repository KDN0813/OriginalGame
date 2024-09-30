#pragma once

// ステート遷移判定
class JudgmentBase
{
public:
    JudgmentBase() {};
    virtual ~JudgmentBase() {};

private:
    virtual void Judgment() = 0;
};
