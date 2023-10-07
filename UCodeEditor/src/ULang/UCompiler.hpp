#pragma once
#include "UCodeRunTime/BasicTypes.hpp"
#include <Editor/EditorNamespace.hpp>
#include "UCodeLang/Compliation/Compiler.hpp"
#include "UCodeRunTime/CoreBooks/BookOfThreads.hpp"
#include "ProjectManagement/RunTimeProjectData.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
EditorStart

class UCompiler
{
public:
	class CompileData
	{
	public:
		UCodeLang::CompliationErrors* Error;
		UCode::BookOfThreads* Threads;
		RunTimeProjectData* RunTimeProject;
	};
    static UCode::AsynTask CompileFile(const CompileData& Data, const String&  Path);
	static UCode::AsynTask CompileProject(const CompileData& Data);

	static Optional<Path> GetIntermediate(const Path& FullFilePath, RunTimeProjectData* RunTimeProject);
	static Optional<Path> GetRelativeIntermediate(const Path& RelativeFilePath, RunTimeProjectData* RunTimeProject);

	static bool IsComponent(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);
};
EditorEnd