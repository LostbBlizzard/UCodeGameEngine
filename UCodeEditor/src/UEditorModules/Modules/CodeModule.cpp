#include "CodeModule.hpp"

#include "UCodeLang/LangCore.hpp"
#include "../ModuleNames.hpp"

#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"

#include "EditorWindows/OtherTypes/ColorData.hpp"
#include "Helper/UserSettings.hpp"

#include "ULang/UCompiler.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "Helper/Tasks.hpp"

#include <chrono>
#include <thread>
#include "EditorWindows/BasicWindows/ConsoleWindow.hpp"
#include "ULang/UCodeDrawer.hpp"
#include "EditorWindows/DragAndDropNames.hpp"
EditorStart

class UCodeAssetFile :public UEditorAssetFileData
{
public:
	UCodeAssetFile()
	{
		FileExtWithDot = UCodeLang::FileExt::SourceFileWithDot;
	}
	virtual bool Draw(UEditorAssetDataConext& Data, const Path& path) override
	{
		bool Value = ImGuIHelper::ImageButton(Data.ObjectPtr, AppFiles::sprite::ULangScript, *(ImVec2*)&Data.ButtionSize);

		ImGuiDragDropFlags src_flags = 0;
		//src_flags |= ImGuiDragDropFlags_::ImGuiDragDropFlags_AcceptBeforeDelivery;
		if (ImGui::BeginDragDropSource(src_flags))
		{
			ImGui::Text("Moving .uc");
			auto Tep = &path;
			ImGui::SetDragDropPayload(DragAndDropType_ULangFilePath,&Tep, sizeof(const Path*));
			ImGui::EndDragDropSource();
		}
	
		if (Value)
		{	const auto& Settings = UserSettings::GetSettings();
			String Args = UserSettings::GetOpenFileStringAsArgs(Settings, path, ImGuIHelper_Asset::ProjectData->Get_ProjectDir(), 0, 0);
			String Exe = Settings.CodeEditorPath;
			FileHelper::OpenExe(Exe, Args);
		}
		return false;
	}
};

class ULangScriptUEditorData : public UEditorComponentData
{
public:
	ULangScriptUEditorData()
	{
		CompoentType = UCode::ULangScript::Get_TypeID();
	}


	void DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const override
	{
		UCode::ULangScript* Component = (UCode::ULangScript*)Value;

		if (Component->HasScript())
		{
			void* Ptr = Component->Get_UObj();
			auto ClassType = Component->Get_ClassData();
			auto& Assembly = Component->GetULang()->Get_Assembly();

			UCodeDrawer::DrawClass(Ptr, *ClassType, Assembly);
		}
		else
		{
			if (Component->Get_ClassData())
			{
				Component->ReLoad();
			}
		}
	}

	void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const override
	{
		UCode::ULangScript* Component = (UCode::ULangScript*)Value;
		{
			ImGui::BeginDisabled(true);


			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });
			ImGui::SameLine();

			String TypeStr = "Compoent/ULangScript";
			if (Component->HasClass())
			{
				TypeStr += "/" + Component->GetClassName();
			}
			ImGuIHelper::InputText("Type", TypeStr);
			ImGui::EndDisabled();
			ImGui::SameLine();
			bool V = Component->Get_IsActive();
			ImGuIHelper::ToggleField("Active", V);
			Component->Set_CompoentActive(V);
			
		}
		auto ULang = Component->GetULang();

		if (!Component->HasScript())
		{
			if (Component->HasClass())
			{
				Component->ReLoad();
				return;
			}
			auto& Assembly = ULang->Get_Assembly();
			

			for (const auto& Item : Assembly.Classes)
			{
				if (Item->Get_Type() == UCodeLang::ClassType::Class) 
				{
					if (UCompiler::IsAComponent(*Item,Assembly))
					{
						if (ImGui::MenuItem(Item->FullName.size() ? Item->FullName.c_str() : "noname"))
						{
							Component->LoadScript(Item.get());
						}
					}
				}
			}
		}
		else
		{
			auto& Assembly = ULang->Get_Assembly();
			void* Ptr = Component->Get_UObj();
			auto ClassType = Component->Get_ClassData();

			UCodeDrawer::DrawClass(Ptr,*ClassType,Assembly);
		}
	}


};

class UColorAssetFile :public UEditorAssetFileData
{
public:
	UColorAssetFile()
	{
		FileExtWithDot = ColorData::FileExtDot;
		CanHaveLiveingAssets = true;
	}

	class LiveingColor :public UEditorAssetFile
	{
	public:
		ColorData _ColorInfoFromFile;
		Unique_ptr<UCode::Texture> _Texture;
		bool WasUpdated = true;
		LiveingColor()
		{
			
		}

		void Init(const UEditorAssetFileInitContext& Context) override
		{
			ColorData::ReadFromFile(FileFullPath, _ColorInfoFromFile);

			UCode::Color32 Color32 = (UCode::Color32)_ColorInfoFromFile._Color;
			_Texture = Unique_ptr<UCode::Texture>(new UCode::Texture(1,1,&Color32));
		}
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override
		{
			ColorData::WriteToFile(FileFullPath, _ColorInfoFromFile,UCode::USerializerType::Readable);

		}
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override
		{
			if (WasUpdated)
			{
				UCode::Color32 Color32 = (UCode::Color32)_ColorInfoFromFile._Color;
				_Texture = Unique_ptr<UCode::Texture>(new UCode::Texture(1, 1, &Color32));
			}

			return ImGuIHelper::ImageButton(Item.ObjectPtr,_Texture.get(), *(ImVec2*)&Item.ButtionSize);
		}
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override
		{
			ImGui::BeginDisabled(true);


			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });
			ImGui::SameLine();

			String tep ="Color";
			Item.Drawer->StringField("Type",tep);

			String tep2 = FileFullPath.filename().replace_extension("").generic_string();
			Item.Drawer->StringField("Name",tep2);
			

			ImGui::EndDisabled();

			ImGui::Separator();

			if (Item.Drawer->ColorField("Color", _ColorInfoFromFile._Color))
			{
				WasUpdated = true;
			}
		}
	};

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr< UEditorAssetFile>(new LiveingColor());
	}

};



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
	}

	{
		Components[0] = std::move(Unique_ptr<UEditorComponentData>(new ULangScriptUEditorData()));
	}
}


void LogErrors(ConsoleWindow* win,const UCodeLang::CompilationErrors& Error)
{
	auto Err = Error;//Get_Errors has not const type update this when it does

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
		win->AddLog(Log);
	}
}
void CodeModule::FilesUpdated(const Vector<FileUpdateData>& paths)
{
	bool IsUCodeFileUpdated = false;

	for (auto& Item : paths)
	{
		if (Item.path.extension() == UCodeLang::FileExt::SourceFileWithDot)
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



				bool ok = UCompiler::CompileProject(data);


				Threads->RunOnOnMainThread([data,ok,app, MovedErrs = std::move(_Errs)]()
					{
						auto Win = app->Get_Window<ConsoleWindow>();
						LogErrors(Win, MovedErrs);
						Win->FocusWindow();
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

		Threads->AddTask_t(UCode::TaskType::FileIO, std::move(OnOtherThread), {});
	}
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
			ExportError v;
			
			r.AddError(Item.File, Item._Msg);
		}

		return r;
	}
}

EditorEnd