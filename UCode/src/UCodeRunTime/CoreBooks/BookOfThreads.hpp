#pragma once

#include "../Core/GameRunTime.hpp"
#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Delegate.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/unordered_map.hpp>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <future>
CoreStart

typedef unsigned char ThreadToRunID;
using TaskID = int;

static constexpr ThreadToRunID AnyThread = 0;
static constexpr ThreadToRunID MainThread = 1;
 
struct CurrentThreadInfo
{
	static thread_local ThreadToRunID CurrentThread;
};



enum class TaskType
{
	Generic,
	FileIO,
	NetWorkIO,

	File_Input = FileIO,
	File_Output = FileIO,
};


class BookOfThreads;
template<typename T>
struct AsynTask_t
{
	friend BookOfThreads;
	using ThisType = AsynTask_t<T>;
	using OnDoneFuncPtr = Delegate<void,T>;
	using Future = std::future<T>;

	
	
	AsynTask_t(Future&& Item)
	{
		Set_Future(std::move(Item));
	}
	AsynTask_t()
	{
	
	}
	AsynTask_t(ThisType&& Other):
		  _TastID(Other._TastID)
	{
		Other._TastID = 0;
	}

	~AsynTask_t()
	{
		
	}
	ThisType& operator=(Future&& Item)
	{
		Set_Future(Item);
		return *this;
	}
	ThisType& operator=(ThisType&& Other)
	{
		_TastID = Other._TastID;
		Other._TastID = 0;
		return *this;
	}

	//Calls this Funcion on the same Thread it was made on.
	ThisType& OnCompletedCurrentThread(OnDoneFuncPtr&& Value)
	{
		return OnCompletedOnThread(std::move(Value), CurrentThread);
	}
	ThisType& OnCompletedOnMainThread(OnDoneFuncPtr&& Value)
	{
		return OnCompletedOnThread(std::move(Value), MainThread);
	}

	ThisType& OnCompletedOnAnyThread(OnDoneFuncPtr&& Value)
	{
		return OnCompletedOnThread(std::move(Value), AnyThread);
	}

	ThisType& OnCompletedOnThread(OnDoneFuncPtr&& Value,TaskType task)
	{
		return  OnCompletedOnThread(std::move(Value), AnyThread);
	}

	ThisType& OnCompletedOnThread(OnDoneFuncPtr&& Value,ThreadToRunID Thread)
	{
		OnDoneInfo V;
		V.OnDone = std::move(Value);
		V.ThreadToRun = Thread;
		AddCallDone(_TastID, V);
		return *this;
	}


	T GetValue()
	{
		return Get_Future().get();
	}
	T* TryGet()
	{
		if ( Get_Future()._Is_ready())
		{
			return   Get_Future().get();
		}
		return nullptr;
	}
	bool valid()
	{
		return Get_Future().valid();
	}
	void wait_for()
	{
		return Get_Future().wait_for();
	}
	void wait()
	{
		return   Get_Future().wait();
	}
	bool IsDone()
	{
		return   Get_Future()._Is_ready();
	}
	static void RemoveCallID(TaskID ID)
	{
		_DoneLock.lock();
		if (ID != 0)
		{
			if (_Map.HasValue(ID)) {
				_Map.erase(ID);
			}
			if (_Futures.HasValue(ID)) {
				_Futures.erase(ID);
			}
		}
		_DoneLock.unlock();
	}

	void SetValue(T&& Value)
	{
		Delegate<void> Func = [_TastID = _TastID, Value = std::move(Value)]() mutable
		{
			TryCallOnDone(_TastID,std::move(Value));
		};
		BookOfThreads::Threads->RunOnOnMainThread(Func);
	}
	Future& Get_Future()
	{
		return  Get_Future(_TastID);
	}
	void Set_Future(Future&& Value)
	{
		_DoneLock.lock();
		_Futures[_TastID] =std::make_shared<Future>(std::move(Value));
		_DoneLock.unlock();
	}

	static Future& Get_Future(TaskID _TastID)
	{
		_DoneLock.lock();
		auto& R = *_Futures[_TastID];
		_DoneLock.unlock();
		return R;
	}
private:
	TaskID _TastID = 0;

	struct OnDoneInfo
	{
		OnDoneFuncPtr OnDone;
		ThreadToRunID ThreadToRun = AnyThread;
	};
	
	inline static std::mutex _DoneLock;
	inline static BinaryVectorMap< TaskID, OnDoneInfo> _Map;
	inline static BinaryVectorMap < TaskID,std::shared_ptr<Future>> _Futures;
	
