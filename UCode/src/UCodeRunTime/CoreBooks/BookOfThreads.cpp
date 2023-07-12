#include "BookOfThreads.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart


constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::BookOfThreads;

static std::thread::id MainThreadID = {};

//To Do set threads on static
BookOfThreads::BookOfThreads(Gamelibrary* lib) :libraryBook(lib), _EndThreads(false), _NextLockKey(1)
{
	MainThreadID = std::this_thread::get_id();

	Threads = this;
	const UInt32 num_threads = std::thread::hardware_concurrency();
	_Threads.resize(num_threads);
	for (UInt32 i = 0; i < num_threads; i++)
	{
		_Threads.at(i) = std::thread(ThreadLoop,this);
	}
}

BookOfThreads::~BookOfThreads()
{
	_EndThreads = true;
	_NewTask.notify_all();
	for (size_t i = 0; i < _Threads.size(); i++)
	{
		auto& Item = _Threads[i];
		Item.join();
	}
	_Threads.clear();
}
void BookOfThreads::ThreadLoop(BookOfThreads* _This)
{
	while (true)
	{
		TastInfo Task;
		{
			std::unique_lock<std::mutex> lock(_This->_TaskLock);

			_This->_NewTask.wait(lock, [_This]
				{
					return !_This->_TaskToDo.empty() || _This->_EndThreads;
				}
			);

			if (_This->_EndThreads){return;}

			Task = _This->_TaskToDo.front();
			_This->_TaskToDo.pop_back();
		}
		Task._Func();
	}
}

void BookOfThreads::CallTaskToDoOnMainThread()
{
	if (_OnMainThreadLock.try_lock())
	{
		Vector<std::function<void()>> CopyList = std::move(_TaskToDoOnMainThread);
		_OnMainThreadLock.unlock();

		for (auto& Item : CopyList)
		{
			Item();
		}
	}
}

void BookOfThreads::Update()
{
	CallTaskToDoOnMainThread();
}

AsynTask BookOfThreads::AddTask(ThreadLockKey thread, FuncPtr Func)
{

	if (_Threads.size() == 0 || _Threads.size() == 1)
	{ 
		Func();
		return AsynTask();
	}
	else
	{
		if (thread != BookOfThreads::AnyThread) 
		{
			UCODE_ENGINE_IMPlEMENTED_LATER;
		}
		else
		{
			TastInfo Task;
			Task._Func = Func;
			_TaskLock.lock();
			

			_TaskToDo.push_back(Task);
			_NewTask.notify_one();

			_TaskLock.unlock();
		}
	}


	return AsynTask();
}
AsynTask BookOfThreads::AddTask(TaskType TaskType,FuncPtr Func)
{
	ThreadLockKey thread;
	if (TaskType == TaskType::Generic)
	{
		thread = AnyThread;
	}
	else
	{
		//get an no work thread of the same TastType.
		thread = AnyThread;
	}

	return AddTask(thread, Func);
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

BookOfThreads::ThreadLockKey BookOfThreads::GetNewThreadLockKey()
{
	_TaskLock.lock();

	auto R = _NextLockKey;
	_NextLockKey++;
	if (_NextLockKey == AnyThread) { _NextLockKey++; }

	_TaskLock.unlock();
	
	return R;
}

void BookOfThreads::FreeThreadLock(ThreadLockKey key)
{
}

void BookOfThreads::RunOnOnMainThread(const std::function<void()>& Func)
{
	_OnMainThreadLock.lock();

	_TaskToDoOnMainThread.push_back(Func);

	_OnMainThreadLock.unlock();
}

void BookOfThreads::RunOnOnMainThread(std::function<void()>&& Func)
{
	_OnMainThreadLock.lock();

	_TaskToDoOnMainThread.push_back(std::move(Func));

	_OnMainThreadLock.unlock();
}
 
bool BookOfThreads::IsOnMainThread()
{
	return MainThreadID == std::this_thread::get_id();
}

bool BookOfThreads::ThrowErrIfNotOnMainThread()
{
	if (!IsOnMainThread())
	{
		throw std::exception("Not on Main-Thread");
	}
}

CoreEnd


