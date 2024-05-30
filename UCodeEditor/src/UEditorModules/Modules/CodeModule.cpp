#include "CodeModule.hpp"

#include "../ModuleNames.hpp"

#include "Assets/UCodeLang/ScriptableAssetFile.hpp"
#include "Assets/UCodeLang/UModuleFile.hpp"
#include "Assets/Art/UColorAsset.hpp"
#include "Assets/UCodeLang/UCodeAssetFile.hpp"
#include "Componets/ULangScript.hpp"

#include "EditorWindows/BasicWindows/ConsoleWindow.hpp"
#include "Helper/UserSettings.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "ULang/UCompiler.hpp"
#include "UCodeRunTime/CoreSystems/Threads.hpp"
#include "Helper/Tasks.hpp"
EditorStart

CodeModule::CodeModule()
{
	Init();
}

CodeModule::~CodeModule()
{

}

void CodeModule::Init()
{
	ID = UCODEEDITOR_MODULENAME_CODEANDSHADERS;

	
	{
		Assets[0] = std::move(Unique_ptr<UEditorAssetFileData>(new UCodeAssetFile()));
		Assets[1] = std::move(Unique_ptr<UEditorAssetFileData>(new UColorAssetFile()));
		Assets[2] = std::move(Unique_ptr<UEditorAssetFileData>(new ScriptableAssetFile()));
		Assets[3] = std::move(Unique_ptr<UEditorAssetFileData>(new UModuleFile()));
	}

	{
		Components[0] = std::move(Unique_ptr<UEditorComponentData>(new ULangScriptUEditorData()));
	}
}


Vector<ConsoleWindow::LogMessageID> LogErrors(ConsoleWindow* win,const UCodeLang::CompilationErrors& Error)
{
	auto Err = Error;//Get_Errors has not const type update this when it does

	Vector<ConsoleWindow::LogMessageID> r;
	r.resize(Err.Get_Errors().size());
	for (const auto& Item : Err.Get_Errors())
	{
		ConsoleWindow::LogType T = ConsoleWindow::LogType::Log;

		if (UCodeLang::CompilationErrors::IsError(Item._Code))
		{
			T = ConsoleWindow::LogType::Error;
		}
		else if (UCodeLang::CompilationErrors::IsWarning(Item._Code))
		{
			T = ConsoleWindow::LogType::Warning;
		}
		ConsoleWindow::Log Log;
		Log._Type = T;
		String LineText;


		LineText = Item.File.generic_string() + " " + (UCode::String)" OnLine:" + std::to_string(Item.Line);

		auto ItemToKeep = Item;//copy
		Path FullPath =Path(win->Get_ProjectData()->GetAssetsDir().native() + Path(Item.File).native());
		Log.Text = LineText;
		Log.MoreText = Item.ToString();
		Log._OnOpen = [FullPath = std::move(FullPath),ItemToKeep](ConsoleWindow& Win, ConsoleWindow::Log& log)
		{
			const auto& Settings = UserSettings::GetSettings();
			size_t Column = 0;
			
			String Args = UserSettings::GetOpenFileStringAsArgs(Settings, FullPath, ImGuIHelper_Asset::ProjectData->Get_ProjectDir(), ItemToKeep.Line, Column);
			String Exe = Settings.CodeEditorPath;
			FileHelper::OpenExe(Exe, Args);
		};

		String File = Item.File.generic_string();
		r.push_back(win->AddLog(Log));
	}

	return r;
}
void CodeModule::FilesUpdated(const Vector<FileUpdateData>& paths)
{
	bool IsUCodeFileUpdated = false;

	for (auto& Item : paths)
	{
		auto ext = Item.path.extension();
		if (ext == UCodeLang::FileExt::SourceFileWithDot
			|| ext == UCodeLang::ModuleFile::FileExtWithDot)
		{
			IsUCodeFileUpdated = true;
			break;
		}
	}


	if (IsUCodeFileUpdated) 
	{
		BuildUCode(true);
	}
}
void CodeModule::BuildUCode(bool IsInEditor)
{
	if (!RuningTasksInfo::HasTaskRuning(RuningTask::Type::BuildingUCodeFiles))
	{

		EditorAppCompoent* app = EditorAppCompoent::GetCurrentEditorAppCompoent();
		auto Threads = UCode::Threads::Get(app->GetGameRunTime()->Get_Library_Edit());


		std::function<void()> OnOtherThread = [Threads]()
			{
				EditorAppCompoent* app = EditorAppCompoent::GetCurrentEditorAppCompoent();
				UCodeLang::CompilationErrors _Errs;

				UCompiler::CompileData data;
				data.Set(app->Get_RunTimeProjectData());
				data.Error = &_Errs;
				data.Threads = Threads;
				data.Editor = true;
				data.OnStatusUpdate = std::make_shared<UCode::Mutex<UCompiler::CompileData::StatusUpdate>>([Threads](String str, size_t thread)
				{
					Threads->RunOnOnMainThread([str, thread]()
						{
							RuningTasksInfo::SetTaskStatus(RuningTask::Type::BuildingUCodeFiles, str, 30);
						});
				});

				bool ok = UCompiler::CompileProject(data);


				Threads->RunOnOnMainThread([data,ok,app, MovedErrs = std::move(_Errs)]()
					{
						{
							static Vector<ConsoleWindow::LogMessageID> oldlogids;
							
							auto Win = app->Get_Window<ConsoleWindow>();
							Win->RemoveLog(spanof(oldlogids));
							oldlogids = LogErrors(Win, MovedErrs);
							Win->FocusWindow();
						}

						
						RuningTasksInfo::ReMoveTask(RuningTask::Type::BuildingUCodeFiles);


						if (ok) {
							auto ULang = UCode::ULangRunTime::Get(app->GetGameRunTime()->Get_Library_Edit());
							auto& runtme = ULang->Get_State();

							UCodeLang::UClib lib;
							static Optional<UCode::ULangRunTime::LibID> Libid;
							

							if (UCodeLang::UClib::FromFile(&lib, data.OutPath))
							{
								
								if (!Libid.has_value())
								{
									UCode::ULangRunTime::Lib runlib;
									
									runlib.Ptr = std::make_unique<UCodeLang::UClib>(std::move(lib));
									runlib.RunPtr = std::make_unique<UCodeLang::RunTimeLib>();
									
									runlib.RunPtr->Init(runlib.Ptr.get());

									Libid = ULang->AddLib(std::move(runlib));

									ULang->ReLoadScripts();
								}
								else
								{
									auto& runlib = ULang->GetLib(Libid.value());

									runlib.RunPtr->UnLoad();
									*runlib.Ptr = std::move(lib);
									runlib.RunPtr->Init(runlib.Ptr.get());

									if (!ULang->HotReLoadScripts())
									{
										ULang->ReLoadScripts();
									}

									app->UpdateUAssemblyKey();
								}

							}
							else
							{
							
							}
						}
					});

			};
		RuningTask task;
		task.TaskType = RuningTask::Type::BuildingUCodeFiles;
		RuningTasksInfo::AddTask(task);

		Threads->AddTask_t(UCode::TaskType::FileIO, std::move(OnOtherThread), {}).Start();
	}
}

