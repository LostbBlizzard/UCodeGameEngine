#pragma once

#include "../Core/GameRunTime.hpp"
#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Delegate.hpp>

#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <future>
CoreStart

struct AsynTask
{

};

template<typename T>
using AsynTask_t = std::future<T>;

class BookOfThreads :private libraryBook
{
public:
	
	using FuncPtr = Delegate<void>;
	struct TastInfo
	{
		FuncPtr _Func;
	};
	enum class TaskType
	{
		Generic,
		FileIO,
		NetWorkIO,

		File_Input = FileIO,
		File_Output = FileIO,
	};
	typedef unsigned char ThreadLockKey;
	static constexpr ThreadLockKey AnyThread = 0;

	static BookOfThreads* Get(Gamelibrary* lib);
	static BookOfThreads* Find(const Gamelibrary* lib);
	
	ThreadLockKey GetNewThreadLockKey();
	void FreeThreadLock(ThreadLockKey key);


	AsynTask AddTask(ThreadLockKey thread,FuncPtr Func);
	AsynTask AddTask(TaskType TaskType, FuncPtr Func);

	template<typename... Pars>
	AsynTask AddTask(void(*Func)(Pars...), Pars...Par)
	{
		return AddTask(TaskType::Generic, Func, Par...);
	}


	template<typename... Pars>
	AsynTask AddTask(TaskType TaskType,void(*Func)(Pars...),Pars...Par)
	{
		FuncPtr Ptr;
		Ptr._func = std::bind(Func, Par);
		return AddTask(TaskType, Ptr);
	}

	template<typename T,typename... Pars>
	AsynTask AddTask(TaskType TaskType,T* This, void(T::*Func)(Pars...), Pars...Par)
	{
		std::function<void()> Tep = [This, Func, Par...]
		{
			(*This.*Func)(Par...);
		};
		FuncPtr Ptr(Tep);
		return AddTask(TaskType, Ptr);
	}



	template <typename... Pars, typename R>
	AsynTask_t<R> AddTask_t(R(*Func)(Pars...), Pars...Par)
	{
		return AddTask(TaskType::Generic, Func, Par...);
	}

	template <typename... Pars, typename R>
	AsynTask_t<R> AddTask_t(TaskType TaskType, R(*Func)(Pars...), Pars...Par)
	{
		std::function<R(Pars...)> task_function = [Func, Par...]
		{
			return Func(Par...);
		};
		return AddTask_t(TaskType, task_function);
	}
	template <typename T, typename... Pars, typename R>
	AsynTask_t<R> AddTask_t(TaskType TaskType, T* This, R(T::* Func)(Pars...), Pars...Par)
	{
		std::function<R(Pars...)> task_function = [This, Func, Par...]
		{
			return (*This.*Func)(Par...);
		};
		return AddTask_t(TaskType, task_function);
	}

	
	//From BS_thread_pool.hpp
	template <typename... Pars, typename R>
	AsynTask_t<R> AddTask_t(TaskType TaskType, std::function<R(Pars...)> task_function)
	{
		auto task_promise =std::make_shared<std::packaged_task<R()>>(task_function);
		FuncPtr NewPtr;
		auto r = task_promise->get_future();
		
		NewPtr = [task = std::move(task_promise)]
		{
			(*task)();
		};
		
		AddTask(TaskType, NewPtr);
		return r;
	}

	inline static BookOfThreads* Get_Threads()
	{
		return Threads;
	}

	void RunOnOnMainThread(const std::function<void()>& Func);
	void RunOnOnMainThread(std::function<void()>&& Func);
	
	static bool IsOnMainThread();
	static bool ThrowErrIfNotOnMainThread();
private:
	BookOfThreads(Gamelibrary* lib);
	~BookOfThreads();
	static void ThreadLoop(BookOfThreads* _This);
	std::mutex _TaskLock;
	std::condition_variable _NewTask;
	Vector<std::thread> _Threads;
	Vector<TastInfo> _TaskToDo;
	bool _EndThreads;
	ThreadLockKey _NextLockKey;


	Vector<std::function<void()>> _TaskToDoOnMainThread;
	std::mutex _OnMainThreadLock;
	inline static BookOfThreads* Threads =nullptr;
	void CallTaskToDoOnMainThread();
	
	void Update() override;
};
CoreEnd
