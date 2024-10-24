#pragma once
#include <vector>
#include <set>
#include <string>
#include <concepts>
#include <memory>
#include "Component/Component.h"

template <class T>
concept is_Component = requires{ std::is_base_of_v<Component, T>; };

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object() {};
	~Object() {};

	/**
	 * @fn ComponentIDAllocate
	 * @brief コンポーネントIDを割り振る
	 * 
	 * @return 割り振られたID
	 */
	int ComponentIDAllocate()
	{
		static int id = 0;
		return id++;
	}
	/**
	 * @fn GetComponentID
	 * @brief コンポーネントIDを取得
	 * 
	 * @tparam コンポーネントの型
	 * @return コンポーネントID
	 */
	template<is_Component ComponentType>
	int GetComponentID()
	{
		static int id = ComponentIDAllocate();
		return id;
	}

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
		for (std::shared_ptr<Component>& component : component_vec)
		{
			std::shared_ptr<ComponentType> return_component 
				= std::dynamic_pointer_cast<ComponentType>(component);
			if (return_component == nullptr) continue;
			return return_component;
		}
		//assert(true);
		return nullptr;
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
		auto component = componentWptr.lock();
		if (component)
		{
			return component;
		}
		
		// コンポーネントを取得する
		component = GetComponent<ComponentType>();
		componentWptr = component;
		return component;
	}

	/**
	 * @fn AddComponent
	 * @brief コンポーネント追加関数。
	 * 
	 * @tparam ComponentType 追加するコンポーネントの型
	 * @tparam Arguments 可変長引数型種
	 * \return 追加したコンポーネントのポインタを返す
	 */
	template<is_Component	ComponentType, typename ... Arguments>
	std::shared_ptr<ComponentType> AddComponent(Arguments ... args)
	{	
		std::shared_ptr<ComponentType> component = std::make_shared<ComponentType>(args...);
		component->SetOwner(shared_from_this());
		component->SetComponentID(GetComponentID<ComponentType>());
		component_vec.emplace_back(component);
		return component;
	}
	/**
	* @fn AddComponent
	* @brief コンポーネント追加関数。
	*
	* @tparam ComponentType 追加するコンポーネントの型
	* @param component 追加するコンポーネントのシェアドポインタ
	* \return 追加したコンポーネントのポインタを返す
	*/
	template<is_Component	ComponentType>
	std::shared_ptr<ComponentType> AddComponent(std::shared_ptr<ComponentType> component)
	{
		auto owner = component->GetOwner();
		assert(owner);	// 既に所有者がいればエラー
		if (owner) return nullptr;	// 既に所有者がいる場合追加しない
		component->SetOwner(shared_from_this());
		component->SetComponentID(GetComponentID<ComponentType>());
		component_vec.emplace_back(component);
		return component;
	}

	std::shared_ptr<Object> AddChildren();

	/**
	 * @fn Update
	 * @brief 更新処理
	 * 
	 * \param elapsedTime 経過時間
	 */
	void Update(float elapsedTime);
	/**
	 * \fn Initialize
	 * \brief 開始関数
	 * 
	 */
	void Start();
	/**
	* \fn Finalize
	* \brief 修了関数
	*
	*/
	void End();

	/**
	 * パラメータを初期値に設定する
	 * 
	 */
	void ReStart();

	// 各取得関数
	void SetName(const char* name) { this->name = name; }
	const std::string GetName() const { return this->name; }
	const char* GetNameCStr() const { return this->name.c_str(); }
	std::shared_ptr<Object> GetParent() { return this->parent_Wptr.lock(); }
	const std::vector<std::shared_ptr<Object>>& GetChildren() { return this->children; }
	const bool& GetIsActive() { return this->is_active; }
	const bool& GetIsRemove() { return this->is_remove; }
	void SetIsActive(const bool is_active) { this->is_active = is_active; }
	void SetParent(std::shared_ptr<Object> parent) { this->parent_Wptr = parent; }
	void SetIsRemove(bool is_remove) { this->is_remove = is_remove; }

private:
	/**
	 * @fn sortComponentsByPriority
	 * @brief コンポーネントを優先意順でソートする
	 */
	void sortComponentsByPriority();
private:
	std::string name;
	bool is_active = true;
	bool is_remove = false;
	using ComponentVector = std::vector<std::shared_ptr<Component>>;
	ComponentVector component_vec;
	std::vector<std::shared_ptr<Object>> children;
	std::weak_ptr<Object> parent_Wptr;

#ifdef _DEBUG
public:
	/**
	 * デバックの情報を3D画面上に出力する関数
	 */
	void DrawDebugGUI();
	/**
	 * デバックの情報を3D画面上に出力する関数
	 */
	void DrawDebugPrimitive();
#endif // _DEBUG
};

class ObjectManager
{
public:
	ObjectManager() {};
	~ObjectManager() {};

	std::shared_ptr<Object> Create();

	void Update(float elapsedTime);

	void Remove(std::shared_ptr<Object> object);

	void ReStart();
private:
	std::vector<std::shared_ptr<Object>> start_object_vec;
	std::vector<std::shared_ptr<Object>> update_object_vec;
	std::set<std::shared_ptr<Object>> remove_object_vec;

#ifdef _DEBUG
public:
	void DrawDebugGUI();

	void DrawDebugPrimitive();

private:
	// 詳細表示するobjectの選択
	void DrawLister();
	// objectの詳細表示
	void DrawDetail();
private:
	std::set<std::shared_ptr<Object>> selection_object_vec;
#endif _DEBUG
};
