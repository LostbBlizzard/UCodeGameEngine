#include "UCompiler.hpp"
#include "Helper/AppFiles.hpp"
#include <filesystem>
#include "UCodeRunTime/ULibrarys/UCodeLang/UCodeAPI_Lib.hpp"
EditorStart

namespace fs = std::filesystem;
UCode::AsynTask  UCompiler::CompileFile(const CompileData& Data, const String&  Path)
{
	return UCode::AsynTask();
}


UCode::AsynTask UCompiler::CompileProject(const CompileData& Data)
{
	const Path&  InPath = Data.RunTimeProject->GetAssetsDir();
	const Path&  intPath = Data.RunTimeProject->GetULangIntDir();
	const String LibName = "UCode" + (UCode::String)UCodeLang::FileExt::LibWithDot;
	const Path&  outDir = Data.RunTimeProject->GetULangOutDir();
	const Path ULangExtPath = Path((UCode::String)UCodeLang::FileExt::SourceFileWithDot);
	const auto& LibsPath = Data.RunTimeProject->GetSrcLibsDir();

	
	


	const Path outlibPath = outDir.native() + Path(LibName).native();
	
	Data.Error->Remove_Errors();
	
	UCodeLang::Compiler Compiler;
	UCodeLang::Compiler::CompilerPathData pathData;
	pathData.FileDir = InPath.generic_string();
	pathData.IntDir = intPath.generic_string();
	pathData.OutFile = outlibPath.generic_string();

	Compiler.CompileFiles_UseIntDir(pathData);

	auto& Errors = Compiler.Get_Errors();
	for (auto item : Errors.Get_Errors())
	{
		Data.Error->AddError(item);
	}

	return UCode::AsynTask();
}
optional<Path> UCompiler::GetIntermediate(const Path& FullFilePath, RunTimeProjectData* RunTimeProject)
{
	const Path& InPath = RunTimeProject->GetAssetsDir();
	
	const Path RelativePath = FullFilePath.native().substr(InPath.native().size());

	return GetRelativeIntermediate(RelativePath, RunTimeProject);
}
optional<Path> UCompiler::GetRelativeIntermediate(const Path& RelativeFilePath, RunTimeProjectData* RunTimeProject)
{
	const Path& intPath = RunTimeProject->GetULangIntDir();
	
	auto FilePath = Path(intPath.native() + RelativeFilePath.native() + Path(UCodeLang::FileExt::ObjectWithDot).native());
	if (fs::exists(FilePath))
	{
		return FilePath;
	}
	else 
	{
		return {};
	}
}
bool UCompiler::IsComponent(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	return true;
}
EditorEnd


