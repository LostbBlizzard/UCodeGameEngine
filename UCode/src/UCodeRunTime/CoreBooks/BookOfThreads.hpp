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


using ThreadToRunID_t = int;
struct ThreadToRunID
{
	constexpr explicit ThreadToRunID(ThreadToRunID_t Value)
		:Base(Value)
	{

	}

	bool operator==(const ThreadToRunID& Other) const
	{
		return this->Base == Other.Base;
	}
	bool operator!=(const ThreadToRunID& Other) const
	{
		return this->Base != Other.Base;
	}

	ThreadToRunID_t Get_Base() const
	{
		return Base;
	}
private:
	ThreadToRunID_t Base;
};
using TaskID_t = int;
struct TaskID
{
	
	constexpr explicit TaskID(TaskID_t Value)
		:Base(Value)
	{

	}
	bool operator==(const TaskID& Other) const
	{
		return this->Base == Other.Base;
	}
	bool operator!=(const TaskID& Other) const
	{
		return this->Base != Other.Base;
	}

	TaskID_t Get_Base()
	{
		return Base;
	}
private:
	TaskID_t Base;
};


struct AsynNonVoidType {};
static constexpr  AsynNonVoidType AsynNonVoidValue = AsynNonVoidType();

static constexpr ThreadToRunID AnyThread = ThreadToRunID(-1);
static constexpr ThreadToRunID MainThread = ThreadToRunID(-2);
static constexpr ThreadToRunID NullThread = ThreadToRunID(-3);
static constexpr TaskID NullTaskID = TaskID(-1);

struct CurrentThreadInfo
{
	static thread_local ThreadToRunID CurrentThread;
};


using TaskType_t = int;
enum class TaskType : TaskType_t
{
	Main,
	Generic,
	FileIO,
	NetWorkIO,
	Rendering,
	Physics,
	Audio,
	DataProcessing,

	StartTaskType = FileIO,
	MaxTaskType = DataProcessing + 1,

	File_Input = FileIO,
	File_Output = FileIO,
	NetWork_Input = NetWorkIO,
	NetWork_Output = NetWorkIO,
	Data_Serialzation,
	Data_Deserialize,
};

ThreadToRunID GetThreadFromTask(TaskType type);

//use c-cast
using AnyFuture = std::future<AsynNonVoidType>;
using AnyDoneFuncPtr = Delegate<void, AsynNonVoidType>;
struct OnDoneInfo
{
	AnyDoneFuncPtr OnDone;
	ThreadToRunID ThreadToRun = AnyThread;
};


class BookOfThreads;
template<typename T>
struct AsynTask_t
{
	friend BookOfThreads;
	using ThisType = AsynTask_t<T>;
	using OnDoneFuncPtr = Delegate<void,T>;
	using Future = std::future<T>;

	template<typename T2> using Continue = Delegate<T2,T&&>;
	template<typename T2> using ContinueRet = AsynTask_t<T2>;
	using Cancel = Delegate<void>;
	using SelfRet = ThisType&&;
	
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
		Other._TastID = NullTaskID;
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
	SelfRet OnCompletedCurrentThread(OnDoneFuncPtr&& Value)
	{
		return OnCompletedOnThread(std::move(Value), CurrentThread);
	}
	SelfRet OnCompletedOnMainThread(OnDoneFuncPtr&& Value)
	{
		return OnCompletedOnThread(std::move(Value), MainThread);
	}

	SelfRet OnCompletedOnAnyThread(OnDoneFuncPtr&& Value)
	{
		return  OnCompletedOnThread(std::move(Value), Value);
	}

	SelfRet OnCompletedOnThread(OnDoneFuncPtr&& Value,TaskType task)
	{
		return  OnCompletedOnThread(std::move(Value), GetThreadFromTask(task));
	}

	SelfRet OnCompletedOnThread(OnDoneFuncPtr&& Value,ThreadToRunID Thread)
	{
		OnDoneInfo V;
		V.OnDone = std::move(Value);
		V.ThreadToRun = Thread;
		AddCallDone(_TastID, V);
		return std::move(*this);
	}

	template<typename T2>
	ContinueRet<T2> ContinueCurrentThread(Continue<T2>&& Value)
	{
		return ContinueOnThread<T2>(CurrentThread, std::move(Value));
	}

