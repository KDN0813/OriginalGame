#pragma once
#include <vector>
#include <string>
#include <concepts>

class Component;

template <class T>
concept is_Component = requires{ std::is_base_of_v<Component, T>; };

class GameObject
{
public:
	GameObject() {};
	~GameObject() {};

	// TODO(08/07)GetComponent関数作成
	template<is_Component T>
	T* GetComponent(int no)
	{
		return	dynamic_cast<T*>(m_pComponents[no]);
	}
	
	// TODO(08/07)AddComponent関数作成
	/**
	 * @fn TypeComponent
	 * @brief コンポーネント追加関数
	 * 
	 * \return 追加したコンポーネントのポインタを返す
	 */
	//template<is_Component	T, typename ... Arguments>
	//T* AddComponent()
	//{
	//	if (GetComponent<T>())
	//	{
	//		return	nullptr;
	//	}
	//
	//	T* component = new T(std::forward<Arguments>(_pArgs)...);
	//	components.emplace_back(component);
	//	return	component;
	//}

	// TODO(08/07)Update関数作成
	
	// TODO(08/07)Componentの優先度順にソートする関数作成

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
	const std::string name;
	std::vector<Component*> components;
};