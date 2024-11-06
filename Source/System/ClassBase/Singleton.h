#pragma once

#include <assert.h>
#include <mutex>
#include "System/Misc.h"
#include "System\ClassBase\CopyDisable.h"

template<class T>
class Singleton : virtual public CopyDisable
{
public:
	//クラスの宣言
	class Instance final : virtual public CopyDisable
	{
		friend class Singleton;
	public:
		~Instance() = default;//デストラクタ

	private://インスタンスを生成するのはSingletonのみ
		explicit Instance(T* instance, std::recursive_mutex& mutex) :instance(instance), lock(mutex)
		{

		};

	public:
		/**
		 * @brief 直接instanceにアクセスできるようにする
		 * @return
		 */
		T* operator->() noexcept
		{
			return instance;
		}

		/**
		 * @brief 直接instanceを実体化できるようにする
		 * @return
		 */
		T& operator*() noexcept
		{
			return *instance;
		}

		/**
		 * @brief instanceのアドレスを取得する
		 * @return instanceのアドレス
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
		_ASSERT_EXPR_W(this->instance == nullptr, L"既にインスタンスが存在します");//インスタンスを複数生成しないように制限

		this->instance = pInstance;
	};
	virtual ~Singleton()
	{
		mutex.lock();//これ以上アクセスされないようにロック
		instance = nullptr;
	}

public:
	static Instance GetInstance()//インスタンスを取得する
	{
#ifdef INSTANCE_ERROR
		_ASSERT_EXPR_W(instance != nullptr, L"インスタンスが存在しません");//インスタンスが存在していない場合はエラー
#endif INSTANCE_ERROR
		return Instance(instance, mutex);
	}

private:
	inline static T* instance = nullptr;
	inline static std::recursive_mutex mutex;
};