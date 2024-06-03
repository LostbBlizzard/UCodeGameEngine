#pragma once
#include "UCodeRunTime/BasicTypes.hpp"
#include <Editor/EditorNamespace.hpp>
#include "UCodeLang/Compilation/Compiler.hpp"
#include "ProjectManagement/RunTimeProjectData.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeRunTime/CoreSystems/Threads.hpp"
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

		using StatusUpdate = std::function<void(String str, size_t thread)>;
		std::shared_ptr<UCode::Mutex<StatusUpdate>> OnStatusUpdate;
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
	static bool CompileProject(CompileData& Data);

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
	static String NewAssetTemplate(const StringView componentname);
	static String NewWindowTemplate(const StringView componentname);
	static String NewCustomDrawTemplate(const StringView componentname);
	static String NewExportTemplate(const StringView componentname);
};
EditorEnd