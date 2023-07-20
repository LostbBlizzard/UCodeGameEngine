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


struct RuningTaskData
{
	using Cancel = Delegate<void>;

	AnyDoneFuncPtr OnDone;
	ThreadToRunID OnDoneToRun = NullThread;

	Cancel OnCancel;
	ThreadToRunID OnCancelToRun = NullThread;


	std::shared_ptr<AnyFuture> Future;
	TaskID ID =NullTaskID;


	//Thread the Task is meant the run on.
	ThreadToRunID ThreadToRun = AnyThread;
private:
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
		  _TaskID(Other._TaskID)
	{
		Other._TaskID = NullTaskID;
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
		_TaskID = Other._TaskID;
		Other._TaskID = 0;
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
		SetTaskData_OnDone(_TaskID,std::move(Value),Thread);
		return std::move(*this);
	}

	template<typename T2>
	ContinueRet<T2> ContinueCurrentThread(Continue<T2>&& Value)
	{
		return ContinueOnThread<T2>(CurrentThreadInfo::CurrentThread, std::move(Value));
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
		return ContinueOnThread<T2>(CurrentThreadInfo::CurrentThread, std::move(Value));
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
		Deps.push_back(_TaskID);
		Delegate<T2> Func = [this,Value = std::move(Value)]()
		{
			return T2();
			//return Value(this.GetValue());
		};
		return {};
		//return BookOfThreads::Threads->AddTask_t<T2>(Thread,Func,Deps);
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
		SetTaskData_OnCancel(_TaskID, std::move(Value), Thread);
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
		TryCallCancel(_TaskID);
	}
	TaskID Get_TaskID() const
	{
		return _TaskID;
	}


	static void RemoveCallID(TaskID ID)
	{
		BookOfThreads::_DoneLock.lock();
		if (ID != NullTaskID)
		{
			if (BookOfThreads::_Map.HasValue(ID)) {
				BookOfThreads::_Map.erase(ID);
			}
		}
		BookOfThreads::_DoneLock.unlock();
	}

	void SetValue(T&& Value)
	{
		Delegate<void> Func = [_TaskID = _TaskID, Value = std::move(Value)]() mutable
		{
			TryCallOnDone(_TaskID,std::move(Value));
		};
		BookOfThreads::Threads->RunOnOnMainThread(std::move(Func));
	}
	
private:
	TaskID _TaskID = NullTaskID;
	static void TryCallOnDone(TaskID ID, T&& Move)
	{
		BookOfThreads::TryCallOnDone<T>(ID, std::move(Move));
	}
	static void TryCallCancel(TaskID ID)
	{
		BookOfThreads::TryCallCancel(ID);
	}

	static void SetTaskData_OnDone(TaskID ID,OnDoneFuncPtr&& Value,ThreadToRunID Thread)
	{
		BookOfThreads::SetTaskData_Done(ID, std::move(*(AnyDoneFuncPtr*)&Value),Thread);
	}
	static void SetTaskData_OnCancel(TaskID ID,Cancel&& Value, ThreadToRunID Thread)
	{
		BookOfThreads::SetTaskData_Cancel(ID, std::move(Value), Thread);
	}

	static bool HasOnDone(TaskID ID)
	{
		return BookOfThreads::HasOnDone(ID);
	}
	Future& Get_Future()
	{
		return  Get_Future(_TaskID);
	}
	void Set_Future(Future&& Value)
	{
		BookOfThreads::_DoneLock.lock();
		BookOfThreads::_Map[_TaskID].Future = std::make_shared<AnyFuture>(std::move(*(AnyFuture*)&Value));
		BookOfThreads::_DoneLock.unlock();
	}

	static Future& Get_Future(TaskID _TaskID)
	{
		BookOfThreads::_DoneLock.lock();
		auto& R = *(Future*)BookOfThreads::_Map[_TaskID].Future.get();
		BookOfThreads::_DoneLock.unlock();
		return R;
	}
};
using AsynTask = AsynTask_t<AsynNonVoidType>;

class BookOfThreads :private libraryBook
{
public:
	
	using FuncPtr = Delegate<void>;

