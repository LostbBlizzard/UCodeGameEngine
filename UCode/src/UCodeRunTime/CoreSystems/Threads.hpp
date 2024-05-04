#pragma once

#include "../Core/GameRunTime.hpp"
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include "UCodeRunTime/BasicTypes/Mutex.hpp"
#include <thread>
#include <functional>
#include <condition_variable>
#include <future>
#include <chrono>
#include "PackagedTask.hpp"
CoreStart


using ThreadToRunID_t = int;
struct ThreadToRunID
{
	static constexpr ThreadToRunID_t badid = -3;
	constexpr ThreadToRunID()
		:Base(badid)
	{
	}
	constexpr explicit ThreadToRunID(ThreadToRunID_t Value)
		: Base(Value)
	{
	}

	bool operator==(const ThreadToRunID &Other) const
	{
		return this->Base == Other.Base;
	}
	bool operator!=(const ThreadToRunID &Other) const
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
	static constexpr TaskID_t badid = -1;
	TaskID()
		: Base(badid)
	{
	}

	constexpr explicit TaskID(TaskID_t Value)
		: Base(Value)
	{
	}
	bool operator==(const TaskID &Other) const
	{
		return this->Base == Other.Base;
	}
	bool operator!=(const TaskID &Other) const
	{
		return this->Base != Other.Base;
	}

	TaskID_t Get_Base() const
	{
		return Base;
	}

private:
	TaskID_t Base;
};
CoreEnd namespace std
{

	template <>
	struct hash<UCode::TaskID>
	{
		std::size_t operator()(const UCode::TaskID &k) const
		{
			using std::hash;
			return ((hash<UCode::TaskID_t>()(k.Get_Base())));
		}
	};
}
CoreStart struct AsynNonVoidType
{
};
static constexpr AsynNonVoidType AsynNonVoidValue = AsynNonVoidType();

static constexpr ThreadToRunID AnyThread = ThreadToRunID(-1);
static constexpr ThreadToRunID MainThread = ThreadToRunID(-2);
static constexpr ThreadToRunID NullThread = ThreadToRunID(ThreadToRunID::badid);
static constexpr TaskID NullTaskID = TaskID(TaskID::badid);

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

// use c-cast
using AnyFuture = std::future<AsynNonVoidType>;
using AnyDoneFuncPtr = Delegate<void, AsynNonVoidType>;

struct RuningTaskData
{
	struct FutureBlock
	{
		virtual ~FutureBlock()
		{
		}
		virtual void *GetFuture() = 0;
	};
	using Cancel = Delegate<void>;

	TaskID ID = NullTaskID;
	std::shared_ptr<FutureBlock> future;
	
	AnyDoneFuncPtr OnDone;
	Cancel OnCancel;

	ThreadToRunID OnDoneToRun = NullThread;
	ThreadToRunID OnCancelToRun = NullThread;	
	ThreadToRunID ThreadToRun= NullThread;

	bool CanStartTask = false;
	
	

	template <typename T>
	void SetFuture(std::future<T> &&Vale)
	{
		struct MyFuture : FutureBlock
		{
			std::future<T> my;
			MyFuture()
			{
			}
			MyFuture(std::future<T> &&V)
				: my(std::move(V))
			{
			}
			void *GetFuture() override
			{
				return &my;
			}
		};
		future = std::make_shared<MyFuture>(std::move(Vale));
	}

	template <typename T>
	std::future<T> &GetFuture()
	{
		return *(std::future<T> *)future.get()->GetFuture();
	}

private:
};

class Threads;

struct TaskProgress
{
	size_t DoneTasks = 0;
	size_t TotalTasks = 0;

	float AsPercent() const
	{
		return (float)DoneTasks / (float)TotalTasks;
	}
};
struct TaskInfo
{
	std::shared_ptr<Delegate<void>> _Func;
	Vector<TaskID> TaskDependencies;
	TaskID taskID = NullTaskID;
};
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
		: _ThreadID(id)
	{
		_NewTask = std::make_shared<std::condition_variable>();
	}
};
struct MainThreadTaskData
{
	ThreadData _MainThreadData;
	Vector<TaskInfo> _TaskToReAddOnToMainThread;
};
struct MainTaskData
{
	//AnyThradInfo
	ThreadData _AnyThread;
	TaskID _TaskID;
	ThreadToRunID _NextLockKey;

	TaskID GetNextTasksID()
	{
		auto R = _TaskID;
		_TaskID = TaskID(R.Get_Base() + 1);

		return R;
	}
};
struct RuningTaskDataList
{
	UnorderedMap<TaskID, RuningTaskData> _RuningTasks;
	MainTaskData TaskData;
};
// Because incomplete types
void Asyn_Map_Erase(TaskID id);
bool Asyn_HasOnDone(TaskID id);
TaskProgress Asyn_GetProgress(TaskID id);
bool Asyn_RuningTasks_HasValue(TaskID id);

