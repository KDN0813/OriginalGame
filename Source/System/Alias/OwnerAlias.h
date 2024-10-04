#pragma once
// Ownerのエイリアス(別名)の定義

#include <memory>
class Object;
using Owner = Object;
using OwnerPtr = std::shared_ptr<Owner>;
using OwnerWPtr = std::weak_ptr<Owner>;
