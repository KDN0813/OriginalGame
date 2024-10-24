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
		assert(this->instance == nullptr);//インスタンスを複数生成しないように制限
		this->instance = pInstance;
	};
	virtual ~Singleton() { instance = nullptr; };
public:
	static T* Instance()noexcept{return instance;}
	static const T* const CInstance()noexcept{return instance;}
	static std::mutex& GetInstanceMutex()noexcept{
		return mutex;
	}
private:
	inline static T* instance = nullptr;
	inline static std::mutex mutex;
};