template <typename T>
void Asyn_SetFuture(TaskID id, std::future<T> &&val);

template <typename T>
void Asyn_GetFuture(TaskID id, std::function<void(std::future<T>&)> func);

void Asyn_SetTaskData_Cancel(TaskID id, Delegate<void> &&Value, ThreadToRunID Thread);
void Asyn_SetTaskData_Done(TaskID id, AnyDoneFuncPtr &&done, ThreadToRunID Thread);

void Asyn_TryCallCancel(TaskID id);

void Asyn_RunOnOnMainThread(Delegate<void> &&Func);

template <typename T>
bool Asyn_TryCallOnDone(TaskID id, T &&Move);
void Asyn_StartTask(TaskID id);

bool Asyn_IsDone(TaskID id);

template <typename T>
struct AsynTask_t;

template <typename T2>
AsynTask_t<T2> Asyn_AddTask_t(ThreadToRunID Thread, Delegate<T2> &&Func, const Vector<TaskID> &Deps);


template <typename T>
struct AsynTask_t
{
	friend Threads;
	using ThisType = AsynTask_t<T>;
	using OnDoneFuncPtr = Delegate<void, T>;
	using Future = std::future<T>;

	template <typename T2>
	using Continue = Delegate<T2, T &&>;
	template <typename T2>
	using ContinueRet = AsynTask_t<T2>;
	using Cancel = Delegate<void>;
	using SelfRet = ThisType &&;

	AsynTask_t() noexcept
	{
	}
	AsynTask_t(ThisType &&Other) noexcept : _TaskID(Other._TaskID)
	{
		Other._TaskID = NullTaskID;
	}
	ThisType &operator=(ThisType &&Other) noexcept
	{
		_TaskID = Other._TaskID;
		Other._TaskID = NullTaskID;
		return *this;
	}
	~AsynTask_t()
	{
		if (_TaskID != NullTaskID)
		{
			//DestructTask();	
		}
	}
	TaskID FreeCallID()
	{
		auto v = _TaskID;
		_TaskID = NullTaskID;
		return v;
	}
	
	
	void DestructTask()
	{	
		if (!Asyn_HasOnDone(_TaskID))
		{
			RemoveCallID(_TaskID);
		}
	}
	
	void DestructTask(RuningTaskDataList& List)
	{	
		if (!Asyn_HasOnDone(_TaskID,List))
		{
			RemoveCallID(_TaskID,List);
		}
	}

	/*
	AsynTask_t(Future&& Item) noexcept
	{
		Set_Future(std::move(Item));
	}
	ThisType& operator=(Future&& Item) noexcept
	{
		Set_Future(Item);
		return *this;
	}
	*/

	AsynTask_t(const ThisType &Other) = delete;
	ThisType &operator=(const ThisType &Other) = delete;

	TaskProgress GetProgress()
	{
		return Asyn_GetProgress(_TaskID);
	}

	// Calls this Funcion on the same Thread it was made on.
	SelfRet OnCompletedCurrentThread(OnDoneFuncPtr &&Value)
	{
		return OnCompletedOnThread(std::move(Value), CurrentThreadInfo::CurrentThread);
	}
	SelfRet OnCompletedOnMainThread(OnDoneFuncPtr &&Value)
	{
		return OnCompletedOnThread(std::move(Value), MainThread);
	}

	SelfRet OnCompletedOnAnyThread(OnDoneFuncPtr &&Value)
	{
		return OnCompletedOnThread(std::move(Value), AnyThread);
	}

	SelfRet OnCompletedOnThread(OnDoneFuncPtr &&Value, TaskType task)
	{
		return OnCompletedOnThread(std::move(Value), GetThreadFromTask(task));
	}

	SelfRet OnCompletedOnThread(OnDoneFuncPtr &&Value, ThreadToRunID Thread)
	{
		SetTaskData_OnDone(_TaskID, std::move(Value), Thread);
		return std::move(*this);
	}

	template <typename T2>
	ContinueRet<T2> ContinueCurrentThread(Continue<T2> &&Value)
	{
		return ContinueOnThread<T2>(CurrentThreadInfo::CurrentThread, std::move(Value));
	}

