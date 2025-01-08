#pragma once

#include <memory>

class Object;

class UIConstant
{
public:
    static const std::shared_ptr<Object>& CreateScoreUI(const std::shared_ptr<Object>& object);
    static const std::shared_ptr<Object>& CreateGameTimerUI(const std::shared_ptr<Object>& object);
    static const std::shared_ptr<Object>& CreatePause(const std::shared_ptr<Object>& object);
    static const std::shared_ptr<Object>& CreateFadeObject(const std::shared_ptr<Object>& object);
};