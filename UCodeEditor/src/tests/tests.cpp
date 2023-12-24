#include "tests.hpp"

#include "NetworkingTests.hpp"
#include "AsynTests.hpp"
#include "ProjectTests.hpp"
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

	{"Project Hello[PlayerMovment]",Project_Hello},
};

constexpr size_t TestCount = sizeof(Tests) / sizeof(Tests[0]);

int RunTests()
{
	UCodeGELog("Runing Tests");
	size_t TestPassed = 0;

	for (size_t i = 0; i < TestCount; i++)
	{
		auto& Item = Tests[i];
		UCodeGELog("Runing Test " + (String)Item.TestName + " ");
		bool Passed = Item.Func();

		if (Passed)
		{
			UCodeGELog("Test " + (String)Item.TestName + "Passed");
			TestPassed++;
		}
		else
		{
			UCodeGELog("Test " + (String)Item.TestName + "Failed");
		}
	}


	UCodeGELog( std::to_string(TestPassed) + "/" + std::to_string(TestCount) + " Tests Passed");

	if (TestPassed == TestCount)
	{
		UCodeGELog("Tests Passed");
	}
	else
	{
		UCodeGELog("Tests Fail")
	}
	return TestPassed == TestCount ? EXIT_SUCCESS : EXIT_FAILURE;
}
EditorTestEnd