#pragma once
#include "Editor/EditorApp.hpp"
#include"TestsNamespace.hpp"
#include "UCodeRunTime/ULibrarys/Loger.hpp"
EditorTestStart

int RunTests()
{
	UCODE_ENGINE_LOG("Runing Tests");
	bool TestPassed = true;

	if (TestPassed) 
	{
		UCODE_ENGINE_LOG("Tests Passed");
	}
	else
	{
		UCODE_ENGINE_LOG("Tests Fail")
	}
	return TestPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}

EditorTestEnd