	static void AddCallDone(TaskID ID,const OnDoneInfo& Info)
	{
		_DoneLock.lock();
		_Map.AddValue(ID, Info);
		_DoneLock.unlock();
	}
	static bool HasOnDone(TaskID ID)
	{
		return _Map.HasValue(ID);
	}
	static void TryCallOnDone(TaskID ID,T&& Move)
	{
		_DoneLock.lock();
		if (_Map.HasValue(ID)) 
		{
			auto& V = _Map.at(ID);

			if (V.OnDone)
			{
				if (V.ThreadToRun == CurrentThreadInfo::CurrentThread)
				{
					V.OnDone(std::move(Move));
				}
				else
				{
					Delegate<void>  Func = [OnDone = std::make_shared<OnDoneFuncPtr>(std::move(V.OnDone)), 
						Val = std::make_shared<T>(std::move(Move))]() mutable
					{
						_DoneLock.lock();
						(*OnDone)(std::move(*Val));
						_DoneLock.unlock();
					};
					
					BookOfThreads::Threads->AddTask_t(
						V.ThreadToRun,
						std::move(Func)
					);
				}
			}
		}
		_DoneLock.unlock();
	}
	
	
};
using AsynTask = AsynTask_t<bool>;

class BookOfThreads :private libraryBook
{
public:
	
	using FuncPtr = Delegate<void>;
	
	struct TastInfo
	{
		FuncPtr _Func;
	};


	static BookOfThreads* Get(Gamelibrary* lib);
	static BookOfThreads* Find(const Gamelibrary* lib);
	
	ThreadToRunID GetNewThreadLockKey();
	void FreeThreadLock(ThreadToRunID key);


	AsynTask AddTask(ThreadToRunID thread,FuncPtr Func);
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

	


	//Based From BS_thread_pool.hpp
	template <typename R,typename... Pars>
	AsynTask_t<R> AddTask_t(TaskType TaskType, std::function<R(Pars...)> task_function)
	{
		auto task_promise =std::make_shared<std::packaged_task<R()>>(task_function);
		FuncPtr NewPtr;
		auto r = task_promise->get_future();
		AsynTask_t<R> Ret;

		_TaskLock.lock();
		Ret._TastID = _TastID++;
		TaskID NewTasklID = Ret._TastID;
		Ret.Set_Future(std::move(r));
		_TaskLock.unlock();

		NewPtr = [NewTasklID,task = std::move(task_promise)]
		{
			(*task)();
			if (AsynTask_t<R>::HasOnDone(NewTasklID)) 
			{	
				auto Val = AsynTask_t<R>::Get_Future(NewTasklID).get();
				AsynTask_t<R>::TryCallOnDone(NewTasklID,std::move(Val));
				AsynTask_t<R>::RemoveCallID(NewTasklID);
			}
			else
			{
				AsynTask_t<R>::RemoveCallID(NewTasklID);
			}
		};
		
		AddTask(TaskType, std::move(NewPtr));
		return Ret;
	}

	template <typename... Pars>
	AsynTask_t<bool> AddTask_t(TaskType TaskType, std::function<void(Pars...)> task_function)
	{
		Delegate<bool, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return true;
		};

		return AddTask_t<bool,Pars...>(TaskType, Func);
	}

	template <typename R, typename... Pars>
	AsynTask_t<R> AddTask_t(ThreadToRunID TaskType, std::function<R(Pars...)> task_function)
	{
		auto task_promise = std::make_shared<std::packaged_task<R()>>(task_function);
		FuncPtr NewPtr;
		auto r = task_promise->get_future();
		AsynTask_t<R> Ret;

		_TaskLock.lock();
		Ret._TastID = _TastID++;
		TaskID NewTasklID = Ret._TastID;
		Ret.Set_Future(std::move(r));
		_TaskLock.unlock();

		NewPtr = [NewTasklID, task = std::move(task_promise)]
		{
			(*task)();
			if (AsynTask_t<R>::HasOnDone(NewTasklID))
			{
				auto Val = AsynTask_t<R>::Get_Future(NewTasklID).get();
				AsynTask_t<R>::TryCallOnDone(NewTasklID, std::move(Val));
				AsynTask_t<R>::RemoveCallID(NewTasklID);
			}
			else
			{
				AsynTask_t<R>::RemoveCallID(NewTasklID);
			}
		};

		AddTask(TaskType, std::move(NewPtr));
		return Ret;
	}

	template <typename... Pars>
	AsynTask_t<bool> AddTask_t(ThreadToRunID TaskType, std::function<void(Pars...)> task_function)
	{
		Delegate<bool, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return true;
		};

		return AddTask_t<bool, Pars...>(TaskType, Func);
	}

	inline static BookOfThreads* Get_Threads()
	{
		return Threads;
	}

	void RunOnOnMainThread(const std::function<void()>& Func);
	void RunOnOnMainThread(std::function<void()>&& Func);
	
	static bool IsOnMainThread();
	static bool ThrowErrIfNotOnMainThread();
	inline static BookOfThreads* Threads = nullptr;
private:
	BookOfThreads(Gamelibrary* lib);
	~BookOfThreads();
	static void ThreadLoop(BookOfThreads* _This);
	std::mutex _TaskLock;
	std::condition_variable _NewTask;
	Vector<std::thread> _Threads;
	Vector<TastInfo> _TaskToDo;
	bool _EndThreads;
	ThreadToRunID _NextLockKey;


	Vector<std::function<void()>> _TaskToDoOnMainThread;
	std::mutex _OnMainThreadLock;
	void CallTaskToDoOnMainThread();
	
	void Update() override;

	TaskID _TastID;
};
CoreEnd
