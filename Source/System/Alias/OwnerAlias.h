#pragma once
// Owner�̃G�C���A�X(�ʖ�)�̒�`

#include <memory>
class Object;
using Owner = Object;
using OwnerPtr = std::shared_ptr<Owner>;
using OwnerWPtr = std::weak_ptr<Owner>;
