#include "AsynTests.hpp"

#include "UCodeRunTime/CoreSystems/Threads.hpp"

EditorTestStart


struct AsynTestContext
{
	UCode::Unique_ptr<UCode::Gamelibrary> Lib = nullptr;
	UCode::Threads* Threads = nullptr;
	void SetUp()
	{
		Lib  = UCode::Unique_ptr<UCode::Gamelibrary>(new UCode::Gamelibrary());
		Threads = UCode::Threads::Get(Lib.get());
	}
	~AsynTestContext()
	{
		
	}
	void Update()
	{
		Lib->libraryUpdate();
	}
};




template<typename V_t> 
bool AsynTest_1_V(V_t XValue,V_t YValue)
{
	AsynTestContext V; V.SetUp();

	
	V_t RValue = XValue + YValue;

	UCode::Delegate<V_t,V_t,V_t> Func = [](V_t X,V_t Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};

	auto task = V.Threads->AddTask_t(UCode::TaskType::Generic, std::move(Func), {}, XValue, YValue);
	while (!task.IsDone()) { V.Update(); }

	V_t NewValue = task.GetValue();

	return NewValue == RValue;
}

template<typename V_t>
bool AsynTest_1_VMove(V_t XValue,V_t YValue)
{
	AsynTestContext V; V.SetUp();


	V_t RValue = XValue + YValue;

	UCode::Delegate<V_t,UCode::Unique_ptr<V_t>,UCode::Unique_ptr<V_t>> Func = [](UCode::Unique_ptr<V_t> X,UCode::Unique_ptr<V_t> Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return *X+  *Y;
	};

	auto task = V.Threads->AddTask_t(UCode::TaskType::Generic, std::move(Func), {},
		std::make_unique<V_t>(XValue), 
		std::make_unique<V_t>(YValue));
	while (!task.IsDone()) { V.Update(); }

	V_t NewValue = task.GetValue();

	return NewValue == RValue;
}


template<typename V_t>
bool AsynTest_2_V(V_t XValue, V_t YValue)
{
	AsynTestContext V; V.SetUp();

	UCode::Delegate<V_t, V_t, V_t> Func = [](V_t X,V_t Y)
	{
		std::this_thread::sleep_for(UCode::Time::fsec(1));
		return X + Y;
	};
	V_t RValue = XValue + YValue;

	auto task = V.Threads->RunOnOnMainThread(std::move(Func), {}, XValue, YValue);
	while (!task.IsDone()) { V.Update(); }

	V_t NewValue = task.GetValue();

	return NewValue == RValue;
}

bool AsynTest_1()
{
	return
		AsynTest_1_V<int>(5, 10);
		AsynTest_1_V<UCode::String>("Hello", "World");
		//AsynTest_1_VMove<int>(5,19) &&
		//AsynTest_1_VMove<UCode::String>(UCode::String("Good bye"), UCode::String("World"));
}
bool AsynTest_2()
{
	return AsynTest_2_V<int>(5, 10) &&
		AsynTest_2_V<UCode::String>("Hello", "World");
		//AsynTest_2_V<MoveOnly<int>>(5, 19) &&
		//AsynTest_2_V<MoveOnly<std::string>>(std::string("Good bye"), std::string("World"));
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