#include "Threads.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart


constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::BookOfThreads;

static std::thread::id MainThreadID = {};
static ThreadToRunID ThreadCount = NullThread;
thread_local ThreadToRunID CurrentThreadInfo::CurrentThread = NullThread;

static u32 threads_count=0;

//To Do set threads on static
Threads::Threads(Gamelibrary* lib) :System(lib)
{

	TaskData.Lock([this](MainTaskData& val) 
	{
		val._NextLockKey = ThreadToRunID(1);
		val._TaskID = TaskID(0);
	});

	_GlobalThreads = this;
	ThreadCount = ThreadToRunID(0);

	MainThreadID = std::this_thread::get_id();

	CurrentThreadInfo::CurrentThread = MainThread;

	const u32 num_threads = std::thread::hardware_concurrency();
	threads_count = num_threads;
	_Threads.reserve(num_threads);
	
	for (u32 i = 0; i < num_threads; i++)
	{
		ThreadInfo* info = _Threads.data() + i;
		_Threads.push_back(ThreadInfo
		(
			ThreadToRunID(i)
		));

		info->thread.reset(new std::thread(ThreadLoop, this, info));
	}
	
}

Threads::~Threads()
{
	_EndThreads = true;
	_GlobalThreads = nullptr;
	
	//
		
	RuningTasks.Lock([this](RuningTaskDataList& list) {
	
		
		MainThreadData.Lock([this,&list](MainThreadTaskData& val)
		{
			for (auto& Item : val._MainThreadData._TaskToDo)
			{
				TryCallCancel(Item.taskID,list);
			}
			val._MainThreadData._TaskToDo.clear();
			for (auto& Item : val._TaskToReAddOnToMainThread)
			{
				TryCallCancel(Item.taskID, list);
			}
			val._TaskToReAddOnToMainThread.clear();
		});

	});

	//
	
	_NewTask.notify_all();
	for (size_t i = 0; i < _Threads.size(); i++)
	{
		auto& Item = _Threads[i];
		Item.thread->join();
	}
	_Threads.clear();
}
void Threads::ThreadLoop(Threads* _This,ThreadInfo* Info)
{
	CurrentThreadInfo::CurrentThread = Info->_ThreadID;

	while (true)
	{
		TaskInfo Task;
		{
			std::unique_lock<std::mutex> lock(_This->TaskData.Unsafe_GetLockBase());

			auto& MyTaskToDo = Info->_Data._TaskToDo;
			auto& AnyThreadTaskToDo = _This->TaskData.Unsafe_GetBaseType()._AnyThread._TaskToDo;

			_This->_NewTask.wait(lock, [_This, Info, MyTaskToDo, AnyThreadTaskToDo]
				{
					return !MyTaskToDo.empty()
						 || !AnyThreadTaskToDo.empty()
						 || _This->_EndThreads;
				}
			);

			if (_This->_EndThreads){break;}

			size_t TaskIndex = 0;
			TaskInfo* TaskToRun = nullptr;

			for (int i = MyTaskToDo.size() - 1; i >= 0; i--)
			{
				auto& Item = MyTaskToDo[i];

				
				if (_This->IsTasksDone(Item.TaskDependencies))
				{
					TaskToRun = &Item;
					TaskIndex = i;
					break;
				}
			}

			if (TaskToRun == nullptr) 
			{ 
				
				for (int i = AnyThreadTaskToDo.size() - 1; i >= 0; i--)
				{
					auto& Item = AnyThreadTaskToDo[i];


					if (_This->IsTasksDone(Item.TaskDependencies))
					{
						TaskToRun = &Item;
						TaskIndex = i;
						break;
					}
				}
				if (TaskToRun == nullptr) {
					continue;
				}
			}


			Task = std::move(*TaskToRun);
			MyTaskToDo.erase(MyTaskToDo.begin() + TaskIndex);

		}
		(*Task._Func)();
	}

	_This->RuningTasks.Lock([_This,Info](RuningTaskDataList& val) 
	{
		for (auto& Item : Info->_Data._TaskToDo)
		{
			_This->TryCallCancel(Item.taskID, val);
		}
	});
	Info->_Data._TaskToDo.clear();
}

void Threads::GetThreadInfo(ThreadToRunID ID,std::function<void(ThreadData&)> functocall)
{
	if (ID == MainThread)
	{
		MainThreadData.Lock([&functocall](MainThreadTaskData& data)
		{
				return functocall(data._MainThreadData);
		});
		TaskData.Lock([&functocall](MainTaskData& val)
			{
			});
	}
	else if (ID == AnyThread)
	{
		TaskData.Lock([&functocall](MainTaskData& val)
			   {
					  return functocall(val._AnyThread);
			   });
	}
	else
	{
		functocall(_Threads[ID.Get_Base()]._Data);
	}
}

void Threads::GetThreadNoneWorkingThread(std::function<void(ThreadData&)> functocall)
{
	size_t BestPixk = SIZE_MAX;
	Threads::ThreadData* Best = nullptr;
	for (auto& Item : _Threads)
	{
		if (Item._Data._TaskToDo.size() < BestPixk)
		{
			BestPixk = Item._Data._TaskToDo.size();
			Best = &Item._Data;
		}
	}
	functocall(*Best);
}

