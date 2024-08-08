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
	 * @brief �R���|�[�l���g���擾����
	 * @tparam ComponentType �擾�R���|�[�l���g�̌^
	 * 
	 * \return �R���|�[�l���g�̃|�C���^��Ԃ�
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
	 * @brief �R���|�[�l���g�ǉ��֐�
	 * @tparam ComponentType �ǉ�����R���|�[�l���g�̌^
	 * @tparam Arguments �ϒ������^��
	 * \return �ǉ������R���|�[�l���g�̃|�C���^��Ԃ�
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
	 * @brief �X�V����
	 * 
	 * \param elapsedTime �o�ߎ���
	 */
	void Update(float elapsedTime);

	// �e�擾�֐�
	const std::string GetName() const { return name; }

#ifdef _DEBUG
	/**
	 * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
	 */
	void DrawDebugGUI() {};
	/**
	 * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
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
	 * @brief �R���|�[�l���g��D��ӏ��Ń\�[�g����
	 */
	void sortComponentsByPriority();
private:
	const std::string name;
	std::vector<Component*> components;
};