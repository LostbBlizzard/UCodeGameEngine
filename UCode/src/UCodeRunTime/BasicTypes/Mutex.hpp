#include <mutex>
#include <functional>
#include <optional>

#include <UCodeRunTime/Core/CoreNamespace.hpp>
CoreStart


template<typename T>
class Mutex
{
public:
	Mutex()
	{

	}
	Mutex(Mutex&& other)
	{
		other._lock.lock();
		this->_lock.lock();
			
		_base = std::move(other);

		this->_lock.unlock();
		other._lock.unlock();
	}
	operator=(Mutex&& other)
	{
		other._lock.lock();
		this->_lock.lock();

		_base = std::move(other);

		this->_lock.unlock();
		other._lock.unlock();
	}
	~Mutex()
	{

	}

	using UseFunc =std::function<void(T&)>;
	void Lock(UseFunc func)
	{
		_lock.lock();

		func(_base);

		_lock.unlock();
	}

	using UseFunc = std::function<void(T&)>;
	bool TryLock(UseFunc func)
	{
		if (_lock.try_lock())
		{
			func(_base);
			_lock.unlock();
			return true;
		}
		return false;
	}

	using UseFuncC = std::function<void(const T&)>;
	template<typename X>
	X LockRead(std::function<X(const T&)> func)
	{
		_lock.lock();

		auto  r = func(_base);
		
		_lock.unlock();
		
		return r;
	}

	using UseFuncC = std::function<void(const T&)>;
	template<typename X>
	std::optional<X> TryLockRead(std::function<X(const T&)> func)
	{
		if (_lock.try_lock())
		{
			auto  r = func(_base);

			_lock.unlock();
			return r;
		}
		return {};
	}
private:
	std::mutex _lock;
	T _base;
};

CoreEnd