const Vector<const UCodeLang::AssemblyNode*>& CodeModule::GetAllVaildCompoents()
{
	auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();

	thread_local Vector<const UCodeLang::AssemblyNode*> vaildcompoents;
	thread_local Optional<ULangAssemblyID> myassemblykey;

	auto assemblykey = app->GetULangAssemblyID();

	bool isoutofdate = myassemblykey.has_value() ? assemblykey != myassemblykey.value() : false;

	if (isoutofdate) 
	{
		vaildcompoents.clear();
		myassemblykey = assemblykey;

		auto ULang = UCode::ULangRunTime::Get(app->GetGameRunTime()->Get_Library_Edit());
		auto& Assembly = ULang->Get_Assembly();

		for (const auto& Item : Assembly.Classes)
		{
			if (Item->Get_Type() == UCodeLang::ClassType::Class)
			{
				if (UCompiler::IsAComponent(*Item, Assembly))
				{
					vaildcompoents.push_back(Item.get());
				}
			}
		}
	}

	return vaildcompoents;
}

Result<Vector<ExportEditorReturn>, ExportErrors> CodeModule::ExportSystems(const ExportEditorContext& Context)
{
	namespace fs = std::filesystem;

	bool Is32bitmode = Context.settings.IfHasSettingStr("target.bitmode").value_or("32") == "32";

	UCodeLang::CompilationErrors errs;
	UCompiler::CompileData data;
	data.Error = &errs;
	data.Threads = nullptr;

	data.InPath = Context.AssetPath;
	data.IntPath = PathString(Context.TemporaryPlatfromPathSpecificPath / "ulang" / "int") + Path::preferred_separator;

	auto outpathdir = Context.TemporaryPlatfromPathSpecificPath / "ulang";
	data.OutPath = outpathdir / Path(Path("ucode").native() + Path(UCodeLang::FileExt::LibWithDot).native());
	
	std::filesystem::create_directories(outpathdir);
	
	Path modoutfile = Context.TemporaryGlobalPath / "ulang.data";
	ExportEditorReturn r;
	r.OutputModuleFile = modoutfile;

	if (UCompiler::CompileProject(data)) {

		UCode::GameFileIndex ind;
		ind.FileFullName = UCode::ULangRunTime::MainFile;
		ind.FileOffset = 0;
		ind.FileSize = fs::file_size(data.OutPath);

		UCode::GameFilesData files;
		files.Offsets.push_back(std::move(ind));


		{
			auto bytes = UCode::GameFiles::ReadFileAsBytes(data.OutPath);

			for (size_t i = 0; i < bytes.Size(); i++)
			{
				files._Data.push_back(bytes[i]);
			}
		}
		UCode::GameFilesData::MakeFile(files, r.OutputModuleFile, USerializerType::Bytes);


		return { {r} };
	}
	else
	{
		ExportErrors r;
		r.Errors.reserve(errs.Get_ErrorCount());

		for (auto& Item : errs.Get_Errors())
		{	
			r.AddError(Item.File, Item._Msg,Item.Line);
		}

		return r;
	}
}

EditorEnd