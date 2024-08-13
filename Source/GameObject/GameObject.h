#pragma once
#include <vector>
#include <string>
#include <concepts>
#include <memory>
#include "Component/Component.h"

template <class T>
concept is_Component = requires{ std::is_base_of_v<Component, T>; };

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject() {};
	~GameObject() {};

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
		for (std::shared_ptr<Component>& component : component_vector)
		{
			std::shared_ptr<ComponentType> return_component 
				= std::dynamic_pointer_cast<ComponentType>(component);
			if (return_component == nullptr) continue;
			return return_component;
		}
		return nullptr;
	}
	
	/**
	 * @fn AddComponent
	 * @brief コンポーネント追加関数
	 * @tparam ComponentType 追加するコンポーネントの型
	 * @tparam Arguments 可変長引数型種
	 * \return 追加したコンポーネントのポインタを返す
	 */
	template<is_Component	ComponentType, typename ... Arguments>
	std::shared_ptr<ComponentType> AddComponent(Arguments ... args)
	{	
		std::shared_ptr<ComponentType> component = std::make_shared<ComponentType>(args...);
		component->SetOwner(shared_from_this());
		component_vector.emplace_back(component);
		return component;
	}

	/**
	 * @fn Update
	 * @brief 更新処理
	 * 
	 * \param elapsedTime 経過時間
	 */
	void Update(float elapsedTime);

	// 各取得関数
	const std::string GetName() const { return name; }

#ifdef _DEBUG
	/**
	 * デバックの情報を3D画面上に出力する関数
	 */
	void DrawDebugGUI() {};
	/**
	 * デバックの情報を3D画面上に出力する関数
	 */
	void DrawDebugPrimitive() {};
#endif // _DEBUG
private:
	/**
	 * @fn sortComponentsByPriority
	 * @brief コンポーネントを優先意順でソートする
	 */
	void sortComponentsByPriority();
private:
	const std::string name;

	using ComponentVector = std::vector<std::shared_ptr<Component>>;
	ComponentVector component_vector;
};