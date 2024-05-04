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

	RuningTasks.Lock([this](RuningTaskDataList& val) 
	{
		val.TaskData._NextLockKey = ThreadToRunID(1);
		val.TaskData._TaskID = TaskID(0);
	});

	_GlobalThreads = this;
	ThreadCount = ThreadToRunID(0);

	MainThreadID = std::this_thread::get_id();

	CurrentThreadInfo::CurrentThread = MainThread;

	const u32 num_threads = std::thread::hardware_concurrency();
	threads_count = num_threads;

	auto& _Threads = this->_Threads.Unsafe_GetBaseType();

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
	
	Vector<std::shared_ptr<std::thread>> list;
	_Threads.Lock([&list](Vector<ThreadInfo>& val)
		{
			for (auto& Item : val)
			{
				Item._NewTask->notify_one();
			}
			for (auto& Item : val)
			{
				list.push_back(std::move(Item.thread));
			}
			val.clear();
	});
	for (auto& Item : list)
	{
		Item->join();
	}

	_GlobalThreads = nullptr;
}
	
void Threads::Notify_AllThreadsWhoDepens(TaskID id)
{
	_Threads.Lock([id](Vector<ThreadInfo>& _Threads)
	{
		for (auto& Item : _Threads)
		{
			bool ok = false;
			for (auto& t : Item._Data._TaskToDo)
			{
				if (ok == true)
				{
					break;
				}
				for (auto& dep : t.TaskDependencies)
				{
					if (dep == id)
					{
						Item._NewTask->notify_one();
						ok = true;
						break;
					}
				}
			}
		}
	});
}
void Threads::ThreadLoop(Threads* _This,ThreadInfo* Info)
{
	CurrentThreadInfo::CurrentThread = Info->_ThreadID;

	while (true)
	{
		TaskInfo Task;
		{
			std::unique_lock<std::mutex> lock(_This->RuningTasks.Unsafe_GetLockBase());

			auto& MyTaskToDo = Info->_Data._TaskToDo;
			auto& AnyThreadTaskToDo = _This->RuningTasks.Unsafe_GetBaseType().TaskData._AnyThread._TaskToDo;

			Info->_NewTask->wait(lock, [_This, Info, &MyTaskToDo, &AnyThreadTaskToDo]
				{
					_This->_Threads.Unsafe_GetLockBase().lock();
					UCodeGEDefer(_This->_Threads.Unsafe_GetLockBase().unlock());

					return !MyTaskToDo.empty()
						|| !AnyThreadTaskToDo.empty()
						|| _This->_EndThreads;
				}
			);

			if (_This->_EndThreads){break;}

			_This->_Threads.Unsafe_GetLockBase().lock();
			UCodeGEDefer(_This->_Threads.Unsafe_GetLockBase().unlock());
			size_t TaskIndex = 0;
			TaskInfo* TaskToRun = nullptr;

			bool shouldskip = false;
			RuningTaskDataList& List = _This->RuningTasks.Unsafe_GetBaseType();
			{
				bool skip = false;
				
				for (size_t i = 0; i < MyTaskToDo.size(); i++)
				{
					auto& Item = MyTaskToDo[i];


					if (List._RuningTasks.GetValue(Item.taskID).CanStartTask && _This->IsTasksDone(Item.TaskDependencies,List))
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


						if (_This->IsTasksDone(Item.TaskDependencies,List))
						{
							TaskToRun = &Item;
							TaskIndex = i;
							break;
						}
					}
					if (TaskToRun == nullptr) {
						skip = true;
					}
				}
				shouldskip = skip;
			};

			if (shouldskip)
			{
				continue;
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
	}
	else if (ID == AnyThread)
	{
		RuningTasks.Lock([&functocall](RuningTaskDataList& val)
			   {
					  return functocall(val.TaskData._AnyThread);
			   });
	}
	else
	{
		_Threads.Lock([ID, &functocall](Vector<ThreadInfo>& val)
		{
			functocall(val[ID.Get_Base()]._Data);
		});
	}
}

void Threads::GetThreadNoneWorkingThread(std::function<void(ThreadData&, ThreadToRunID)> functocall)
{
	size_t BestPixk = SIZE_MAX;
	ThreadData* Best = nullptr;
	size_t BestIndex = 0;


	_Threads.Lock([&](Vector<ThreadInfo>& val)
	{
		for (size_t i = 0; i < val.size(); i++)
		{
			auto& Item = val[i];

			if (Item._Data._TaskToDo.size() < BestPixk)
			{
				BestPixk = Item._Data._TaskToDo.size();
				Best = &Item._Data;
				BestIndex = i;
			}
		}
		UCodeGEAssert(Best);

		functocall(*Best, ThreadToRunID(BestIndex));
	});
}

void Threads::CallTaskToDoOnMainThread()
{
	Vector<std::shared_ptr<FuncPtr>> FuncList;
	MainThreadData.TryLock([&FuncList,this](MainThreadTaskData& val)
	{
		for (auto& Item : val._TaskToReAddOnToMainThread)
		{
			val._MainThreadData._TaskToDo.push_back(std::move(Item));
		}
		val._TaskToReAddOnToMainThread.clear();
		
		Vector<TaskInfo> CopyList = std::move(val._MainThreadData._TaskToDo);	

		bool b = RuningTasks.TryLock([&FuncList,&val,this,&CopyList](RuningTaskDataList& list)
		{
			for (auto& Item : CopyList)
			{
				if (list._RuningTasks.GetValue(Item.taskID).CanStartTask && IsTasksDone(Item.TaskDependencies,list))
				{
					FuncList.push_back(Item._Func);
				}
				else
				{
					val._TaskToReAddOnToMainThread.push_back(std::move(Item));
				}
			}
		});

		if (b == false)
		{
			for (auto& Item : CopyList)
			{
				val._TaskToReAddOnToMainThread.push_back(std::move(Item));
			}
		}
	});

	for (auto& Item : FuncList) 
	{
		(*Item)();
	}
}

void Threads::Update()
{
	CallTaskToDoOnMainThread();
}

AsynTask Threads::AddTask(ThreadToRunID thread, FuncPtr&& Func, const Vector<TaskID>& TaskDependencies,RuningTaskDataList& List)
{
	bool hasnothreads = false;
	if (hasnothreads)
	{
		TaskInfo Task;
		Task._Func = std::make_shared<FuncPtr>(Func);
		Task.TaskDependencies = TaskDependencies;


		auto taskid = List.TaskData._TaskID.Get_Base();

		Task.taskID = TaskID(taskid - 1);

		List._RuningTasks.GetValue(Task.taskID).ThreadToRun = MainThread;
		MainThreadData.Lock([&Task](MainThreadTaskData& data)
		{
			data._MainThreadData._TaskToDo.push_back(std::move(Task));
		});
	}
	else
	{
		if (thread == MainThread)
		{	
			TaskInfo Task;
			Task._Func =std::make_shared<FuncPtr>(Func);
			Task.TaskDependencies = TaskDependencies;
			

			auto taskid = List.TaskData._TaskID.Get_Base(); 

			Task.taskID = TaskID(taskid-1);
			

			List._RuningTasks.GetValue(Task.taskID).ThreadToRun = thread;
			MainThreadData.Lock([&Task](MainThreadTaskData& data)
			{
				data._MainThreadData._TaskToDo.push_back(std::move(Task));
			});
			return  AsynTask();
		}
		else if (thread == AnyThread)
		{
			TaskInfo Task;
			Task._Func =std::make_shared<FuncPtr>(Func);
			Task.TaskDependencies = TaskDependencies;
			

			auto taskid = List.TaskData._TaskID.Get_Base(); 

			Task.taskID = TaskID(taskid-1);
			
			
			GetThreadNoneWorkingThread([&Task,&List](ThreadData& data,ThreadToRunID ind)
				{
					List._RuningTasks.GetValue(Task.taskID).ThreadToRun = ind;
					data._TaskToDo.push_back(std::move(Task));
				});
			return  AsynTask();
		}
		else
		{
			TaskInfo Task;
			Task._Func = std::make_shared<FuncPtr>(Func);
			Task.TaskDependencies = TaskDependencies;

			auto taskid = List.TaskData._TaskID.Get_Base(); 	


			Task.taskID = TaskID(taskid - 1);
			List._RuningTasks.GetValue(Task.taskID).ThreadToRun = thread;
			GetThreadInfo(thread, [&Task](ThreadData& data)
				{
					data._TaskToDo.push_back(std::move(Task));
				});

			return AsynTask();
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
		RuningTasks.Lock([&val,&Func,&TaskDependencies](RuningTaskDataList& list)
		{
				val._MainThreadData._TaskToDo.push_back({ std::make_shared<FuncPtr>(std::move(Func)),TaskDependencies,TaskID(list.TaskData._TaskID.Get_Base() - 1) });
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

bool Threads::IsTasksDone(const Vector<TaskID>& Tasks, RuningTaskDataList& val)
{

	for (auto& Item : Tasks)
	{
		if (val._RuningTasks.HasValue(Item))
		{
			return false;
		}
	}

	return true;
}

bool Threads::IsTasksDone(TaskID Task,RuningTaskDataList& val)
{
	Vector <TaskID> list; list.push_back(Task);
	return IsTasksDone(list,val);
}

bool Threads::IsTasksDone(const Vector<TaskID>& Tasks)
{
	return RuningTasks.Lock_r<bool>([this,&Tasks](RuningTaskDataList& val)
	{
		return IsTasksDone(Tasks,val);
	});
}

bool Threads::IsTasksDone(TaskID Task)
{
	return RuningTasks.Lock_r<bool>([this,&Task](RuningTaskDataList& val)
	{
		return IsTasksDone(Task,val);
	});
}

ThreadToRunID GetThreadFromTask(TaskType type)
{
	if (type == TaskType::Main
		|| type == TaskType::Rendering
		|| type == TaskType::Physics
		|| type == TaskType::Audio
		|| threads_count == 0)
	{
		return MainThread;
	}
	else if (type == TaskType::Generic) {
		return AnyThread;
	}
	else
	{
		TaskType_t TypeAsInt = (TaskType_t)type;	
		return ThreadToRunID(TypeAsInt % threads_count);
	}
}

CoreEnd

