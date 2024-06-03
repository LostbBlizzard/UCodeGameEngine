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


bool UCompiler::CompileProject(CompileData& Data)
{
	UCodeGEStackFrame("UCompiler:CompileProject");
	const Path& InPath = Data.InPath;
	const Path& intPath = Data.IntPath;
	const Path& outlibPath = Data.OutPath;

	const Path Modulepath = InPath / UCodeLang::ModuleFile::FileNameWithExt;

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
	if (!HasModule(index, "UCodeGameEngine")) {
		index.AddModueToList(AppFiles::GetFilePathByMove("source") / "UCodeGameEngine" / UCodeLang::ModuleFile::FileNameWithExt);
		update = true;
	}
	if (!HasModule(index, "UCodeGameEngineEditor")) {
		index.AddModueToList(AppFiles::GetFilePathByMove("source") / "UCodeGameEngineEditor" / UCodeLang::ModuleFile::FileNameWithExt);
		update = true;
	}

	if (!fs::exists(Modulepath))
	{
		UCodeLang::ModuleFile m;

		m.ModuleName.AuthorName = "UEngineDev";
		m.ModuleName.ModuleName = "Project";

		m.RemoveUnSafe = true;
		{
			UCodeLang::ModuleFile f;
			Path modpath = AppFiles::GetFilePathByMove("source");
			modpath /= "UCodeGameEngine";
			modpath /= UCodeLang::ModuleFile::FileNameWithExt;

			f.FromFile(&f, modpath);

			UCodeLang::ModuleFile::ModuleDependencie dep;
			dep.Identifier = f.ModuleName;
			m.ModuleDependencies.push_back(std::move(dep));
		}
		{

			UCodeLang::ModuleFile f;
			Path modpath = AppFiles::GetFilePathByMove("source");
			modpath /= "UCodeGameEngineEditor";
			modpath /= UCodeLang::ModuleFile::FileNameWithExt;

			f.FromFile(&f, modpath);


			UCodeLang::ModuleFile::ModuleDependencie dep;

			dep.Identifier = f.ModuleName;
			m.ModuleDependencies.push_back(std::move(dep));
		}

		UCodeLang::ModuleFile::ToFile(&m, Modulepath);
	}

	if (update) {
		UCodeLang::ModuleIndex::SaveModuleIndex(index);
	}


	UCodeLang::ModuleFile mainmoule;
	if (UCodeLang::ModuleFile::FromFile(&mainmoule, Modulepath))
	{
		UCodeLang::TaskManger taskmanger;
		taskmanger.Init();

		{
			std::function<void(CompileData::StatusUpdate&)> func = [&mainmoule, threads = Data.Threads](CompileData::StatusUpdate& func)-> void
				{
					func("Downloading Dependencies", UCode::CurrentThreadInfo::CurrentThread.Get_Base());
				};
			Data.OnStatusUpdate->Lock(func);

			std::mutex errorlock;

			if (!mainmoule.DownloadModules(index, [&errorlock, &Data](String log)
				{
					std::function<void(CompileData::StatusUpdate&)> func = [&log,threads = Data.Threads](CompileData::StatusUpdate& func)-> void
						{
							func(log, UCode::CurrentThreadInfo::CurrentThread.Get_Base());
						};
					Data.OnStatusUpdate->Lock(func);

				}, taskmanger))
			{
				return Data.Error;
			}
		}
		Compiler.Get_Settings().PtrSize = Data.Is32bitMode ? UCodeLang::IntSizes::Int32 : UCodeLang::IntSizes::Int64;

		struct ModuleBuildOut 
		{
			UCodeLang::ModuleFile::ModuleRet ret;
			UCodeLang::CompilationErrors errors;
			ModuleBuildOut(UCodeLang::ModuleFile::ModuleRet&& ret) :ret(std::move(ret))
			{

			}
		};
		Vector<UCodeLang::TaskManger::Task<Optional<ModuleBuildOut>>> DependsList;
		DependsList.reserve(mainmoule.ModuleDependencies.size());

		UCode::Mutex<size_t> BuildCount = 0;
		std::function<void(CompileData::StatusUpdate&)> func = [&mainmoule, threads = Data.Threads](CompileData::StatusUpdate& func)-> void
			{
				func("Building Dependencies", UCode::CurrentThreadInfo::CurrentThread.Get_Base());
			};
		Data.OnStatusUpdate->Lock(func);


		for (auto& Deps : mainmoule.ModuleDependencies)
		{
			bool iseditor = false;
			if (Deps.Identifier.AuthorName == "LostbBlizzard" && Deps.Identifier.ModuleName == "UCodeGameEngineEditor")
			{
				iseditor = true;
			}

			if (iseditor && !Data.Editor)
			{
				continue;
			}

			UCodeLang::ModuleIndex::IndexModuleFile* modindex = nullptr;


			auto val = index.FindFile(Deps.Identifier);
			if (val.has_value())
			{
				modindex = &index._IndexedFiles[val.value()];
			}
			

			if (modindex == nullptr)
			{


			}
			else
			{
				std::function<Optional<ModuleBuildOut>()> func = [&BuildCount,&Data, &modindex, &index, &Compiler,&taskmanger]()->Optional<ModuleBuildOut>
					{
						UCodeLang::ModuleFile file;
						if (UCodeLang::ModuleFile::FromFile(&file, modindex->_ModuleFullPath))
						{
							ModuleBuildOut r = file.BuildModule(Compiler, index, true,
								[&Data](String msg) mutable
								{
								std::function<void(CompileData::StatusUpdate&)> func = [msg, threads = Data.Threads](CompileData::StatusUpdate& func)->void
									{
										func(msg, UCode::CurrentThreadInfo::CurrentThread.Get_Base());

									};
								Data.OnStatusUpdate->Lock(func);
								},taskmanger);

							BuildCount.Lock([](size_t& item)
								{
									item++;
								});

							if (r.ret.CompilerRet.IsError()) 
							{
								r.errors = Compiler.Get_Errors();
							}
							return r;
						}
						else
						{
							return {};
						}
					};
				
				DependsList.push_back(taskmanger.AddTask(func, {}));
			}
		}


		
		auto taskoutput = taskmanger.WaitFor(DependsList);
		for (size_t i = 0; i < taskoutput.size(); i++)
		{
			auto& Item = taskoutput[i];
			auto& Dep = mainmoule.ModuleDependencies[i];

			Path moddir = index._IndexedFiles[index.FindFile(Dep.Identifier).value()]._ModuleFullPath.parent_path();
			if (Item.has_value())
			{
				if (Item.value().ret.CompilerRet.IsError())
				{
					*Data.Error = std::move(Item.value().errors);

					
					for (auto& Item : (*Data.Error).Get_Errors())
					{
						Item.File = moddir.native() + Item.File.native();
					}
					return false;
				}
				else 
				{
					External.Files.push_back(Item.value().ret.OutputItemPath);
				}
			}
			else
			{
				(*Data.Error).AddError(UCodeLang::ErrorCodes::InternalCompilerError, 0, 0
					, String("Unable to Read ") + String(UCodeLang::ModuleFile::FileNameWithExt) + " at" + moddir.generic_string());
				return false;
			}
		}	
		
		Compiler.SetLog([&Data,&mainmoule]()
			{				
				std::function<void(CompileData::StatusUpdate&)> func = [&mainmoule,threads = Data.Threads](CompileData::StatusUpdate& func)-> void
									{
										func("Building Project:" + mainmoule.ModuleName.ModuleName, UCode::CurrentThreadInfo::CurrentThread.Get_Base());
									};
				Data.OnStatusUpdate->Lock(func);

			});
		auto r = Compiler.CompileFiles_UseIntDir(pathData, External, taskmanger);


		*Data.Error = std::move(Compiler.Get_Errors());
		return !r.IsError();
	}
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

//Because UCodeLang ReflectionID dont change when recompilation or between ClassAssemblys we can just cash it
thread_local Optional<UCodeLang::ReflectionCustomTypeID> CompoentID;
thread_local Optional<UCodeLang::ReflectionCustomTypeID> AssetID;
thread_local Optional<UCodeLang::ReflectionCustomTypeID> WindowID;
thread_local Optional<UCodeLang::ReflectionCustomTypeID> ExporterID;

bool UCompiler::IsAComponent(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	if (!CompoentID.has_value())
	{
		if (auto node = Assembly.Find_Node((StringView)"Component", "UCodeGameEngine"))
		{
			if (node->Get_Type() == UCodeLang::ClassType::Trait)
			{
				CompoentID = node->Get_TraitData().TypeID;
			}
		}
	}

	if (Node.Get_Type() == UCodeLang::ClassType::Class)
	{
		const UCodeLang::Class_Data& classnode = Node.Get_ClassData();
		bool hastrait = false;
		for (auto& Item : classnode.InheritedTypes)
		{
			if (CompoentID.has_value())
			{
				if (CompoentID.value() == Item.TraitID)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool UCompiler::IsComponentTrait(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	if (!CompoentID.has_value())
	{
		if (auto node = Assembly.Find_Node((StringView)"Component", "UCodeGameEngine"))
		{
			if (node->Get_Type() == UCodeLang::ClassType::Trait)
			{
				CompoentID = node->Get_TraitData().TypeID;
			}
		}
	}

	if (Node.Get_Type() == UCodeLang::ClassType::Class)
	{
		const UCodeLang::Class_Data& classnode = Node.Get_ClassData();
		
		if (CompoentID.has_value())
		{
			return classnode.TypeID == CompoentID.value();
		}
	}

	return false;
}

bool UCompiler::IsAAsset(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	if (!AssetID.has_value())
	{
		if (auto node = Assembly.Find_Node((StringView)"Asset", "UCodeGameEngine"))
		{
			if (node->Get_Type() == UCodeLang::ClassType::Trait)
			{
				AssetID = node->Get_TraitData().TypeID;
			}
		}
	}

	if (Node.Get_Type() == UCodeLang::ClassType::Class)
	{
		const UCodeLang::Class_Data& classnode = Node.Get_ClassData();
		bool hastrait = false;
		for (auto& Item : classnode.InheritedTypes)
		{
			if (AssetID.has_value())
			{
				if (AssetID.value() == Item.TraitID)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool UCompiler::IsAssetTrait(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	if (!AssetID.has_value())
	{
		if (auto node = Assembly.Find_Node((StringView)"Component", "UCodeGameEngine"))
		{
			if (node->Get_Type() == UCodeLang::ClassType::Trait)
			{
				AssetID = node->Get_TraitData().TypeID;
			}
		}
	}

	if (Node.Get_Type() == UCodeLang::ClassType::Class)
	{
		const UCodeLang::Class_Data& classnode = Node.Get_ClassData();

		if (AssetID.has_value())
		{
			return classnode.TypeID == AssetID.value();
		}
	}

	return false;
}

bool UCompiler::IsAWindow(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	if (!WindowID.has_value())
	{
		if (auto node = Assembly.Find_Node((StringView)"Window", "UCodeGameEngineEditor"))
		{
			if (node->Get_Type() == UCodeLang::ClassType::Trait)
			{
				WindowID = node->Get_TraitData().TypeID;
			}
		}
	}

	if (Node.Get_Type() == UCodeLang::ClassType::Class)
	{
		const UCodeLang::Class_Data& classnode = Node.Get_ClassData();
		bool hastrait = false;
		for (auto& Item : classnode.InheritedTypes)
		{
			if (WindowID.has_value())
			{
				if (WindowID.value() == Item.TraitID)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool UCompiler::IsWindowTrait(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	if (!WindowID.has_value())
	{
		if (auto node = Assembly.Find_Node((StringView)"Window", "UCodeGameEngineEditor"))
		{
			if (node->Get_Type() == UCodeLang::ClassType::Trait)
			{
				WindowID = node->Get_TraitData().TypeID;
			}
		}
	}

	if (Node.Get_Type() == UCodeLang::ClassType::Class)
	{
		const UCodeLang::Class_Data& classnode = Node.Get_ClassData();
		
		if (WindowID.has_value())
		{
			return classnode.TypeID == WindowID.value();
		}
	}

	return false;
}


bool UCompiler::IsAExporter(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	if (!ExporterID.has_value())
	{
		if (auto node = Assembly.Find_Node((StringView)"Component", "UCodeGameEngineEditor"))
		{
			if (node->Get_Type() == UCodeLang::ClassType::Trait)
			{
				ExporterID = node->Get_TraitData().TypeID;
			}
		}
	}

	if (Node.Get_Type() == UCodeLang::ClassType::Class)
	{
		const UCodeLang::Class_Data& classnode = Node.Get_ClassData();
		bool hastrait = false;
		for (auto& Item : classnode.InheritedTypes)
		{
			if (ExporterID.has_value())
			{
				if (ExporterID.value() == Item.TraitID)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool UCompiler::IsExportTrait(const UCodeLang::AssemblyNode& Node, const UCodeLang::ClassAssembly& Assembly)
{
	if (!ExporterID.has_value())
	{
		if (auto node = Assembly.Find_Node((StringView)"Component", "UCodeGameEngineEditor"))
		{
			if (node->Get_Type() == UCodeLang::ClassType::Trait)
			{
				ExporterID = node->Get_TraitData().TypeID;
			}
		}
	}

	if (Node.Get_Type() == UCodeLang::ClassType::Class)
	{
		const UCodeLang::Class_Data& classnode = Node.Get_ClassData();

		if (ExporterID.has_value())
		{
			return classnode.TypeID == ExporterID.value();
		}
	}

	return false;
}



String UCompiler::NewComponentTemplate(const StringView componentname)
{
	StringView Tab = "  ";

	String r;
	r += "use ULang;\n";
	r += "use UCodeGameEngine;\n";

	r += "\n";

	r += "$";
	r += componentname;
	r += "[Component]:\n\n";

	r += Tab;
	r+= "//Called once when the script is loaded.\n";
	r += Tab;
	r += "|Start[this&] -> void:\n";

	r += Tab;
	r += " //\n\n\n";

	r += Tab;
	r += "//Called once every Frame.\n";
	r += Tab;
	r += "|Update[this&] -> void:\n";

	r += Tab;
	r += " //\n";


	return r;
}
String UCompiler::NewAssetTemplate(const StringView componentname)
{
	StringView Tab = "  ";

	String r;
	r += "use ULang;\n";
	r += "use UCodeGameEngine;\n";

	r += "\n";

	r += String("[MenuItem(\"New ") + String(componentname) + "\")]\n";

	r += "$";
	r += componentname;
	r += "[Asset]:\n\n";

	r += Tab;
	r+= "//Add Varables\n";
	r += Tab;
	r += "int Number = 0;\n";
	
	r += Tab;
	r += "String StringVal;\n";

	r += Tab;
	r += "bool? OptBool;\n";


	r += "//\n";

	return r;
}
String UCompiler::NewWindowTemplate(const StringView componentname)
{
	StringView Tab = "  ";

	String r;
	r += "use ULang;\n";
	r += "use UCodeGameEngine;\n";
	r += "use UCodeGameEngineEditor;\n";

	r += "\n";

	r += String("[MenuItem(\"New ") + String(componentname) + "\")]\n";
	r += "$";
	r += componentname;
	r += "[Window]:\n\n";

	r += Tab;
	r+= "//Add Varables\n";
	r += Tab;
	r += "int Number = 0;\n\n\n";
	r += Tab;

	r+= "//The EditorSave will automatically saved the varable when the editor closes\n";
	r += Tab;
	r += "//[EditorSave]\n";
	r += Tab;
	r += "int OtherNumber = 0;\n";

	r += Tab;
	r+= "//Called once when the Window is loaded.\n";
	r += Tab;
	r += "|Start[this&] -> void:\n";

	r += Tab;
	r += " //\n\n\n";

	r += Tab;
	r += "//Called once every Frame.\n";
	r += Tab;
	r += "|Update[this&] -> void:\n";

	r += Tab;
	r += " //\n";

	return r;
}
String UCompiler::NewCustomDrawTemplate(const StringView componentname)
{
	return String();
}
String UCompiler::NewExportTemplate(const StringView componentname)
{
	return String();
}
EditorEnd


