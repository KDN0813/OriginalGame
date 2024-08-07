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

	// TODO(08/07)GetComponent�֐��쐬
	template<is_Component T>
	T* GetComponent(int no)
	{
		return	dynamic_cast<T*>(m_pComponents[no]);
	}
	
	// TODO(08/07)AddComponent�֐��쐬
	/**
	 * @fn TypeComponent
	 * @brief �R���|�[�l���g�ǉ��֐�
	 * 
	 * \return �ǉ������R���|�[�l���g�̃|�C���^��Ԃ�
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

	// TODO(08/07)Update�֐��쐬
	
	// TODO(08/07)Component�̗D��x���Ƀ\�[�g����֐��쐬

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
	const std::string name;
	std::vector<Component*> components;
};