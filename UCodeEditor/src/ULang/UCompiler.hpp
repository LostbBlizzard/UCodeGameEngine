#pragma once
#include "UCodeRunTime/BasicTypes.hpp"
#include <Editor/EditorNamespace.hpp>
#include "UCodeLang/Compilation/Compiler.hpp"
#include "ProjectManagement/RunTimeProjectData.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
EditorStart

class UCompiler
{
public:
	class CompileData
	{
	public:
		UCodeLang::CompilationErrors* Error;
		UCode::Threads* Threads;

		Path InPath;
		Path IntPath;
		Path OutPath;
		bool Editor = false;
		void Set(const RunTimeProjectData* Data)
		{
			InPath = Data->GetAssetsDir();
			IntPath = Data->GetULangIntDir();

			const String LibName = "UCode" + (UCode::String)UCodeLang::FileExt::LibWithDot;

			const Path& outDir = Data->GetULangOutDir();
			const Path outlibPath = outDir.native() + Path(LibName).native();

		}
	};
	static bool CompileProject(const CompileData& Data);

	static Optional<Path> GetIntermediate(const Path& FullFilePath, RunTimeProjectData* RunTimeProject);
	static Optional<Path> GetRelativeIntermediate(const Path& RelativeFilePath, RunTimeProjectData* RunTimeProject);

	static bool IsComponent(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);
};
EditorEnd