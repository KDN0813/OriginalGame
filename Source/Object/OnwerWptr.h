#pragma once
#include <memory>
#include "Object/Object.h"

using Owner = Object;
using OwnerPtr = std::shared_ptr<Owner>;
using OwnerWPtr = std::weak_ptr<Owner>;

// Onwerクラス
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
     * @brief コンポーネントを取得する
     * @tparam ComponentType 取得コンポーネントの型
     *
     * \return コンポーネントのシェアドポインタを返す
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
     * @brief コンポーネントが有効であるか確認する
     * 引数のweak_ptrのリンクが切れていなければlock関数を用いて値を返す
     * 切れていた場合は総当たりで検索する
     * @tparam ComponentType 取得コンポーネントの型
     *
     * \param componentWptr リンク切れを確認するweak_ptr
     * \return コンポーネントのシェアドポインタを返す
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