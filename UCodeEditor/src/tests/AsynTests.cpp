#include "AsynTests.hpp"

#include "UCodeRunTime/CoreBooks/BookOfThreads.hpp"
EditorTestStart


struct AsynTestContext
{
	UCode::Unique_ptr<UCode::Gamelibrary> Lib = nullptr;
	UCode::BookOfThreads* Threads = nullptr;
	void SetUp()
	{
		Lib  = UCode::Unique_ptr<UCode::Gamelibrary>(new UCode::Gamelibrary());
		Threads = UCode::BookOfThreads::Get(Lib.get());
	}
	~AsynTestContext()
	{
		
	}
	void Update()
	{
		Lib->libraryUpdate();
	}
};

bool AsynTest_1()
{
	AsynTestContext V;V.SetUp();


	UCode::Delegate<int, int, int> Func = [](int X, int Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	auto task = V.Threads->AddTask_t(UCode::TaskType::Generic,std::move(Func), {}, 5, 10);
	while (!task.IsDone()){V.Update();}

	int NewValue = task.GetValue();

	return NewValue == 15;
}
bool AsynTest_2()
{
	AsynTestContext V; V.SetUp();

	UCode::Delegate<int, int, int> Func = [](int X, int Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	auto task = V.Threads->RunOnOnMainThread(std::move(Func), {}, 5, 10);
	while (!task.IsDone()){V.Update();}

	int NewValue = task.GetValue();

	return NewValue == 15;
}
bool AsynTest_3()
{
	AsynTestContext V; V.SetUp();

	bool WasSet = false;
	int Value = 0;
	UCode::Delegate<int, int, int> Func = [](int X, int Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	UCode::Delegate<void,int> DoneFunc = [&](int value)
	{
		//std::this_thread::sleep_until(UCode::Time::Time_point(UCode::Time::fsec(1)));
		Value = value;
		WasSet = true;
	};

	auto task = V.Threads->RunOnOnMainThread(std::move(Func), {}, 5, 10)
		.OnCompletedOnMainThread(std::move(DoneFunc));


	while (WasSet == false) { V.Update(); }

	int NewValue = Value;

	return NewValue == 15;
}
bool AsynTest_4()
{
	AsynTestContext V; V.SetUp();

	bool WasSet = false;
	int Value = 0;
	UCode::Delegate<int, int, int> Func = [](int X, int Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	UCode::Delegate<void, int> DoneFunc = [&](int&& value)
	{
		//std::this_thread::sleep_until(UCode::Time::Time_point(UCode::Time::fsec(1)));
		Value = value;
		WasSet = true;
	};

	auto task = V.Threads->RunOnOnMainThread(std::move(Func), {}, 5, 10)
		.ContinueTaskThread(std::move(DoneFunc));


	while (WasSet == false) { V.Update(); }

	int NewValue = Value;

	return NewValue == 15;
}
bool AsynTest_5()
{
	AsynTestContext V; V.SetUp();

	bool WasSet = false;
	bool WasFailSet = false;
	int Value = 0;
	UCode::Delegate<int, int, int> Func = [](int X, int Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	UCode::Delegate<void, int> DoneFunc = [&](int&& value)
	{
		Value = value;
		WasSet = true;
	};

	UCode::Delegate<void> FuncFail = [&]()
	{
		WasFailSet = true;
		Value = 15;
	};

	auto task = V.Threads->RunOnOnMainThread(std::move(Func), {}, 5, 10)
		.ContinueTaskThread(std::move(DoneFunc))
		.OnCancelOnMainThread(std::move(FuncFail));

	task.CancelTask();

	while (WasFailSet == false) { V.Update(); }

	int NewValue = Value;

	return NewValue == 15;
}

bool AsynTest_6()
{
	AsynTestContext V; V.SetUp();

	bool WasSet = false;
	int Value = 0;
	UCode::Delegate<int, int, int> Func = [](int X, int Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	UCode::Delegate<void, int> DoneFunc = [&](int&& value)
	{
		//std::this_thread::sleep_until(UCode::Time::Time_point(UCode::Time::fsec(1)));
		Value = value;
		WasSet = true;
	};

	auto task = V.Threads->RunOnOnMainThread(std::move(Func), {}, 5, 10)
		.ContinueOnMainThread(std::move(DoneFunc));


	while (WasSet == false) { V.Update(); }

	int NewValue = Value;

	return NewValue == 15;
}
bool AsynTest_7()
{
	AsynTestContext V; V.SetUp();

	bool WasSet = false;
	int Value = 0;
	UCode::Delegate<int, int, int> Func = [](int X, int Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	UCode::Delegate<void, int> DoneFunc = [&](int&& value)
	{
		//std::this_thread::sleep_until(UCode::Time::Time_point(UCode::Time::fsec(1)));
		Value = value;
		WasSet = true;
	};

	auto task = V.Threads->RunOnOnMainThread(std::move(Func), {}, 5, 10)
		.ContinueOnAnyThread(std::move(DoneFunc));


	while (WasSet == false) { V.Update(); }

	int NewValue = Value;

	return NewValue == 15;
}
bool AsynTest_8()
{
	AsynTestContext V; V.SetUp();

	bool WasSet = false;
	int Value = 0;
	UCode::Delegate<int, int, int> Func = [](int X, int Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	UCode::Delegate<void, int> DoneFunc = [&](int&& value)
	{
		//std::this_thread::sleep_until(UCode::Time::Time_point(UCode::Time::fsec(1)));
		Value = value;
		WasSet = true;
	};

	auto task = V.Threads->RunOnOnMainThread(std::move(Func), {}, 5, 10)
		.ContinueCurrentThread(std::move(DoneFunc));


	while (WasSet == false) { V.Update(); }

	int NewValue = Value;

	return NewValue == 15;
}
EditorTestEnd