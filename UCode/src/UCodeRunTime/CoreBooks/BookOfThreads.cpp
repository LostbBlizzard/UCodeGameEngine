#include "BookOfThreads.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart


constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::BookOfThreads;

static std::thread::id MainThreadID = {};
static ThreadToRunID ThreadCount = NullThread;
thread_local ThreadToRunID CurrentThreadInfo::CurrentThread = NullThread;

static u32 threads_count=0;

//To Do set threads on static
BookOfThreads::BookOfThreads(Gamelibrary* lib) :libraryBook(lib), 
_EndThreads(false), 
_NextLockKey(1),
 _TaskID(0)
{
	_TaskID =TaskID(0);
	Threads = this;
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

BookOfThreads::~BookOfThreads()
{
	_EndThreads = true;
	Threads = nullptr;
	
	//
	for (auto& Item : _MainThreadData._TaskToDo)
	{
		TryCallCancel(Item.taskID);
	}
	_MainThreadData._TaskToDo.clear();
	for (auto& Item : _TaskToReAddOnToMainThread)
	{
		TryCallCancel(Item.taskID);
	}
	_TaskToReAddOnToMainThread.clear();
	//
	
	_NewTask.notify_all();
	for (size_t i = 0; i < _Threads.size(); i++)
	{
		auto& Item = _Threads[i];
		Item.thread->join();
	}
	_Threads.clear();


	
}
void BookOfThreads::ThreadLoop(BookOfThreads* _This,ThreadInfo* Info)
{
	CurrentThreadInfo::CurrentThread = Info->_ThreadID;

	while (true)
	{
		TaskInfo Task;
		{
			std::unique_lock<std::mutex> lock(_This->_TaskLock);

			auto& MyTaskToDo = Info->_Data._TaskToDo;
			auto& AnyThreadTaskToDo = _This->_AnyThread._TaskToDo;

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

				
				if (IsTasksDone(Item.TaskDependencies))
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


					if (IsTasksDone(Item.TaskDependencies))
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

	for (auto& Item : Info->_Data._TaskToDo)
	{
		TryCallCancel(Item.taskID);
	}
	Info->_Data._TaskToDo.clear();
}

BookOfThreads::ThreadData& BookOfThreads::GetThreadInfo(ThreadToRunID ID)
{
	if (ID == MainThread)
	{
		return _MainThreadData;
	}
	else if (ID == AnyThread)
	{
		return _AnyThread;
	}
	else
	{
		return _Threads[ID.Get_Base()]._Data;
	}
}

BookOfThreads::ThreadData& BookOfThreads::GetThreadNoneWorkingThread()
{
	_TaskLock.lock();
	size_t BestPixk = SIZE_MAX;
	BookOfThreads::ThreadData* Best = nullptr;
	for (auto& Item : _Threads)
	{
		if (Item._Data._TaskToDo.size() < BestPixk)
		{
			BestPixk = Item._Data._TaskToDo.size();
			Best = &Item._Data;
		}
	}
	_TaskLock.unlock();
	return *Best;
}

void BookOfThreads::CallTaskToDoOnMainThread()
{
	if (_OnMainThreadLock.try_lock())
	{
		for (auto& Item : _TaskToReAddOnToMainThread)
		{
			_MainThreadData._TaskToDo.push_back(std::move(Item));
		}
		_TaskToReAddOnToMainThread.clear();
		
		Vector<TaskInfo> CopyList = std::move(_MainThreadData._TaskToDo);
		
		_OnMainThreadLock.unlock();
	

		/*
		for (auto i = CopyList.rbegin();i != CopyList.rend(); ++i)
		{

			auto& Item = *i;
		*/
		_MainThreadData._TaskToDo = CopyList;//for GetProgress
		size_t V = _MainThreadData._TaskToDo.size();

		for (auto  Item: CopyList)
		{	
			if (IsTasksDone(Item.TaskDependencies))
			{
				(*Item._Func)();
			}
			else
			{
				//_TaskToReAddOnToMainThread.push_back(std::move(Item));

				_TaskToReAddOnToMainThread.push_back(std::move(Item));
			}
		}

		for (size_t i = 0; i < V; i++)
		{
			_MainThreadData._TaskToDo.erase(_MainThreadData._TaskToDo.begin());
		}
	}
}

void BookOfThreads::Update()
{
	CallTaskToDoOnMainThread();
}

AsynTask BookOfThreads::AddTask(ThreadToRunID thread, FuncPtr&& Func, const Vector<TaskID>& TaskDependencies)
{
	RunOnOnMainThread(std::move(Func), TaskDependencies);
	return  AsynTask();

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
			

			_TaskLock.lock();
			Task.taskID = TaskID(_TaskID.Get_Base()-1);
			_TaskLock.unlock();	
			GetThreadNoneWorkingThread()._TaskToDo.push_back(std::move(Task));
			return  AsynTask();
		}
		else
		{
			TaskInfo Task;
			Task._Func = std::make_shared<FuncPtr>(Func);
			Task.TaskDependencies = TaskDependencies;
			

			_TaskLock.lock();
			Task.taskID = TaskID(_TaskID.Get_Base()-1);
			auto& V = GetThreadInfo(thread);
			
			V._TaskToDo.push_back(std::move(Task));

			_TaskLock.unlock();
		}
	}


	return AsynTask();
}
BookOfThreads* BookOfThreads::Get(Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	if (V) { return (BookOfThreads*)V; }

	BookOfThreads* r = new BookOfThreads(lib);;

	lib->MoveBook(r);
	lib->SetStaticComponent(KeyIdex, r);
	return r;
}
BookOfThreads* BookOfThreads::Find(const Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	return (BookOfThreads*)V;
}

ThreadToRunID BookOfThreads::GetNewThreadLockKey()
{
	_TaskLock.lock();

	auto R = _NextLockKey;
	_NextLockKey = ThreadToRunID(R.Get_Base() + 1);

	_TaskLock.unlock();
	
	return R;
}

void BookOfThreads::FreeThreadLock(ThreadToRunID key)
{
}
void BookOfThreads::RunOnOnMainThread(FuncPtr&& Func, const Vector<TaskID>& TaskDependencies)
{
	_OnMainThreadLock.lock();

	_MainThreadData._TaskToDo.push_back({ std::make_shared<FuncPtr>(std::move(Func)),TaskDependencies,TaskID(_TaskID.Get_Base()-1) });

	_OnMainThreadLock.unlock();
}
 
bool BookOfThreads::IsOnMainThread()
{
	return MainThreadID == std::this_thread::get_id();
}

void BookOfThreads::ThrowErrIfNotOnMainThread()
{
	if (!IsOnMainThread())
	{
		throw std::exception("Not on Main-Thread");
	}
}

bool BookOfThreads::IsTasksDone(const Vector<TaskID>& Tasks)
{
	for (auto& Item : Tasks)
	{
		if (_Map.count(Item))
		{
			return false;
		}
	}
	return true;
}

bool BookOfThreads::IsTasksDone(TaskID Task)
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

