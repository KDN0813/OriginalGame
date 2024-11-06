#pragma once

#include <assert.h>
#include <mutex>
#include "System/Misc.h"
#include "System\ClassBase\CopyDisable.h"

template<class T>
class Singleton : virtual public CopyDisable
{
public:
	//�N���X�̐錾
	class Instance final : virtual public CopyDisable
	{
		friend class Singleton;
	public:
		~Instance() = default;//�f�X�g���N�^

	private://�C���X�^���X�𐶐�����̂�Singleton�̂�
		explicit Instance(T* instance, std::recursive_mutex& mutex) :instance(instance), lock(mutex)
		{

		};

	public:
		/**
		 * @brief ����instance�ɃA�N�Z�X�ł���悤�ɂ���
		 * @return
		 */
		T* operator->() noexcept
		{
			return instance;
		}

		/**
		 * @brief ����instance�����̉��ł���悤�ɂ���
		 * @return
		 */
		T& operator*() noexcept
		{
			return *instance;
		}

		/**
		 * @brief instance�̃A�h���X���擾����
		 * @return instance�̃A�h���X
		 */
		_NODISCARD T* Get() const noexcept
		{
			return instance;
		}

	private:
		T* instance = nullptr;
		std::lock_guard<std::recursive_mutex> lock;
	};

protected:
	Singleton(T* const pInstance)
	{
		_ASSERT_EXPR_W(this->instance == nullptr, L"���ɃC���X�^���X�����݂��܂�");//�C���X�^���X�𕡐��������Ȃ��悤�ɐ���

		this->instance = pInstance;
	};
	virtual ~Singleton()
	{
		mutex.lock();//����ȏ�A�N�Z�X����Ȃ��悤�Ƀ��b�N
		instance = nullptr;
	}

public:
	static Instance GetInstance()//�C���X�^���X���擾����
	{
#ifdef INSTANCE_ERROR
		_ASSERT_EXPR_W(instance != nullptr, L"�C���X�^���X�����݂��܂���");//�C���X�^���X�����݂��Ă��Ȃ��ꍇ�̓G���[
#endif INSTANCE_ERROR
		return Instance(instance, mutex);
	}

private:
	inline static T* instance = nullptr;
	inline static std::recursive_mutex mutex;
};