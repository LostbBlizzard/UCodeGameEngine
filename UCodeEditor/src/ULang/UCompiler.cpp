#include "UCompiler.hpp"
#include "Helper/AppFiles.hpp"
#include <filesystem>
#include "UCodeLang/Compilation/ModuleFile.hpp"
EditorStart

namespace fs = std::filesystem;



bool HasModule(const UCodeLang::ModuleIndex& index,const String& name)
{
	for (auto& Item : index._IndexedFiles)
	{
		if (Item._ModuleData.ModuleName == name)
		{
			return true;
		}
	}
	return false;
}

bool UCompiler::CompileProject(const CompileData& Data)
{
	const Path& InPath = Data.InPath;
	const Path& intPath = Data.IntPath;
	const Path& outlibPath = Data.OutPath;
	
	Data.Error->Remove_Errors();
	
	UCodeLang::Compiler Compiler;
	UCodeLang::Compiler::CompilerPathData pathData;
	pathData.FileDir = InPath.generic_string();
	pathData.IntDir = intPath.generic_string();
	pathData.OutFile = outlibPath.generic_string();

	UCodeLang::Compiler::ExternalFiles External;


	UCodeLang::ModuleIndex index = UCodeLang::ModuleIndex::GetModuleIndex();
	
	bool update = false;
	if (!HasModule(index, "StandardLibrary")) {
		index.AddModueToList(AppFiles::GetFilePathByMove("source") / "StandardLibrary" / UCodeLang::ModuleFile::FileNameWithExt);
		update = true;
	}
	if (!HasModule(index, "NStandardLibrary")) {
		index.AddModueToList(AppFiles::GetFilePathByMove("source") / "NStandardLibrary" / UCodeLang::ModuleFile::FileNameWithExt);
		update = true;
	}

	if (update) {
		UCodeLang::ModuleIndex::SaveModuleIndex(index);
	}

	{
		UCodeLang::ModuleFile f;
		Path modpath = AppFiles::GetFilePathByMove("source");
		modpath /= "UCodeGameEngine";
		modpath /= UCodeLang::ModuleFile::FileNameWithExt;

		f.FromFile(&f, modpath);


		auto v = f.BuildModule(Compiler, index, true);
		if (v.CompilerRet.IsError())
		{
			return false;
		}
		else 
		{
			External.Files.push_back(v.OutputItemPath);
		}
	}

	if (Data.Editor)
	{
		UCodeLang::ModuleFile f;
		Path modpath = AppFiles::GetFilePathByMove("source");
		modpath /= "UCodeGameEngineEditor";
		modpath /= UCodeLang::ModuleFile::FileNameWithExt;

		f.FromFile(&f, modpath);


		auto v = f.BuildModule(Compiler, index, true);
		if (v.CompilerRet.IsError())
		{
			return false;
		}
		else
		{
			External.Files.push_back(v.OutputItemPath);
		}
	}

	auto r = Compiler.CompileFiles_UseIntDir(pathData, External);

	auto& Errors = Compiler.Get_Errors();
	for (auto item : Errors.Get_Errors())
	{
		Data.Error->AddError(item);
	}
	return !r.IsError();
}
Optional<Path> UCompiler::GetIntermediate(const Path& FullFilePath, RunTimeProjectData* RunTimeProject)
{
	const Path& InPath = RunTimeProject->GetAssetsDir();
	
	const Path RelativePath = FullFilePath.native().substr(InPath.native().size());

	return GetRelativeIntermediate(RelativePath, RunTimeProject);
}
Optional<Path> UCompiler::GetRelativeIntermediate(const Path& RelativeFilePath, RunTimeProjectData* RunTimeProject)
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