	ContinueRet<AsynNonVoidType> ContinueCurrentThread(Continue<void> &&Value)
	{
		Delegate<AsynNonVoidType, T &&> Func = [Value = std::move(Value)](T &&Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueCurrentThread<AsynNonVoidType>(std::move(Func));
	}

	template <typename T2>
	ContinueRet<T2> ContinueTaskThread(Continue<T2> &&Value)
	{
		return ContinueOnThread<T2>(CurrentThreadInfo::CurrentThread, std::move(Value));
	}

	ContinueRet<AsynNonVoidType> ContinueTaskThread(Continue<void> &&Value)
	{
		Delegate<AsynNonVoidType, T &&> Func = [Value = std::move(Value)](T &&Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueTaskThread<AsynNonVoidType>(std::move(Func));
	}

	template <typename T2>
	ContinueRet<T2> ContinueOnMainThread(Continue<T2> &&Value)
	{
		return ContinueOnThread<T2>(MainThread, std::move(Value));
	}

	ContinueRet<AsynNonVoidType> ContinueOnMainThread(Continue<void> &&Value)
	{
		Delegate<AsynNonVoidType, T &&> Func = [Value = std::move(Value)](T &&Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueOnMainThread<AsynNonVoidType>(std::move(Func));
	}

	template <typename T2>
	ContinueRet<T2> ContinueOnAnyThread(Continue<T2> &&Value)
	{
		return ContinueOnThread<T2>(AnyThread, std::move(Value));
	}

	ContinueRet<AsynNonVoidType> ContinueOnAnyThread(Continue<void> &&Value)
	{
		Delegate<AsynNonVoidType, T &&> Func = [Value = std::move(Value)](T &&Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueOnAnyThread<AsynNonVoidType>(std::move(Func));
	}

	template <typename T2>
	ContinueRet<T2> ContinueOnThread(TaskType task, Continue<T2> &&Value)
	{
		return ContinueOnThread<T2>(GetThreadFromTask(task), std::move(Value));
	}

	ContinueRet<AsynNonVoidType> ContinueOnThread(TaskType task, Continue<void> &&Value)
	{
		Delegate<AsynNonVoidType, T &&> Func = [Value = std::move(Value)](T &&Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueOnThread<AsynNonVoidType>(task, std::move(Func));
	}

	template <typename T2>
	ContinueRet<T2> ContinueOnThread(ThreadToRunID Thread, Continue<T2> &&Value)
	{
		Vector<TaskID> Deps;
		Deps.push_back(_TaskID);

		std::shared_ptr<Optional<T>> v = std::make_shared<Optional<T>>();
		Delegate<void, T> FuncDone = [v](T &&Val)
		{
			*v.get() = std::move(Val);
		};

		SetTaskData_OnDone(_TaskID, std::move(FuncDone), Thread);
		Start();

		Delegate<T2> Func = [Value = std::move(Value), v]()
		{
			auto &Val = (*v.get()).value();
			return Value(std::move(Val));
		};
		return Asyn_AddTask_t<T2>(Thread, std::move(Func), Deps);
	}

	ContinueRet<AsynNonVoidType> ContinueOnThread(ThreadToRunID Thread, Continue<AsynNonVoidType> &&Value)
	{
		Delegate<AsynNonVoidType, T &&> Func = [Value = std::move(Value)](T &&Val)
		{
			Value(std::move(Val));
			return AsynNonVoidValue;
		};
		return ContinueOnThread<AsynNonVoidType>(Thread, std::move(Value));
	}

	SelfRet OnCancelCurrentThread(Cancel &&Value)
	{
		return OnCancelOnThread(std::move(Value), CurrentThreadInfo::CurrentThread);
	}
	SelfRet OnCancelOnMainThread(Cancel &&Value)
	{
		return OnCancelOnThread(std::move(Value), MainThread);
	}

	SelfRet OnCancelOnAnyThread(Cancel &&Value)
	{
		return OnCancelOnThread(std::move(Value), AnyThread);
	}

	SelfRet OnCancelOnThread(Cancel &&Value, TaskType task)
	{
		return OnCancelOnThread(std::move(Value), GetThreadFromTask(task));
	}
	SelfRet OnCancelOnThread(Cancel &&Value, ThreadToRunID Thread)
	{
		SetTaskData_OnCancel(_TaskID, std::move(Value), Thread);
		return std::move(*this);
	}
	SelfRet Start()
	{
		if (_TaskID != NullTaskID) 
		{
			Asyn_StartTask(_TaskID);
		}
		return std::move(*this);
	}

	T GetValue()
	{
		T r = {};

		Get_Future([&r](Future& val)
				{
					r = std::move(val.get());
				});

		return r;
	}
	T *TryGet()
	{
		if (IsDone())
		{
			return GetValue();
		}
		return nullptr;
	}
	Optional<T> TryGet_Op()
	{
		auto V = TryGet();
		if (V)
		{
			T r = std::move(*V);
			return r;
		}
		else
		{
			return {};
		}
	}
	bool valid()
	{
		if (Asyn_RuningTasks_HasValue(_TaskID))
		{
			bool r = {};
			Get_Future([&r](Future& val)
					{
						r = val.valid();
					});

			return r;
		}
		else
		{
			return false;
		}
	}
	void wait_for()
	{
		if (Asyn_RuningTasks_HasValue(_TaskID))
		{
			Future* p = nullptr;
			Get_Future([](Future& val)
					{
						p = &val;
					});
			//p->wait_for();
		}
	}
	void wait()
	{
		if (Asyn_RuningTasks_HasValue(_TaskID))
		{
			Future* p = nullptr;
			Get_Future([&p](Future& val)
				  {
					  p = &val;
				  });

			p->wait();
		}
	}
	bool IsDone()
	{
		using namespace std::chrono_literals;
		if (_TaskID == NullTaskID)
		{
			return false;
		}
		return Asyn_IsDone(_TaskID);
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
		if (ID != NullTaskID)
		{
			if (Asyn_RuningTasks_HasValue(ID))
			{
				Asyn_Map_Erase(ID);
			}
		}
	}
	static void RemoveCallID(TaskID ID,RuningTaskDataList& List)
	{
		if (ID != NullTaskID)
		{
			if (Asyn_RuningTasks_HasValue(ID,List))
			{
				Asyn_Map_Erase(ID,List);
			}
		}
	}

	void SetValue(T &&Value)
	{
		Delegate<void> Func = [_TaskID = _TaskID, Value = std::move(Value)]() mutable
		{
			TryCallOnDone(_TaskID, std::move(Value));
		};
		Asyn_RunOnOnMainThread(std::move(Func));
	}

private:
	TaskID _TaskID = NullTaskID;
	static bool TryCallOnDone(TaskID ID, T &&Move)
	{
		return Asyn_TryCallOnDone(ID, std::move(Move));
	}
	static void TryCallCancel(TaskID ID)
	{
		Asyn_TryCallCancel(ID);
	}

	static void SetTaskData_OnDone(TaskID ID, OnDoneFuncPtr &&Value, ThreadToRunID Thread)
	{
		Asyn_SetTaskData_Done(ID, std::move(*(AnyDoneFuncPtr *)&Value), Thread);
	}
	static void SetTaskData_OnCancel(TaskID ID, Cancel &&Value, ThreadToRunID Thread)
	{
		Asyn_SetTaskData_Cancel(ID, std::move(Value), Thread);
	}

	static bool HasOnDone(TaskID ID)
	{
		return Asyn_HasOnDone(ID);
	}
	void Get_Future(std::function<void(Future& val)> func)
	{
		return Get_Future(_TaskID,func);
	}
	void Set_Future(Future &&Value)
	{	
		Asyn_SetFuture(_TaskID, std::move(Value));
	}
	void Set_Future(Future&& Value,RuningTaskDataList& List)
	{
		Asyn_SetFuture(_TaskID, std::move(Value),List);
	}

	static void Get_Future(TaskID _TaskID,std::function<void(Future& val)> func)
	{
		Asyn_GetFuture<T>(_TaskID,func);	
	}
};
using AsynTask = AsynTask_t<AsynNonVoidType>;

class Threads : private System
{
public:
	using FuncPtr = Delegate<void>;

	template <typename T>
	using OnDoneFuncPtr = Delegate<void, T>;
	// #define MoveParPack(Pack_t,P) std::forward<##Pack_t&&>(##P)...
	// #define MovedParsParam(Pars) Pars##&&...
	// #define MoveParPack(Pack_t,P) P...
	// #define MoveParPack2(Pack_t, P) std::forward<##Pack_t&&>(##P)...

	#define MoveParPack(Pack_t, P) std::forward<Pack_t>(P)...
	#define MovedParsParam(Pars) Pars...

	

	static Threads *Get(Gamelibrary *lib);
	static Threads *Find(const Gamelibrary *lib);

	ThreadToRunID GetNewThreadLockKey(MainTaskData& taskdata);
	inline ThreadToRunID GetNewThreadLockKey()
	{
		return	RuningTasks.Lock_r<ThreadToRunID>([this](RuningTaskDataList& taskdata)
		{
			return GetNewThreadLockKey(taskdata.TaskData);
		});
	}


	AsynTask AddTask(ThreadToRunID thread, FuncPtr &&Func, const Vector<TaskID> &TaskDependencies,RuningTaskDataList& List);
	inline AsynTask AddTask(TaskType TaskType, FuncPtr &&Func, const Vector<TaskID> &TaskDependencies,RuningTaskDataList& List)
	{
		return AddTask(GetThreadFromTask(TaskType), std::move(Func), TaskDependencies,List);
	}
	
	
	
	inline AsynTask AddTask(ThreadToRunID thread, FuncPtr&& Func, const Vector<TaskID>& TaskDependencies)
	{
		return	RuningTasks.Lock_r<AsynTask>([thread,&Func,&TaskDependencies,this](RuningTaskDataList& taskdata)
		   {
			   return AddTask(thread, std::move(Func), TaskDependencies, taskdata);
		   });
	}
	
	inline AsynTask AddTask(TaskType TaskType, FuncPtr&& Func, const Vector<TaskID>& TaskDependencies)
	{
		return	RuningTasks.Lock_r<AsynTask>([TaskType, &Func, &TaskDependencies, this](RuningTaskDataList& taskdata)
		   {
			   return AddTask(TaskType, std::move(Func), TaskDependencies, taskdata);
		   });
	}
	
	

	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> AddTask_t(TaskType TaskType, Delegate<void, Pars...> task_function, const Vector<TaskID> &TaskDependencies, MovedParsParam(Pars) pars)
	{
		Delegate<AsynNonVoidType, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return AsynNonVoidValue;
		};

		return AddTask_t<AsynNonVoidType, Pars...>(TaskType, std::move(Func), TaskDependencies, MoveParPack(Pars, pars));
	}

	template <typename R, typename... Pars>
	AsynTask_t<R> AddTask_t(TaskType TaskType, Delegate<R, Pars...> &&task_function, const Vector<TaskID> &TaskDependencies, MovedParsParam(Pars) pars)
	{
		return AddTask_t<R, Pars...>(GetThreadFromTask(TaskType), std::move(task_function), TaskDependencies, MoveParPack(Pars, pars));
	}

	// Based From BS_thread_pool.hpp
	template <typename R, typename... Pars>
	AsynTask_t<R> AddTask_t(ThreadToRunID TaskType, Delegate<R, Pars...>&& task_function, const Vector<TaskID>& TaskDependencies,RuningTaskDataList& List, MovedParsParam(Pars) pars)
	{
		auto task_promise = std::make_shared<PackagedTask<R(Pars...)>>(task_function);

		auto r = task_promise->get_future();
		AsynTask_t<R> Ret;

		Ret._TaskID = List.TaskData.GetNextTasksID();
			
		TaskID NewTasklID = Ret._TaskID;

		Asyn_SetFuture(Ret._TaskID, std::move(r), List);

		FuncPtr NewPtr = [NewTasklID,
						  task = std::move(task_promise),
						  parsV = std::make_tuple<Pars...>(MoveParPack(Pars, pars))]() mutable
		{
			std::apply([&](auto &&...args)
					   { (*task)(args...); },
					   std::move(parsV));

			if (AsynTask_t<R>::HasOnDone(NewTasklID))
			{
				R Val = {};
				
				AsynTask_t<R>::Get_Future(NewTasklID,[&Val](typename AsynTask_t<R>::Future& future)
					{
						Val = std::move(future.get());
					});

				if (!AsynTask_t<R>::TryCallOnDone(NewTasklID, std::move(Val)))
				{
					AsynTask_t<R>::RemoveCallID(NewTasklID);
				}
			}
			else
			{

				AsynTask_t<R>::RemoveCallID(NewTasklID);
			}
		};

		AddTask(TaskType, std::move(NewPtr), TaskDependencies,List);
		return Ret;


	}
	template <typename R, typename... Pars>
	AsynTask_t<R> AddTask_t(ThreadToRunID TaskType, Delegate<R, Pars...> &&task_function, const Vector<TaskID> &TaskDependencies, MovedParsParam(Pars) pars)
	{
		return RuningTasks.Lock_r<AsynTask_t<R>>([&](RuningTaskDataList& List)
			{
				return AddTask_t<Pars...>(TaskType, std::move(task_function),TaskDependencies,List,MoveParPack(Pars, pars));
			});
	}

	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> AddTask_t(ThreadToRunID TaskType, Delegate<void, Pars...> &&task_function, const Vector<TaskID> &TaskDependencies,RuningTaskDataList& List, MovedParsParam(Pars) pars)
	{
		Delegate<AsynNonVoidType, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return AsynNonVoidValue;
		};

		return AddTask_t<AsynNonVoidType, Pars...>(TaskType, std::move(Func), TaskDependencies,List, MoveParPack(Pars, pars));
	}

	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> AddTask_t(ThreadToRunID TaskType, Delegate<void, Pars...> &&task_function, const Vector<TaskID> &TaskDependencies, MovedParsParam(Pars) pars)
	{
		Delegate<AsynNonVoidType, Pars...> Func = [task_function = std::move(task_function)]()
		{
			task_function();

			return AsynNonVoidValue;
		};

		return AddTask_t<AsynNonVoidType, Pars...>(TaskType, std::move(Func), TaskDependencies, MoveParPack(Pars, pars));	
	}
	
	inline static Threads *Get_Threads()
	{
		return _GlobalThreads;
	}

	void RunOnOnMainThread(FuncPtr &&Func, const Vector<TaskID> &TaskDependencies = {});

	template <typename R, typename... Pars>
	AsynTask_t<R> RunOnOnMainThread(Delegate<R, Pars...> &&task_function, const Vector<TaskID> &TaskDependencies, MovedParsParam(Pars) pars)
	{
		return AddTask_t<R, Pars...>(MainThread, std::move(task_function), TaskDependencies, MoveParPack(Pars, pars));
	}

	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> RunOnOnMainThread(Delegate<void, Pars...> &&task_function, const Vector<TaskID> &TaskDependencies, MovedParsParam(Pars) pars)
	{
		return AddTask_t(MainThread, std::move(task_function), TaskDependencies, MoveParPack(Pars, pars));
	}

	template <typename... Pars>
	AsynTask_t<AsynNonVoidType> RunOnOnMainThread(Delegate<void, Pars...> &&task_function, const Vector<TaskID> &TaskDependencies,RuningTaskDataList& List, MovedParsParam(Pars) pars)
	{
		return AddTask_t(MainThread, std::move(task_function), TaskDependencies, List, MoveParPack(Pars, pars));
	}

	static bool IsOnMainThread();
	static void ThrowErrIfNotOnMainThread();
	inline static Threads *_GlobalThreads = nullptr;

	bool IsTasksDone(const Vector<TaskID> &Tasks);
	bool IsTasksDone(TaskID Task);

	bool IsTasksDone(const Vector<TaskID> &Tasks,RuningTaskDataList& val);
	bool IsTasksDone(TaskID Task,RuningTaskDataList& val);
	// Only for AsynTask

	void StartTask(TaskID Task)
	{
		RuningTasks.Lock([this, Task](RuningTaskDataList& List)
					{
						StartTask(Task, List);
					});
	}
	
	void StartTask(TaskID Task,RuningTaskDataList& List)
	{
		auto& info = List._RuningTasks.GetValue(Task);
		info.CanStartTask = true;

		if (info.ThreadToRun != MainThread)
		{
			_Threads.Lock([&info](Vector<ThreadInfo>& val) 
			{
				val[info.ThreadToRun.Get_Base()]._NewTask->notify_one();
			});
		}
		else
		{

		}
	}
	void AddCallDone(TaskID ID, RuningTaskData&& Info)
	{
		RuningTasks.Lock([this,ID,&Info](RuningTaskDataList& List)
			{
				AddCallDone(ID,std::move(Info), List);
			});
	}
	void AddCallDone(TaskID ID, RuningTaskData &&Info,RuningTaskDataList& List)
	{
		Info.ID = ID;
		List._RuningTasks.GetValue(ID) = Info;
	}
	void SetTaskData_Cancel(TaskID ID, Delegate<void>&& Value, ThreadToRunID Thread)
	{
		RuningTasks.Lock([ID,this,&Value,Thread](RuningTaskDataList& val) 
			{
				SetTaskData_Cancel(ID,std::move(Value),Thread,val);
			});
	}
	void SetTaskData_Cancel(TaskID ID, Delegate<void> &&Value, ThreadToRunID Thread,RuningTaskDataList& List)
	{
		auto &V = List._RuningTasks.GetValue(ID);
		V.OnCancel = std::move(Value);
		V.OnCancelToRun = Thread;
	}
	void SetTaskData_Done(TaskID ID, AnyDoneFuncPtr&& Value, ThreadToRunID Thread)
	{
		RuningTasks.Lock([ID, this, &Value, Thread](RuningTaskDataList& val)
				{
					SetTaskData_Done(ID, std::move(Value), Thread,val);
				});
	}
	void SetTaskData_Done(TaskID ID, AnyDoneFuncPtr &&Value, ThreadToRunID Thread,RuningTaskDataList& List)
	{
		auto &V = List._RuningTasks.GetValue(ID);
		V.OnDone = std::move(Value);
		V.OnDoneToRun = Thread;
	}
	bool HasOnDone(TaskID ID)
	{
		return RuningTasks.Lock_r<bool>([this,ID](RuningTaskDataList& val) 
			{
				return HasOnDone(ID,val);
			});
	}
	bool HasOnDone(TaskID ID,RuningTaskDataList& List)
	{
		bool r = false;
		if (List._RuningTasks.HasValue(ID))
		{
			r = List._RuningTasks.GetValue(ID).OnDoneToRun != NullThread;
		}

		return r;
	}
	bool HasOnCancel(TaskID ID,RuningTaskDataList& List)
	{
		bool r = false;
		if (List._RuningTasks.HasValue(ID))
		{
			r = List._RuningTasks.GetValue(ID).OnCancelToRun != NullThread;
		}

		return r;
	}

	void TryCallCancel(TaskID ID)
	{
		RuningTasks.Lock([ID, this](RuningTaskDataList& val)
			   {
				   TryCallCancel(ID, val);
			   });
	}
	void TryCallCancel(TaskID ID,RuningTaskDataList& List)
	{
		Optional<AsynTask> tep;
		if (List._RuningTasks.HasValue(ID))
		{
			auto &V = List._RuningTasks.GetValue(ID);

			if (V.OnCancel)
			{
				if (V.OnCancelToRun == CurrentThreadInfo::CurrentThread)
				{
					V.OnCancel();

					List._RuningTasks.erase(ID);
					return;
				}
				else
				{
					Delegate<void> Func = [ID, OnDone = std::make_shared<RuningTaskData::Cancel>(std::move(V.OnCancel))]() mutable
						{
							(*OnDone)();

							Get_Threads()->RuningTasks.Lock([ID](RuningTaskDataList& List)
							{
										List._RuningTasks.erase(ID);
							});
						};

					tep = AddTask_t(
						V.OnCancelToRun,
						std::move(Func),
						{},List).Start();
				}
			}
		}
	}

	template <typename T>
	bool TryCallOnDone(TaskID ID, T&& Move)
	{
		return RuningTasks.Lock_r<bool>([this,ID, &Move](RuningTaskDataList& val)
				 {
					 return TryCallOnDone(ID, std::move(Move), val);
			     });
	}
	template <typename T>
	bool TryCallOnDone(TaskID ID, T &&Move,RuningTaskDataList& List)
	{
		if (List._RuningTasks.HasValue(ID))
		{
			auto &V = List._RuningTasks.GetValue(ID);

			if (V.OnDone)
			{
				if (V.OnDoneToRun == CurrentThreadInfo::CurrentThread)
				{
					auto &done = (V.OnDone);
					auto &donefunc = (*(OnDoneFuncPtr<T> *)&done);
					donefunc(std::move(Move));

					List._RuningTasks.erase(ID);
					return true;
				}
				else
				{
					Delegate<void> Func = [this,OnDone = std::make_shared<AnyDoneFuncPtr>(std::move(V.OnDone)),
						Val = std::make_shared<T>(std::move(Move)), ID]() mutable
						{
							auto& done = (*OnDone);
							(*(OnDoneFuncPtr<T>*) & done)(std::move(*Val));

							RuningTasks.Lock([ID](RuningTaskDataList& List)
							{
								List._RuningTasks.erase(ID);	
							});

							Notify_AllThreadsWhoDepens(ID);
						};

					auto tep = AddTask_t(
						V.OnDoneToRun,
						std::move(Func),
						{},List);
					
					auto taskid = tep.Get_TaskID();
					StartTask(taskid, List);
					//tep.DestructTask(List);
					tep.FreeCallID();
	
					
					return true;
				}
			}
		}
		return false;
	}

	
	Mutex<RuningTaskDataList> RuningTasks;
	
	
	TaskProgress GetProgress(TaskID ID)
	{
		return RuningTasks.Lock_r<TaskProgress>([this, ID](RuningTaskDataList& TaskList) -> TaskProgress
		{
			return MainThreadData.Lock_r<TaskProgress>([this, &TaskList, ID](MainThreadTaskData& Maintasks) -> TaskProgress
			{
				return _Threads.Lock_r<TaskProgress>([this, &Maintasks, &TaskList, ID](Vector<ThreadInfo>& Thread) -> TaskProgress
				{
					return GetProgress(ID, TaskList, Maintasks, Thread);
				});
			});
		});
	}

	TaskProgress GetProgress(TaskID ID,RuningTaskDataList& TaskList,MainThreadTaskData& Maintasks,Vector<ThreadInfo>& threads)
	{
		TaskProgress R;	

		if (TaskList._RuningTasks.HasValue(ID))
		{
			TaskInfo *V = GetTaskInfo(ID,Maintasks,threads);
			R.DoneTasks = 0;
			R.TotalTasks = 1;
			if (V)
			{
				for (auto &Item : V->TaskDependencies)
				{
					auto v = GetProgress(Item, TaskList,Maintasks,threads);
					R.DoneTasks += v.DoneTasks;
					R.TotalTasks += v.TotalTasks;
				}
			}
		}
		else
		{
			R.DoneTasks = 1;
			R.TotalTasks = 1;
		}

		return R;
	}

	void Notify_AllThreadsWhoDepens(TaskID id);
private:
	TaskInfo *GetTaskInfo(TaskID ID,MainThreadTaskData& Maintasks,Vector<ThreadInfo>& threads)
	{
		for (auto &Item : Maintasks._MainThreadData._TaskToDo)
		{
			if (Item.taskID == ID)
			{
				return &Item;
			}
		}

		for (auto &Thr : threads)
		{
			for (auto &Item : Thr._Data._TaskToDo)
			{
				if (Item.taskID == ID)
				{
					return &Item;
				}
			}
		}

		return nullptr;
	}
	
	void CallTaskToDoOnMainThread();
	void Update() override;

	Threads(Gamelibrary *lib);
	~Threads();
	static void ThreadLoop(Threads *_This, ThreadInfo *Info);

	Mutex<MainThreadTaskData> MainThreadData;

	Mutex<Vector<ThreadInfo>> _Threads;
	bool _EndThreads =false;
	
	void GetThreadInfo(ThreadToRunID ID,std::function<void(ThreadData&)> functocall);
	void GetThreadNoneWorkingThread(std::function<void(ThreadData&,ThreadToRunID I)> functocall);
};

template <typename T2>
AsynTask_t<T2> Asyn_AddTask_t(ThreadToRunID Thread, Delegate<T2> &&Func, const Vector<TaskID> &Deps)
{
	return Threads::Get_Threads()->AddTask_t<T2>(Thread, std::move(Func), Deps);
}

inline void Asyn_RunOnOnMainThread(Delegate<void> &&Func)
{
	Threads::Get_Threads()->RunOnOnMainThread(std::move(Func));
}

template <typename T>
bool Asyn_TryCallOnDone(TaskID id, T &&Move)
{
	return Threads::Get_Threads()->TryCallOnDone<T>(id, std::move(Move));
}

inline void Asyn_TryCallCancel(TaskID id)
{
	Threads::Get_Threads()->TryCallCancel(id);
}
inline void Asyn_SetTaskData_Done(TaskID id, AnyDoneFuncPtr &&done, ThreadToRunID Thread)
{
	Threads::Get_Threads()->SetTaskData_Done(id, std::move(done), Thread);
}

inline void SetTaskData_Cancel(TaskID id, Delegate<void> &&Value, ThreadToRunID Thread)
{
	Threads::Get_Threads()->SetTaskData_Cancel(id, std::move(Value), Thread);
}

template <typename T>
void Asyn_SetFuture(TaskID id, std::future<T>&& val, RuningTaskDataList& data)
{
	RuningTaskData run;
	run.ID = id;
	data._RuningTasks.GetOrAdd(id, std::move(run)).SetFuture(std::move(val));
}
template <typename T>
void Asyn_SetFuture(TaskID id, std::future<T> &&val)
{
	Threads::Get_Threads()->RuningTasks.Lock([id,&val](RuningTaskDataList& data)
		{
			Asyn_SetFuture(id, std::move(val), data);
		});
}

template <typename T>
void Asyn_GetFuture(TaskID id,std::function<void(std::future<T>&)> func)
{
	Threads::Get_Threads()->RuningTasks.Lock([id,&func](RuningTaskDataList& data)
		   {
			   func(data._RuningTasks.GetValue(id).GetFuture<T>());
		   });
}

inline bool Asyn_HasOnDone(TaskID id, RuningTaskDataList& val)
{
	return Threads::Get_Threads()->HasOnDone(id,val);
}
inline bool Asyn_HasOnDone(TaskID id)
{
	return Threads::Get_Threads()->HasOnDone(id);
}

inline bool Asyn_RuningTasks_HasValue(TaskID id, RuningTaskDataList& val)
{
	return val._RuningTasks.HasValue(id);
}
inline bool Asyn_RuningTasks_HasValue(TaskID id)
{
	return Threads::Get_Threads()->RuningTasks.Lock_r<bool>([id](RuningTaskDataList& val)
	{
		return Asyn_RuningTasks_HasValue(id,val);
	});
}
inline void Asyn_Map_Erase(TaskID id, RuningTaskDataList& val)
{
	val._RuningTasks.erase(id);
}
inline void Asyn_Map_Erase(TaskID id)
{
	Threads::Get_Threads()->RuningTasks.Lock([id](RuningTaskDataList& val)
	{
		Asyn_Map_Erase(id, val);
	});
}
inline void Asyn_StartTask(TaskID id)
{
	Threads::Get_Threads()->StartTask(id);
}
inline bool Asyn_IsDone(TaskID id)
{
	return Threads::Get_Threads()->IsTasksDone(id);
}

inline TaskProgress Asyn_GetProgress(TaskID id)
{
	return Threads::Get_Threads()->GetProgress(id);
}
inline void Asyn_SetTaskData_Cancel(TaskID id, Delegate<void> &&Value, ThreadToRunID Thread)
{
	Threads::Get_Threads()->SetTaskData_Cancel(id, std::move(Value), Thread);
}
CoreEnd
