#pragma once
#include <memory>
#include "Object/Object.h"

using Owner = Object;
using OwnerPtr = std::shared_ptr<Owner>;
using OwnerWPtr = std::weak_ptr<Owner>;

// Onwer�N���X
class OnwerWptr
{
public:
    OnwerWptr(OwnerPtr owner) :owner_Wptr(owner) {};
    OnwerWptr(const OnwerWptr&) = delete;
    OnwerWptr& operator=(const OnwerWptr&) = delete;
    OnwerWptr(const OnwerWptr&&) = delete;
    OnwerWptr& operator=(const OnwerWptr&&) = delete;
    ~OnwerWptr() {};

    /**
     * @fn GetComponent
     * @brief �R���|�[�l���g���擾����
     * @tparam ComponentType �擾�R���|�[�l���g�̌^
     *
     * \return �R���|�[�l���g�̃V�F�A�h�|�C���^��Ԃ�
     */
    template<is_Component ComponentType>
    std::shared_ptr<ComponentType> GetComponent()
    {
        auto owner = this->owner_Wptr.lock();
        if (!owner) return nullptr;
        return owner->GetComponent<ComponentType>();
    }

    /**
     * @fn EnsureComponentValid
     * @brief �R���|�[�l���g���L���ł��邩�m�F����
     * ������weak_ptr�̃����N���؂�Ă��Ȃ����lock�֐���p���Ēl��Ԃ�
     * �؂�Ă����ꍇ�͑�������Ō�������
     * @tparam ComponentType �擾�R���|�[�l���g�̌^
     *
     * \param componentWptr �����N�؂���m�F����weak_ptr
     * \return �R���|�[�l���g�̃V�F�A�h�|�C���^��Ԃ�
     */
    template<is_Component ComponentType>
    std::shared_ptr<ComponentType> EnsureComponentValid(std::weak_ptr<ComponentType>& componentWptr)
    {
        auto owner = this->owner_Wptr.lock();
        if (!owner) return nullptr;
        return owner->EnsureComponentValid<ComponentType>(componentWptr);
    }

private:
    OwnerWPtr owner_Wptr;
};