#pragma once

#include <assert.h>
#include <mutex>
#include "System\ClassBase\CopyDisable.h"

template<class T>
class Singleton : virtual public CopyDisable
{
protected:
	Singleton(T* const pInstance)
	{
		assert(instance == nullptr);//�C���X�^���X�𕡐��������Ȃ��悤�ɐ���
		instance = pInstance;
	};
	virtual ~Singleton() { instance = nullptr; };
public:
	static T* Instance()noexcept{return instance;}
	static const T* const CInstance()noexcept{return instance;}
	static const std::mutex& GetInstanceMutex()noexcept{return mutex;}
private:
	inline static T* instance = nullptr;
	inline static std::mutex mutex;
};