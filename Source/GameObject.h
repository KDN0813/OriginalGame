#pragma once
#include <vector>
#include <string>
#include <concepts>
#include "Component/Component.h"

template <class T>
concept is_Component = requires{ std::is_base_of_v<Component, T>; };

class GameObject
{
public:
	GameObject() {};
	~GameObject() {};

	/**
	 * @fn GetComponent
	 * @brief コンポーネントを取得する
	 * @tparam ComponentType 取得コンポーネントの型
	 * 
	 * \return コンポーネントのポインタを返す
	 */
	template<is_Component ComponentType>
	ComponentType* GetComponent()
	{
		ComponentType* return_component = nullptr;

		for (auto& component : components)
		{
			return_component = dynamic_cast<ComponentType*>(component);
			if (return_component != nullptr) break;
		}

		return return_component;
	}
	
	/**
	 * @fn TypeComponent
	 * @brief コンポーネント追加関数
	 * @tparam ComponentType 追加するコンポーネントの型
	 * @tparam Arguments 可変長引数型種
	 * \return 追加したコンポーネントのポインタを返す
	 */
	template<is_Component	ComponentType, typename ... Arguments>
	ComponentType* AddComponent(Arguments ... args)
	{	
		ComponentType* component = new ComponentType(std::forward<Arguments>(args)...);
		AddComponent(component);
		return	component;
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
	void AddComponent(Component* component)
	{
		component->SetOwner(this);
		components.emplace_back(component);
	}

	/**
	 * @fn sortComponentsByPriority
	 * @brief コンポーネントを優先意順でソートする
	 */
	void sortComponentsByPriority();
private:
	const std::string name;
	std::vector<Component*> components;
};