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
	 * @brief �R���|�[�l���gID������U��
	 * 
	 * @return ����U��ꂽID
	 */
	int ComponentIDAllocate()
	{
		static int id = 0;
		return id++;
	}
	/**
	 * @fn GetComponentID
	 * @brief �R���|�[�l���gID���擾
	 * 
	 * @tparam �R���|�[�l���g�̌^
	 * @return �R���|�[�l���gID
	 */
	template<is_Component ComponentType>
	int GetComponentID()
	{
		static int id = ComponentIDAllocate();
		return id;
	}

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
		auto component = componentWptr.lock();
		if (component)
		{
			return component;
		}
		
		// �R���|�[�l���g���擾����
		component = GetComponent<ComponentType>();
		componentWptr = component;
		return component;
	}

	/**
	 * @fn AddComponent
	 * @brief �R���|�[�l���g�ǉ��֐��B
	 * 
	 * @tparam ComponentType �ǉ�����R���|�[�l���g�̌^
	 * @tparam Arguments �ϒ������^��
	 * \return �ǉ������R���|�[�l���g�̃|�C���^��Ԃ�
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
	* @brief �R���|�[�l���g�ǉ��֐��B
	*
	* @tparam ComponentType �ǉ�����R���|�[�l���g�̌^
	* @param component �ǉ�����R���|�[�l���g�̃V�F�A�h�|�C���^
	* \return �ǉ������R���|�[�l���g�̃|�C���^��Ԃ�
	*/
	template<is_Component	ComponentType>
	std::shared_ptr<ComponentType> AddComponent(std::shared_ptr<ComponentType> component)
	{
		auto owner = component->GetOwner();
		assert(owner);	// ���ɏ��L�҂�����΃G���[
		if (owner) return nullptr;	// ���ɏ��L�҂�����ꍇ�ǉ����Ȃ�
		component->SetOwner(shared_from_this());
		component->SetComponentID(GetComponentID<ComponentType>());
		component_vec.emplace_back(component);
		return component;
	}

	std::shared_ptr<Object> AddChildren();

	/**
	 * @fn Update
	 * @brief �X�V����
	 * 
	 * \param elapsedTime �o�ߎ���
	 */
	void Update(float elapsedTime);
	/**
	 * \fn Initialize
	 * \brief �J�n�֐�
	 * 
	 */
	void Start();
	/**
	* \fn Finalize
	* \brief �C���֐�
	*
	*/
	void End();

	/**
	 * �p�����[�^�������l�ɐݒ肷��
	 * 
	 */
	void ReStart();

	// �e�擾�֐�
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
	 * @brief �R���|�[�l���g��D��ӏ��Ń\�[�g����
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
	 * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
	 */
	void DrawDebugGUI();
	/**
	 * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
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
	// �ڍו\������object�̑I��
	void DrawLister();
	// object�̏ڍו\��
	void DrawDetail();
private:
	std::set<std::shared_ptr<Object>> selection_object_vec;
#endif _DEBUG
};