void Threads::CallTaskToDoOnMainThread()
{
	MainThreadData.Lock([this](MainThreadTaskData& val)
	{
		for (auto& Item : val._TaskToReAddOnToMainThread)
		{
			val._MainThreadData._TaskToDo.push_back(std::move(Item));
		}
		val._TaskToReAddOnToMainThread.clear();
		
		Vector<TaskInfo> CopyList = std::move(val._MainThreadData._TaskToDo);	

		/*
		for (auto i = CopyList.rbegin();i != CopyList.rend(); ++i)
		{

			auto& Item = *i;
		*/
		val._MainThreadData._TaskToDo = CopyList;//for GetProgress
		size_t V = val._MainThreadData._TaskToDo.size();

		for (auto& Item : CopyList)
		{	
			if (IsTasksDone(Item.TaskDependencies))
			{
				(*Item._Func)();
			}
			else
			{
				//_TaskToReAddOnToMainThread.push_back(std::move(Item));

				val._TaskToReAddOnToMainThread.push_back(std::move(Item));
			}
		}

		for (size_t i = 0; i < V; i++)
		{
			val._MainThreadData._TaskToDo.erase(val._MainThreadData._TaskToDo.begin());
		}
	});	
}

void Threads::Update()
{
	CallTaskToDoOnMainThread();
}

AsynTask Threads::AddTask(ThreadToRunID thread, FuncPtr&& Func, const Vector<TaskID>& TaskDependencies)
{
	//RunOnOnMainThread(std::move(Func), TaskDependencies);
	//return  AsynTask();

	if (_Threads.size() == 0 || _Threads.size() == 1)
	{ 
		Func();
		return AsynTask();
	}
	else
	{
		if (thread == MainThread)
		{
			RunOnOnMainThread(std::move(Func),TaskDependencies);
			return  AsynTask();
		}
		else if (thread == AnyThread)
		{
			TaskInfo Task;
			Task._Func =std::make_shared<FuncPtr>(Func);
			Task.TaskDependencies = TaskDependencies;
			

			auto taskid = TaskData.Lock_r<TaskID_t>([](Threads::MainTaskData& data)
				{
					return data._TaskID.Get_Base();

				});
			
			Task.taskID = TaskID(taskid-1);
			
			
			GetThreadNoneWorkingThread([Task](ThreadData& data)
				{
					data._TaskToDo.push_back(std::move(Task));
				});
			return  AsynTask();
		}
		else
		{
			TaskInfo Task;
			Task._Func = std::make_shared<FuncPtr>(Func);
			Task.TaskDependencies = TaskDependencies;

			auto taskid = TaskData.Lock_r<TaskID_t>([](Threads::MainTaskData& data)
					{
							return data._TaskID.Get_Base();
					});


			Task.taskID = TaskID(taskid - 1);
			GetThreadInfo(thread, [&Task](Threads::ThreadData& data)
				{
					data._TaskToDo.push_back(std::move(Task));
				});

		}
	}


	return AsynTask();
}
Threads* Threads::Get(Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	if (V) { return (Threads*)V; }

	Threads* r = new Threads(lib);;

	lib->MoveSystem(r);
	lib->SetStaticComponent(KeyIdex, r);
	return r;
}
Threads* Threads::Find(const Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	return (Threads*)V;
}

ThreadToRunID Threads::GetNewThreadLockKey(MainTaskData& taskdata)
{

	auto R = taskdata._NextLockKey;
	taskdata._NextLockKey = ThreadToRunID(R.Get_Base() + 1);
	
	return R;
}

void Threads::RunOnOnMainThread(FuncPtr&& Func, const Vector<TaskID>& TaskDependencies)
{

	MainThreadData.Lock([this,&Func,&TaskDependencies](MainThreadTaskData& val)
	{
		TaskData.Lock([&val,&Func,&TaskDependencies](MainTaskData& list)
		{
				val._MainThreadData._TaskToDo.push_back({ std::make_shared<FuncPtr>(std::move(Func)),TaskDependencies,TaskID(list._TaskID.Get_Base() - 1) });
		});
	});

}
 
bool Threads::IsOnMainThread()
{
	return MainThreadID == std::this_thread::get_id();
}

void Threads::ThrowErrIfNotOnMainThread()
{
	if (!IsOnMainThread())
	{
		UCodeGEThrow("Not on Main-Thread");
	}
}

bool Threads::IsTasksDone(const Vector<TaskID>& Tasks)
{
	return RuningTasks.Lock_r<bool>([&Tasks](RuningTaskDataList& val)
	{
		for (auto& Item : Tasks)
		{
			if (val._RuningTasks.HasValue(Item))
			{
				return false;
			}
		}
	
		return true;
	});
}

bool Threads::IsTasksDone(TaskID Task)
{
	Vector <TaskID> list; list.push_back(Task);
	return IsTasksDone(list);
}

ThreadToRunID GetThreadFromTask(TaskType type)
{
	if (type == TaskType::Main
		|| type == TaskType::Rendering
		|| type == TaskType::Physics
		|| type == TaskType::Audio)
	{
		return MainThread;
	}
	else if (type == TaskType::Generic) {
		return AnyThread;
	}
	else
	{
		TaskType_t TypeAsInt = (TaskType_t)type;
		ThreadToRunID_t NewV;
		if (TypeAsInt > threads_count)
		{
			NewV = TypeAsInt-threads_count;
		}
		else
		{
			NewV = TypeAsInt;
		}

		return ThreadToRunID(NewV);
	}
}

CoreEnd

