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
	 * @fn GetComponent
	 * @brief �R���|�[�l���g���擾����
	 * @tparam ComponentType �擾�R���|�[�l���g�̌^
	 * 
	 * \return �R���|�[�l���g�̃V�F�A�h�|�C���^��Ԃ�
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
		assert(true);
		return nullptr;
	}
	
	/**
	 * @fn AddComponent
	 * @brief �R���|�[�l���g�ǉ��֐�
	 * @tparam ComponentType �ǉ�����R���|�[�l���g�̌^
	 * @tparam Arguments �ϒ������^��
	 * \return �ǉ������R���|�[�l���g�̃|�C���^��Ԃ�
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
	 * @brief �X�V����
	 * 
	 * \param elapsedTime �o�ߎ���
	 */
	void Update(float elapsedTime);

	// �e�擾�֐�
	void SetName(const char* name) { this->name = name; }
	const std::string GetName() const { return this->name; }
	const char* GetNameCStr() const { return this->name.c_str(); }
	const bool GetIsActive() { return this->is_active; }
	void SetIsActive(const bool is_active) { this->is_active = is_active; }

private:
	/**
	 * @fn sortComponentsByPriority
	 * @brief �R���|�[�l���g��D��ӏ��Ń\�[�g����
	 */
	void sortComponentsByPriority();
private:
	std::string name;
	bool is_active = true;
	using ComponentVector = std::vector<std::shared_ptr<Component>>;
	ComponentVector component_vector;

#ifdef _DEBUG
public:
	/**
	 * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
	 */
	void DrawDebugGUI();
	/**
	 * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
	 */
	void DrawDebugPrimitive() {};
#endif // _DEBUG
};

class ObjectManager
{
public:
	ObjectManager() {};
	~ObjectManager() {};

	std::shared_ptr<Object> Create();

	void Update(float elapsedTime);

private:
	std::vector<std::shared_ptr<Object>> object_vector;

#ifdef _DEBUG
public:
	void DrawDebugGUI();

private:
	// �ڍו\������object�̑I��
	void DrawLister();
	// object�̏ڍו\��
	void DrawDetail();
private:
	std::set<std::shared_ptr<Object>> selection_object_vector;
#endif _DEBUG
};
