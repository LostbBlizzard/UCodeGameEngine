#pragma once

#include "../Core/GameRunTime.hpp"
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include <mutex>
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

	TaskID_t Get_Base() const
	{
		return Base;
	}
private:
	TaskID_t Base;
};
CoreEnd
namespace std {

	template <>
	struct hash<UCode::TaskID>
	{
		std::size_t operator()(const UCode::TaskID& k) const
		{
			using std::hash;
			return ((hash<UCode::TaskID_t>()(k.Get_Base())));
		}
	};

}
CoreStart
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

	struct FutureBlock
	{
		virtual ~FutureBlock()
		{

		}
		virtual void* GetFuture() = 0;
	};
	std::shared_ptr<FutureBlock> future;
	TaskID ID =NullTaskID;


	//Thread the Task is meant the run on.
	ThreadToRunID ThreadToRun = AnyThread;

	template<typename T>
	void SetFuture(std::future<T>&& Vale)
	{
		struct MyFuture :FutureBlock
		{
			std::future<T> my;
			MyFuture()
			{

			}
			MyFuture(std::future<T>&& V)
				:my(std::move(V))
			{
				
			}
			void* GetFuture() override
			{
				return &my;
			}
		};
		future = std::make_shared<MyFuture>(std::move(Vale));
	}

	template<typename T>
	std::future<T>&  GetFuture()
	{
		return *(std::future<T>*)future.get()->GetFuture();
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


//Because incomplete types
void Asyn_Map_Erase(TaskID id);
size_t Asyn_Map_Count(TaskID id);
void Asyn_LockDoneUnlock();
void Asyn_LockDoneLock();
TaskProgress Asyn_GetProgress(TaskID id);
bool Asyn_HasOnDone(TaskID id);
bool Asyn_IsDoneLocked();

template<typename T>
void Asyn_SetFuture(TaskID id,std::future<T>&& val);

template<typename T>
std::future<T>& Asyn_GetFuture(TaskID id);

void Asyn_SetTaskData_Cancel(TaskID id,Delegate<void>&& Value, ThreadToRunID Thread);
void Asyn_SetTaskData_Done(TaskID id,AnyDoneFuncPtr&& done,ThreadToRunID Thread);

void Asyn_TryCallCancel(TaskID id);

void Asyn_RunOnOnMainThread(Delegate<void>&& Func);


template<typename T>
bool Asyn_TryCallOnDone(TaskID id, T&& Move);

template<typename T>
struct AsynTask_t;

template<typename T2>
AsynTask_t<T2> Asyn_AddTask_t(ThreadToRunID Thread,Delegate<T2>&& Func,const Vector<TaskID>& Deps);

template<typename T>
struct AsynTask_t
{
	friend Threads;
	using ThisType = AsynTask_t<T>;
	using OnDoneFuncPtr = Delegate<void,T>;
	using Future = std::future<T>;

	template<typename T2> using Continue = Delegate<T2,T&&>;
	template<typename T2> using ContinueRet = AsynTask_t<T2>;
	using Cancel = Delegate<void>;
	using SelfRet = ThisType&&;
	
	AsynTask_t(Future&& Item) noexcept
	{
		Set_Future(std::move(Item));
	}
	AsynTask_t() noexcept
	{
	
	}
	AsynTask_t(ThisType&& Other) noexcept:
		  _TaskID(Other._TaskID)
	{
		Other._TaskID = NullTaskID;
	}

	~AsynTask_t()
	{
		
	}
	ThisType& operator=(Future&& Item) noexcept
	{
		Set_Future(Item);
		return *this;
	}
	ThisType& operator=(ThisType&& Other) noexcept
	{
		_TaskID = Other._TaskID;
		Other._TaskID = NullTaskID;
		return *this;
	}

	TaskProgress GetProgress()
	{
		return Asyn_GetProgress(_TaskID);
	}

	//Calls this Funcion on the same Thread it was made on.
	SelfRet OnCompletedCurrentThread(OnDoneFuncPtr&& Value)
	{
		return OnCompletedOnThread(std::move(Value),CurrentThreadInfo::CurrentThread);
	}
	SelfRet OnCompletedOnMainThread(OnDoneFuncPtr&& Value)
	{
		return OnCompletedOnThread(std::move(Value), MainThread);
	}

	SelfRet OnCompletedOnAnyThread(OnDoneFuncPtr&& Value)
	{
		return  OnCompletedOnThread(std::move(Value), AnyThread);
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



		std::shared_ptr<T> v =std::make_shared<T>();
		Delegate<void,T> FuncDone = [v](T&& Val)
		{
			*v.get() = std::move(Val);
		};

		SetTaskData_OnDone(_TaskID,std::move(FuncDone), Thread);

		Delegate<T2> Func = [Value = std::move(Value), v]()
		{
			auto& Val = *v.get();
			return Value(std::move(Val));
		};
		return Asyn_AddTask_t<T2>(Thread,std::move(Func),Deps);
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
		return OnCancelOnThread(std::move(Value),CurrentThreadInfo::CurrentThread);
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
	Optional<T> TryGet_Op()
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
		return  Get_Future().wait();
	}
	bool IsDone()
	{
		using namespace std::chrono_literals;
		return  Get_Future().wait_for(0s) == std::future_status::ready;
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
		Asyn_LockDoneLock();
		if (ID != NullTaskID)
		{
			if (Asyn_Map_Count(ID)) {
				Asyn_Map_Erase(ID);
			}
		}
		Asyn_LockDoneUnlock();
	}

	void SetValue(T&& Value)
	{
		Delegate<void> Func = [_TaskID = _TaskID, Value = std::move(Value)]() mutable
		{
			TryCallOnDone(_TaskID,std::move(Value));
		};
		Asyn_RunOnOnMainThread(std::move(Func));
	}
	
private:
	TaskID _TaskID = NullTaskID;
	static bool TryCallOnDone(TaskID ID, T&& Move)
	{
		return Asyn_TryCallOnDone(ID, std::move(Move));
	}
	static void TryCallCancel(TaskID ID)
	{
		Asyn_TryCallCancel(ID);
	}

	static void SetTaskData_OnDone(TaskID ID,OnDoneFuncPtr&& Value,ThreadToRunID Thread)
	{
		Asyn_SetTaskData_Done(ID, std::move(*(AnyDoneFuncPtr*)&Value),Thread);
	}
	static void SetTaskData_OnCancel(TaskID ID,Cancel&& Value, ThreadToRunID Thread)
	{
		Asyn_SetTaskData_Cancel(ID, std::move(Value), Thread);
	}

	static bool HasOnDone(TaskID ID)
	{
		return Asyn_HasOnDone(ID);
	}
	Future& Get_Future()
	{
		return  Get_Future(_TaskID);
	}
	void Set_Future(Future&& Value)
	{
		bool isdonel = Asyn_IsDoneLocked();

		if (!isdonel) {
			Asyn_LockDoneLock();
		}
		Asyn_SetFuture(_TaskID,std::move(Value));

		if (!isdonel) {
			Asyn_LockDoneUnlock();
		}
	}

	static Future& Get_Future(TaskID _TaskID)
	{
		bool isdonel = Asyn_IsDoneLocked();

		if (!isdonel) {
			Asyn_LockDoneLock();
		}
		
		auto& R = Asyn_GetFuture<T>(_TaskID);
		
		if (!isdonel) {
			Asyn_LockDoneUnlock();
		}
		return R;
	}
};
using AsynTask = AsynTask_t<AsynNonVoidType>;

class Threads :private System
{
public:
	
	using FuncPtr = Delegate<void>;

	template<typename T> using OnDoneFuncPtr = Delegate<void, T>;
	//#define MoveParPack(Pack_t,P) std::forward<##Pack_t&&>(##P)...
	//#define MovedParsParam(Pars) Pars##&&...
	//#define MoveParPack(Pack_t,P) P...
	//#define MoveParPack2(Pack_t, P) std::forward<##Pack_t&&>(##P)...

	#define MoveParPack(Pack_t,P) std::forward< Pack_t >( P ) ...	
	#define MovedParsParam(Pars) Pars ...



	struct TaskInfo
	{
		std::shared_ptr<FuncPtr> _Func;
		Vector<TaskID> TaskDependencies;
		TaskID taskID = NullTaskID;
	};

	static Threads* Get(Gamelibrary* lib);
	static Threads* Find(const Gamelibrary* lib);
	
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

		return AddTask_t<AsynNonVoidType,Pars...>(TaskType,std::move(Func), TaskDependencies, MoveParPack(Pars, pars));
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
		auto task_promise = std::make_shared<PackagedTask<R(Pars...)>>(task_function);

		auto r = task_promise->get_future();
		AsynTask_t<R> Ret;

		_TaskLock.lock();
		Ret._TaskID = GetNextTasksID();
		TaskID NewTasklID = Ret._TaskID;
		_TaskLock.unlock();

		Ret.Set_Future(std::move(r));

		FuncPtr NewPtr = [NewTasklID,
			task = std::move(task_promise),
			parsV = std::make_tuple<Pars...>(MoveParPack(Pars,pars))]() mutable
		{
			std::apply([&](auto&&... args)
			{
				(*task)(args...);

			},std::move(parsV));

			
			if (AsynTask_t<R>::HasOnDone(NewTasklID))
			{
				auto Val = AsynTask_t<R>::Get_Future(NewTasklID).get();
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

	inline static Threads* Get_Threads()
	{
		return _GlobalThreads;
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
		return AddTask_t<AsynNonVoidType>(MainThread, std::move(task_function), TaskDependencies, MoveParPack(Pars, pars));
	}

	static bool IsOnMainThread();
	static void ThrowErrIfNotOnMainThread();
	inline static Threads* _GlobalThreads = nullptr;

	static bool IsTasksDone(const Vector<TaskID>& Tasks);
	static bool IsTasksDone(TaskID Task);

	//Only for AsynTask
	
	inline static bool _IsDoneLocked = false;
	static bool IsDoneLocked()
	{
		return _IsDoneLocked;
	}
	static void LockDoneLock()
	{
		_DoneLock.lock(); 
		_IsDoneLocked = true;
	}
	static void LockDoneUnLock()
	{
		_IsDoneLocked = false;
		_DoneLock.unlock();
	}

	static void AddCallDone(TaskID ID, RuningTaskData&& Info)
	{
		Info.ID = ID;

		LockDoneLock();
		_Map[ID] = Info;
		LockDoneUnLock();
	}
	static void SetTaskData_Cancel(TaskID ID, Delegate<void>&& Value, ThreadToRunID Thread)
	{
		LockDoneLock();
		auto& V = _Map[ID];
		V.OnCancel = std::move(Value);
		V.OnCancelToRun = Thread;
		LockDoneUnLock();
	}
	static void SetTaskData_Done(TaskID ID, AnyDoneFuncPtr&& Value, ThreadToRunID Thread)
	{
		LockDoneLock();
		auto& V = _Map[ID];
		V.OnDone = std::move(Value);
		V.OnDoneToRun = Thread;
		LockDoneUnLock();
	}
	static bool HasOnDone(TaskID ID)
	{
		LockDoneLock();
		bool r = false;
		if (_Map.count(ID))
		{
			r = _Map.at(ID).OnDoneToRun != NullThread;
		}
		LockDoneUnLock();

		return r;
	}
	static bool HasOnCancel(TaskID ID)
	{
		LockDoneLock();
		bool r = false;
		if (_Map.count(ID))
		{
			r = _Map.at(ID).OnCancelToRun != NullThread;
		}
		LockDoneUnLock();

		return r;
	}


	static void TryCallCancel(TaskID ID)
	{
		LockDoneLock();
		if (_Map.count(ID))
		{
			auto& V = _Map.at(ID);

			if (V.OnCancel)
			{
				if (V.ThreadToRun == CurrentThreadInfo::CurrentThread)
				{
					V.OnCancel();

					_Map.erase(ID);
					LockDoneUnLock();
					return;
				}
				else
				{
					Delegate<void>  Func = [OnDone = std::make_shared<RuningTaskData::Cancel>(std::move(V.OnCancel))]() mutable
					{
						LockDoneLock();
						(*OnDone)();
						LockDoneUnLock();
					};

					_Map.erase(ID);

					Threads::_GlobalThreads->AddTask_t(
						V.ThreadToRun,
						std::move(Func),
						{}
					);

				}
			}
		}
		LockDoneUnLock();
	}
	template<typename T>
	static bool TryCallOnDone(TaskID ID, T&& Move)
	{
		LockDoneLock();
		if (_Map.count(ID))
		{
			auto& V = _Map.at(ID);

			if (V.OnDone)
			{
				if (V.ThreadToRun == CurrentThreadInfo::CurrentThread)
				{
					auto& done = (V.OnDone);
					auto& donefunc = (*(OnDoneFuncPtr<T>*) & done);
					donefunc(std::move(Move));
					
					LockDoneUnLock();
					return false;
				}
				else
				{
					Delegate<void>  Func = [OnDone = std::make_shared<AnyDoneFuncPtr>(std::move(V.OnDone)),
						Val = std::make_shared<T>(std::move(Move))
					  ,ID]() mutable
					{
						auto& done = (*OnDone);
						LockDoneLock();
						(*(OnDoneFuncPtr<T>*)&done)(std::move(*Val));
						_Map.erase(ID);
						LockDoneUnLock();
					};

					Threads::Get_Threads()->AddTask_t(
						V.ThreadToRun,
						std::move(Func),
						{}
					);

					LockDoneUnLock();
					return true;
				}
			}
		}
		LockDoneUnLock();
		return false;
	}
	
	inline static std::mutex _DoneLock;
	inline static UnorderedMap<TaskID, RuningTaskData> _Map;

	static TaskProgress GetProgress(TaskID ID,bool ShouldLock =true)
	{
		TaskProgress R;

		if (ShouldLock) {
			Get_Threads()->_OnMainThreadLock.lock();
			Get_Threads()->_TaskLock.lock();
		}

		if (Threads::_Map.count(ID))
		{
			TaskInfo* V = Get_Threads()->GetTaskInfo(ID);
			R.DoneTasks = 0;
			R.TotalTasks = 1;
			if (V)
			{
				for (auto& Item : V->TaskDependencies)
				{
					auto v = GetProgress(Item,false);
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

		if (ShouldLock) {
			Get_Threads()->_OnMainThreadLock.unlock();
			Get_Threads()->_TaskLock.unlock();
		}

		return R;
	}
private:
	TaskInfo* GetTaskInfo(TaskID ID)
	{
		for (auto& Item : _MainThreadData._TaskToDo)
		{
			if (Item.taskID == ID)
			{
				return &Item;
			}
		}

		for (auto& Thr : _Threads)
		{
			for (auto& Item : Thr._Data._TaskToDo)
			{
				if (Item.taskID == ID)
				{
					return &Item;
				}
			}
		}

		return nullptr;
	}
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

	Threads(Gamelibrary* lib);
	~Threads();
	static void ThreadLoop(Threads* _This, ThreadInfo* Info);



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


template<typename T2>
AsynTask_t<T2> Asyn_AddTask_t(ThreadToRunID Thread,Delegate<T2>&& Func,const Vector<TaskID>& Deps)
{
	return Threads::Get_Threads()->AddTask_t<T2>(Thread,std::move(Func),Deps);
}

inline void Asyn_RunOnOnMainThread(Delegate<void>&& Func)
{
	Threads::Get_Threads()->RunOnOnMainThread(std::move(Func));
}

template<typename T>
bool Asyn_TryCallOnDone(TaskID id, T&& Move)
{
	return Threads::TryCallOnDone<T>(id, std::move(Move));
}

inline void Asyn_TryCallCancel(TaskID id)
{
	Threads::TryCallCancel(id);
}
inline void Asyn_SetTaskData_Done(TaskID id,AnyDoneFuncPtr&& done,ThreadToRunID Thread)
{
	Threads::SetTaskData_Done(id,std::move(done),Thread);
}

inline void SetTaskData_Cancel(TaskID id,Delegate<void>&& Value, ThreadToRunID Thread)
{
	Threads::SetTaskData_Cancel(id, std::move(Value), Thread);
}

template<typename T>
void Asyn_SetFuture(TaskID id,std::future<T>&& val)
{
	Threads::_Map[id].SetFuture(std::move(val));
}

template<typename T>
std::future<T>& Asyn_GetFuture(TaskID id)
{
	return Threads::_Map[id].GetFuture<T>();
}
inline bool Asyn_IsDoneLocked()
{
	return Threads::IsDoneLocked();
}
inline bool Asyn_HasOnDone(TaskID id)
{
	return Threads::HasOnDone(id);
}
inline size_t Asyn_Map_Count(TaskID id)
{
	return Threads::_Map.count(id);
}

inline void Asyn_Map_Erase(TaskID id)
{
	Threads::_Map.erase(id);
}
inline void Asyn_LockDoneLock()
{
	Threads::LockDoneLock();
}
inline void Asyn_LockDoneUnlock()
{
	Threads::LockDoneUnLock();
}
inline TaskProgress Asyn_GetProgress(TaskID id)
{
	return Threads::GetProgress(id);
}
inline void Asyn_SetTaskData_Cancel(TaskID id,Delegate<void>&& Value, ThreadToRunID Thread)
{
	Threads::SetTaskData_Cancel(id, std::move(Value), Thread);
}
CoreEnd


