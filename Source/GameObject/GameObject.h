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
	/**
	 * @fn sortComponentsByPriority
	 * @brief �R���|�[�l���g��D��ӏ��Ń\�[�g����
	 */
	void sortComponentsByPriority();
private:
	const std::string name;

	using ComponentVector = std::vector<std::shared_ptr<Component>>;
	ComponentVector component_vector;
};