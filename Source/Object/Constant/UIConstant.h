#pragma once

#include <memory>

class Object;

class UIConstant
{
public:
    static const std::shared_ptr<Object>& CreateScoreUI(const std::shared_ptr<Object>& object);
};