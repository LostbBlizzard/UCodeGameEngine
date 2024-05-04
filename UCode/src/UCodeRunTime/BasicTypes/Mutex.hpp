#include <mutex>
#include <functional>
#include <optional>
#include "defer.hpp"
#include <UCodeRunTime/Core/CoreNamespace.hpp>
CoreStart


template<typename T>
class Mutex
{
public:
	Mutex()
	{

	}
	Mutex(T&& val)
	{
		_base = std::move(val);
	}
	Mutex(const T& val)
	{
		_base = std::move(val);
	}
	Mutex(Mutex&& other)
	{
		other._lock.lock();
		this->_lock.lock();

		UCodeGEDefer(this->_lock.unlock();)
		UCodeGEDefer(other._lock.unlock();)

		_base = std::move(other._base);
	}
	Mutex& operator=(Mutex&& other)
	{
		other._lock.lock();
		this->_lock.lock();
	
		UCodeGEDefer(this->_lock.unlock();)
		UCodeGEDefer(other._lock.unlock();)

		_base = std::move(other._base);

		return *this;
	}
	~Mutex()
	{

	}

	using UseFunc =std::function<void(T&)>;
	void Lock(UseFunc func)
	{
		_lock.lock();
		UCodeGEDefer(_lock.unlock();)

		func(_base);
	}

	bool TryLock(UseFunc func)
	{
		if (_lock.try_lock())
		{
			UCodeGEDefer(_lock.unlock();)
			
			func(_base);
			return true;
		}
		return false;
	}

	template<typename X>
	X Lock_r(std::function<X(T&)> func)
	{
		_lock.lock();
		UCodeGEDefer(_lock.unlock();)
		
		auto r = func(_base);	
		
		return r;
	}

	template<typename X>
	std::optional<X> TryLock_r(std::function<X(T&)> func)
	{
		if (_lock.try_lock())
		{
			UCodeGEDefer(_lock.unlock();)
			auto r = func(_base);

			return r;
		}
		return {};
	}

	std::mutex& Unsafe_GetLockBase()
	{
		return _lock;
	}
	T& Unsafe_GetBaseType()
	{
		return _base;
	}
private:
	std::mutex _lock;
	T _base;
};

CoreEnd
