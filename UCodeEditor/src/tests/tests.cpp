#include "tests.hpp"

#include "NetworkingTests.hpp"
#include "AsynTests.hpp"
EditorTestStart
using TestFunc = bool(*)();
struct TestInfo
{
	const char* TestName;
	TestFunc Func;
};
const TestInfo Tests[] =
{
	{"NetTest 1",NetTest_1},

	{"AsynTest 1",AsynTest_1},
	{"AsynTest 2",AsynTest_2},
	{"AsynTest 3",AsynTest_3},
	{"AsynTest 4",AsynTest_4},
	{"AsynTest 5",AsynTest_5},
};

constexpr size_t TestCount = sizeof(Tests) / sizeof(Tests[0]);

int RunTests()
{
	UCODE_ENGINE_LOG("Runing Tests");
	size_t TestPassed = 0;

	for (size_t i = 0; i < TestCount; i++)
	{
		auto& Item = Tests[i];
		UCODE_ENGINE_LOG("Runing Test" + (String)Item.TestName);
		bool Passed = Item.Func();

		if (Passed)
		{
			UCODE_ENGINE_LOG("Test " + (String)Item.TestName + "Passed");
			TestPassed++;
		}
		else
		{
			UCODE_ENGINE_LOG("Test " + (String)Item.TestName + "Failed");
		}
	}


	UCODE_ENGINE_LOG( std::to_string(TestPassed) + "/" + std::to_string(TestCount) + " Tests Passed");

	if (TestPassed == 0)
	{
		UCODE_ENGINE_LOG("Tests Passed");
	}
	else
	{
		UCODE_ENGINE_LOG("Tests Fail")
	}
	return TestPassed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
EditorTestEnd