	template<typename T> using OnDoneFuncPtr = Delegate<void, T>;
	//#define MoveParPack(Pack_t,P) std::forward<##Pack_t&&>(##P)...
	//#define MovedParsParam(Pars) Pars##&&...
	#define MoveParPack(Pack_t,P) P...
	#define MovedParsParam(Pars) Pars##...


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
		, const Vector<TaskID>& TaskDependencies, MovedParsParam(Pars) pars)
	{
		Delegate<AsynNonVoidType, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return AsynNonVoidValue;
		};

		return AddTask_t<AsynNonVoidType,Pars...>(TaskType, Func, TaskDependencies, MoveParPack(Pars, pars));
	}
	

	template <typename R, typename... Pars>
	AsynTask_t<R> AddTask_t(TaskType TaskType, Delegate<R,Pars...>&& task_function
		, const Vector<TaskID>& TaskDependencies,MovedParsParam(Pars) pars)
	{
		return AddTask_t<R,Pars...>(GetThreadFromTask(TaskType),std::move(task_function),TaskDependencies,MoveParPack(Pars,pars));
	}

	//Based From BS_thread_pool.hpp
	template <typename R, typename... Pars>
	AsynTask_t<R> AddTask_t(ThreadToRunID TaskType,Delegate<R,Pars...>&& task_function
		,const Vector<TaskID>& TaskDependencies, MovedParsParam(Pars) pars)
	{
		auto task_promise = std::make_shared<std::packaged_task<R(Pars...)>>(task_function);

		auto r = task_promise->get_future();
		AsynTask_t<R> Ret;

		_TaskLock.lock();
		Ret._TaskID = GetNextTasksID();
		TaskID NewTasklID = Ret._TaskID;
		Ret.Set_Future(std::move(r));
		_TaskLock.unlock();

		FuncPtr NewPtr = [NewTasklID,
			task = std::move(task_promise)]()
		{
			//(*task)(MoveParPack(Pars,parsv));
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
		, const Vector<TaskID>& TaskDependencies,MovedParsParam(Pars) pars)
	{
		Delegate<AsynNonVoidType, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return AsynNonVoidValue;
		};

		return AddTask_t<AsynNonVoidType, Pars...>(TaskType,std::move(Func), TaskDependencies, MoveParPack(Pars,pars));
	}

	inline static BookOfThreads* Get_Threads()
	{
		return Threads;
	}

	
	void RunOnOnMainThread(FuncPtr&& Func, const Vector<TaskID>& TaskDependencies = {});
	

	template <typename R, typename... Pars>
	AsynTask_t<R> RunOnOnMainThread(Delegate<R, Pars...>&& task_function, const Vector<TaskID>& TaskDependencies,MovedParsParam(Pars) pars)
	{
		return AddTask_t<R, Pars...>(MainThread, std::move(task_function), TaskDependencies, MoveParPack(Pars,pars));
	}

	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> RunOnOnMainThread(Delegate<void, Pars...>&& task_function, const Vector<TaskID>& TaskDependencies,MovedParsParam(Pars) pars)
	{
		return AddTask_t<NonVoidType>(MainThreadID, std::move(task_function), TaskDependencies, MoveParPack(Pars, pars));
	}

	static bool IsOnMainThread();
	static void ThrowErrIfNotOnMainThread();
	inline static BookOfThreads* Threads = nullptr;

	static bool IsTasksDone(const Vector<TaskID>& Tasks);
	static bool IsTasksDone(TaskID Task);

	//Only for AsynTask
	

	static void AddCallDone(TaskID ID, RuningTaskData&& Info)
	{
		Info.ID = ID;

		_DoneLock.lock();
		_Map.AddValue(ID, Info);
		_DoneLock.unlock();
	}
	static void SetTaskData_Cancel(TaskID ID, Delegate<void>&& Value, ThreadToRunID Thread)
	{
		_DoneLock.lock();
		auto& V = _Map[ID];
		V.OnCancel = std::move(Value);
		V.OnCancelToRun = Thread;
		_DoneLock.unlock();
	}
	static void SetTaskData_Done(TaskID ID, AnyDoneFuncPtr&& Value, ThreadToRunID Thread)
	{
		_DoneLock.lock();
		auto& V = _Map[ID];
		V.OnDone = std::move(Value);
		V.OnDoneToRun = Thread;
		_DoneLock.unlock();
	}
	static bool HasOnDone(TaskID ID)
	{
		_DoneLock.lock();
		bool r = false;
		if (_Map.HasValue(ID))
		{
			r = _Map.at(ID).OnDoneToRun != NullThread;
		}
		_DoneLock.unlock();

		return r;
	}
	static bool HasOnCancel(TaskID ID)
	{
		_DoneLock.lock();
		bool r = false;
		if (_Map.HasValue(ID))
		{
			r = _Map.at(ID).OnCancelToRun != NullThread;
		}
		_DoneLock.unlock();

		return r;
	}


	static void TryCallCancel(TaskID ID)
	{
		_DoneLock.lock();
		if (_Map.HasValue(ID))
		{
			auto& V = _Map.at(ID);

			if (V.OnCancel)
			{
				if (V.ThreadToRun == CurrentThreadInfo::CurrentThread)
				{
					V.OnCancel();

					_Map.erase(ID);
					_DoneLock.unlock();
					return;
				}
				else
				{
					Delegate<void>  Func = [OnDone = std::make_shared<RuningTaskData::Cancel>(std::move(V.OnCancel))]() mutable
					{
						_DoneLock.lock();
						(*OnDone)();
						_DoneLock.unlock();
					};

					_Map.erase(ID);

					BookOfThreads::Threads->AddTask_t(
						V.ThreadToRun,
						std::move(Func),
						{}
					);

				}
			}
		}
		_DoneLock.unlock();
	}
	template<typename T>
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
					auto& done = (V.OnDone);
					auto& donefunc = (*(OnDoneFuncPtr<T>*) & done);
					donefunc(std::move(Move));
					
					_DoneLock.unlock();
					return;
				}
				else
				{
					Delegate<void>  Func = [OnDone = std::make_shared<AnyDoneFuncPtr>(std::move(V.OnDone)),
						Val = std::make_shared<T>(std::move(Move))]() mutable
					{
						auto& done = (*OnDone);
						_DoneLock.lock();
						(*(OnDoneFuncPtr<T>*)&done)(std::move(*Val));
						_DoneLock.unlock();
					};

					BookOfThreads::Threads->AddTask_t(
						V.ThreadToRun,
						std::move(Func),
						{}
					);
				}
			}
		}
		_DoneLock.unlock();
	}
	
	inline static std::mutex _DoneLock;
	inline static BinaryVectorMap<TaskID, RuningTaskData> _Map;
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
	
	TaskID _TaskID;
	 
	TaskID GetNextTasksID()
	{
		auto R = _TaskID;
		_TaskID = TaskID(R.Get_Base() + 1);

		return R;
	}

	ThreadData& GetThreadInfo(ThreadToRunID ID);
	ThreadData& GetThreadNoneWorkingThread();
	


	


	
	
	

};
CoreEnd