	ContinueRet<AsynNonVoidType> ContinueCurrentThread(Continue<void>&& Value)
	{
		Delegate<AsynNonVoidType, T&&> Func = [Value = std::move(Value)](T&& Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueCurrentThread<AsynNonVoidType>(std::move(Func));
	}

	template<typename T2>
	ContinueRet<T2> ContinueTaskThread(Continue<T2>&& Value)
	{
		return ContinueOnThread<T2>(CurrentThread, std::move(Value));
	}

	
	ContinueRet<AsynNonVoidType> ContinueTaskThread(Continue<void>&& Value)
	{
		Delegate<AsynNonVoidType, T&&> Func = [Value = std::move(Value)](T&& Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueTaskThread<AsynNonVoidType>(std::move(Func));
	}

	template<typename T2>
	ContinueRet<T2> ContinueOnMainThread(Continue<T2>&& Value)
	{
		return ContinueOnThread<T2>(MainThread, std::move(Value));
	}


	ContinueRet<AsynNonVoidType> ContinueOnMainThread(Continue<void>&& Value)
	{
		Delegate<AsynNonVoidType, T&&> Func = [Value = std::move(Value)](T&& Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueOnMainThread<AsynNonVoidType>(std::move(Func));
	}

	template<typename T2>
	ContinueRet<T2> ContinueOnAnyThread(Continue<T2>&& Value)
	{
		return ContinueOnThread<T2>(AnyThread, std::move(Value));
	}

	ContinueRet<AsynNonVoidType> ContinueOnAnyThread(Continue<void>&& Value)
	{
		Delegate<AsynNonVoidType, T&&> Func = [Value = std::move(Value)](T&& Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueOnAnyThread<AsynNonVoidType>(std::move(Func));
	}

	template<typename T2>
	ContinueRet<T2> ContinueOnThread(TaskType task, Continue<T2>&& Value)
	{
		return  ContinueOnThread<T2>(GetThreadFromTask(task), std::move(Value));
	}

	ContinueRet<AsynNonVoidType> ContinueOnThread(TaskType task, Continue<void>&& Value)
	{
		Delegate<AsynNonVoidType, T&&> Func = [Value = std::move(Value)](T&& Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueOnThread<AsynNonVoidType>(task,std::move(Func));
	}

	template<typename T2>
	ContinueRet<T2> ContinueOnThread(ThreadToRunID Thread, Continue<T2>&& Value)
	{
		Vector<TaskID> Deps;
		Deps.push_back(_TastID);
		return BookOfThreads::Threads->AddTask_t<T2>(Thread, std::move(Value), Deps);
	}

	ContinueRet<AsynNonVoidType> ContinueOnThread(ThreadToRunID Thread, Continue<AsynNonVoidType>&& Value)
	{
		Delegate<AsynNonVoidType, T&&> Func = [Value = std::move(Value)](T&& Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueOnThread<AsynNonVoidType>(Thread, std::move(Value));
	}




	SelfRet OnCancelCurrentThread(Cancel&& Value)
	{
		return OnCancelOnThread(std::move(Value), CurrentThread);
	}
	SelfRet OnCancelOnMainThread(Cancel&& Value)
	{
		return OnCancelOnThread(std::move(Value), MainThread);
	}

	SelfRet OnCancelOnAnyThread(Cancel&& Value)
	{
		return OnCancelOnThread(std::move(Value), AnyThread);
	}

	SelfRet OnCancelOnThread(Cancel&& Value, TaskType task)
	{
		return  OnCancelOnThread(std::move(Value), GetThreadFromTask(task));
	}
	SelfRet OnCancelOnThread(Cancel&& Value, ThreadToRunID Thread)
	{
		
		//OnDoneInfo V;
		//V.OnDone = std::move(Value);
		//V.ThreadToRun = Thread;
		//AddCallDone(_TastID, V);
		return std::move(*this);
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
	optional<T> TryGet_Op()
	{
		auto V = TryGet();
		if (V)
		{
			return *V;
		}
		else
		{
			return {};
		}
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
	void CancelTask()
	{

	}
	TaskID Get_TaskID() const
	{
		return _TastID;
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
		BookOfThreads::Threads->RunOnOnMainThread(std::move(Func));
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
	TaskID _TastID = NullTaskID;
	static void TryCallOnDone(TaskID ID, T&& Move)
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
	static void AddCallDone(TaskID ID, const OnDoneInfo& Info)
	{
		_DoneLock.lock();
		_Map.AddValue(ID, Info);
		_DoneLock.unlock();
	}
	static bool HasOnDone(TaskID ID)
	{
		return _Map.HasValue(ID);
	}
};
using AsynTask = AsynTask_t<AsynNonVoidType>;

class BookOfThreads :private libraryBook
{
public:
	
	using FuncPtr = Delegate<void>;
	
	struct TaskInfo
	{
		std::shared_ptr<FuncPtr> _Func;
		Vector<TaskID> TaskDependencies;
	};

	static BookOfThreads* Get(Gamelibrary* lib);
	static BookOfThreads* Find(const Gamelibrary* lib);
	
	ThreadToRunID GetNewThreadLockKey();
	void FreeThreadLock(ThreadToRunID key);


	AsynTask AddTask(ThreadToRunID thread,FuncPtr&& Func, const Vector<TaskID>& TaskDependencies);
	AsynTask AddTask(TaskType TaskType, FuncPtr&& Func,const Vector<TaskID>& TaskDependencies)
	{
		return AddTask(GetThreadFromTask(TaskType),std::move(Func), TaskDependencies);
	}

	

	
	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> AddTask_t(TaskType TaskType, Delegate<void,Pars...> task_function
		, const Vector<TaskID>& TaskDependencies, Pars&&... pars)
	{
		Delegate< AsynNonVoidType, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return AsynNonVoidValue;
		};

		return AddTask_t<NonVoidType,Pars...>(TaskType, Func, TaskDependencies, pars);
	}

	

	template <typename R, typename... Pars>
	AsynTask_t<R> AddTask_t(TaskType TaskType, Delegate<R,Pars...>&& task_function
		, const Vector<TaskID>& TaskDependencies, Pars... pars)
	{
		return AddTask_t(GetThreadFromTask(TaskType), task_function,TaskDependencies,pars...);
	}

	//Based From BS_thread_pool.hpp
	template <typename R, typename... Pars>
	AsynTask_t<R> AddTask_t(ThreadToRunID TaskType,Delegate<R,Pars...>&& task_function
		,const Vector<TaskID>& TaskDependencies,Pars&&... pars)
	{
		auto task_promise = std::make_shared<std::packaged_task<R(Pars...)>>(task_function);
		FuncPtr NewPtr;
		auto r = task_promise->get_future();
		AsynTask_t<R> Ret;

		_TaskLock.lock();
		Ret._TastID = _TastID++;
		TaskID NewTasklID = Ret._TastID;
		Ret.Set_Future(std::move(r));
		_TaskLock.unlock();

		NewPtr = [NewTasklID, task = std::move(task_promise),pars = std::move(pars...)]
		{
			(*task)(pars...);
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

		AddTask(TaskType, std::move(NewPtr), TaskDependencies);
		return Ret;
	}

	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> AddTask_t(ThreadToRunID TaskType, Delegate<void,Pars...>&& task_function
		, const Vector<TaskID>& TaskDependencies, Pars&&... pars)
	{
		Delegate<NonVoidType, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return AsynNonVoidValue;
		};

		return AddTask_t<NonVoidType, Pars...>(TaskType, Func, TaskDependencies,pars);
	}

	inline static BookOfThreads* Get_Threads()
	{
		return Threads;
	}

	
	void RunOnOnMainThread(FuncPtr&& Func, const Vector<TaskID>& TaskDependencies = {});
	

	template <typename R, typename... Pars>
	AsynTask_t<R> RunOnOnMainThread(Delegate<R, Pars...>&& task_function, const Vector<TaskID>& TaskDependencies, Pars&&... pars)
	{
		return AddTask_t<R>(MainThreadID, std::move(task_function), TaskDependencies,std::move(pars));
	}

	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> RunOnOnMainThread(Delegate<void, Pars...>&& task_function, const Vector<TaskID>& TaskDependencies, Pars&&... pars)
	{
		return AddTask_t<NonVoidType>(MainThreadID,std::move(task_function), TaskDependencies, std::move(pars));
	}

	static bool IsOnMainThread();
	static void ThrowErrIfNotOnMainThread();
	inline static BookOfThreads* Threads = nullptr;

	static bool IsTasksDone(const Vector<TaskID>& Tasks);
	static bool IsTasksDone(TaskID Task);


	


	

private:
	struct ThreadData
	{
		Vector<TaskInfo> _TaskToDo;
	};
	struct ThreadInfo
	{
		std::shared_ptr<std::condition_variable> _NewTask;
		std::shared_ptr<std::thread> thread;
		ThreadData _Data;
		const ThreadToRunID _ThreadID;
		ThreadInfo(ThreadToRunID id)
			:_ThreadID(id)
		{
			_NewTask = std::make_shared<std::condition_variable>();
		}
	}; 
	void CallTaskToDoOnMainThread();
	void Update() override;

	BookOfThreads(Gamelibrary* lib);
	~BookOfThreads();
	static void ThreadLoop(BookOfThreads* _This, ThreadInfo* Info);



	std::mutex _TaskLock;

	std::condition_variable _NewTask;
	Vector<ThreadInfo> _Threads;
	bool _EndThreads;
	ThreadToRunID _NextLockKey;

	ThreadData _MainThreadData;
	Vector<TaskInfo> _TaskToReAddOnToMainThread;

	//AnyThradInfo
	ThreadData _AnyThread;

	std::mutex _OnMainThreadLock;
	
	TaskID _TastID;
	 

	ThreadData& GetThreadInfo(ThreadToRunID ID);
	ThreadData& GetThreadNoneWorkingThread();
	



	
	inline static std::mutex _DoneLock;
	inline static BinaryVectorMap< TaskID, OnDoneInfo> _Map;
	inline static BinaryVectorMap < TaskID, std::shared_ptr<AnyFuture>> _Futures;

	static void AddCallDone(TaskID ID, const OnDoneInfo& Info)
	{
		_DoneLock.lock();
		_Map.AddValue(ID, Info);
		_DoneLock.unlock();
	}
	static bool HasOnDone(TaskID ID)
	{
		return _Map.HasValue(ID);
	}
	

};
CoreEnd
