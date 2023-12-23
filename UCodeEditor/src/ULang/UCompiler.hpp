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
		bool Is32bitMode = sizeof(void*) == 4;
		void Set(const RunTimeProjectData* Data)
		{
			InPath = Data->GetAssetsDir();
			IntPath = Data->GetULangIntDir();

			const String LibName = "ucode" + (UCode::String)UCodeLang::FileExt::LibWithDot;

			const Path& outDir = Data->GetULangOutDir();
			const Path outlibPath = outDir.native() + Path(LibName).native();

			OutPath = outlibPath;
		}
	};
	static bool CompileProject(const CompileData& Data);

	static Optional<Path> GetIntermediate(const Path& FullFilePath, RunTimeProjectData* RunTimeProject);
	static Optional<Path> GetRelativeIntermediate(const Path& RelativeFilePath, RunTimeProjectData* RunTimeProject);

	static bool IsAComponent(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);
	static bool IsComponentTrait(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);
	
	static bool IsAAsset(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);
	static bool IsAssetTrait(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);

	static bool IsAWindow(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);
	static bool IsWindowTrait(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);

	static bool IsAExporter(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);
	static bool IsExportTrait(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly);


	static String NewComponentTemplate(const StringView componentname);
};
EditorEnd