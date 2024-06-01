#include "ScriptableAssetFile.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "ULang/UCompiler.hpp"
#include "ULang/UCodeDrawer.hpp"
EditorStart
ScriptableAssetFile::ScriptableAssetFile()
{
	FileExtWithDot = UC::ScirptableObjectData::FileExtDot;
	CanHaveLiveingAssets = true;
	CallLiveingAssetsWhenUpdated = true;
}
ScriptableAssetFile::Liveing::Liveing()
{

}
ScriptableAssetFile::Liveing::~Liveing()
{
	if (_ScriptKey.has_value()) {
		UC::UCodeRunTimeState::Get_Current()->RemoveScript(_ScriptKey.value());
	}
}
void ScriptableAssetFile::Liveing::Init(const UEditorAssetFileInitContext& Context)
{
	if (!UC::ScirptableObjectData::FromFile(_Data, FileFullPath))
	{
		UCodeGEError("Opening Asset for " << FileFullPath << " Failed");
	}
}
void ScriptableAssetFile::Liveing::TryLoadAsset()
{
	if (!_ScriptKey.has_value()) 
	{
		_Object.LoadScript(_Data);
		_InFile = _Data;

		UC::ULangRunTime::ScriptInfo info;
		info.ObjPtr = _Object.Get_UObjPtr();
		info.ScriptType = _Object.Get_ClassDataPtr();
		info.SetScriptType = [this](const UCodeLang::AssemblyNode* nod)
			{
				_Object.LoadScript(nod);
			};
		info.PreReload = [this]()
			{
				_Object.SaveTo(_ReloadBufferObject, USerializerType::BytesSafeMode);
				_Object.UnLoadScript();
				return nullptr;
			};
		info.PostReload = [this](void* ptr)
			{
				_Object.LoadScript(_ReloadBufferObject);
			};

		_ScriptKey = UC::UCodeRunTimeState::Get_Current()->AddScript(std::move(info));
	}
}
void ScriptableAssetFile::Liveing::FileUpdated()
{
	if (justsaved)
	{
		justsaved = false;
		return;
	}

	if (!UC::ScirptableObjectData::FromFile(_Data, FileFullPath))
	{
		UCodeGEError("Opening Asset for " << FileFullPath << " Failed");
	}
	_Object.LoadScript(_Data);
	_InFile = _Data;
}
void ScriptableAssetFile::Liveing::SaveFile(const UEditorAssetFileSaveFileContext& Context)
{
	if (!_Object.HasScript()) { return; }
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	USerializerType type = runprojectdata->Get_ProjData()._SerializeType;
	_Object.SaveTo(_Data, type);

	bool hasdiff = _Data._DataSerializeType != _InFile._DataSerializeType
		|| _Data._ObjectType != _InFile._ObjectType
		|| _Data._Data != _InFile._Data;

	if (hasdiff)
	{
		_InFile = _Data;
		if (!UC::ScirptableObjectData::ToFile(FileFullPath, _Data, type))
		{
			auto p = FileFullPath.generic_string();
			auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());

			UCodeGEError("Saveing Asset for " << relpath << " Failed");
		}
		else
		{
			auto& index = runprojectdata->Get_AssetIndex();

			auto p = FileFullPath.generic_string();
			auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
			auto op = index.FindFileRelativeAssetName(relpath);

			if (op.has_value())
			{
				auto& ind = op.value();
				ind.FileLastUpdatedTime = std::filesystem::last_write_time(FileFullPath).time_since_epoch().count();
				ind.FileSize = std::filesystem::file_size(FileFullPath);
			}
		}
		justsaved = true;
	}
}
bool ScriptableAssetFile::Liveing::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::ScirptableObjectFile, *(ImVec2*)&Item.ButtionSize);;
}
void ScriptableAssetFile::Liveing::DrawInspect(const UEditorAssetDrawInspectContext& Item)
{
	ImGui::BeginDisabled(true);


	ImGuIHelper::Image(AppFiles::sprite::ScirptableObjectFile, { 20 ,20 });
	ImGui::SameLine();

	String tep = "ScirptableObject/";
	tep += _Object.GetClassName();
	ImGuIHelper::InputText("Type", tep);

	String tep2 = FileFullPath.filename().replace_extension("").generic_string();
	ImGuIHelper::InputText("Name", tep2);


	ImGui::EndDisabled();

	ImGui::Separator();
	
	TryLoadAsset();
	//
	if (!_Object.HasScript())
	{
		if (_Object.HasClass())
		{
			//_Object.ReLoad();
			return;
		}
		auto& Assembly = UC::UCodeRunTimeState::Get_Current()->Get_Assembly();


		for (const auto& Item : Assembly.Classes)
		{
			if (Item->Get_Type() == UCodeLang::ClassType::Class)
			{
				if (UCompiler::IsAAsset(*Item, Assembly))
				{
					if (ImGui::MenuItem(Item->FullName.size() ? Item->FullName.c_str() : "noname"))
					{
						_Object.LoadScript(Item.get());
					}
				}
			}
		}
	}
	else
	{
		auto& Assembly = UC::UCodeRunTimeState::Get_Current()->Get_Assembly();
		void* Ptr = _Object.Get_UObj();
		auto ClassType = _Object.Get_ClassData();

		UCodeDrawer::DrawClass(Ptr, *ClassType, Assembly);
	}
	//
}
ExportFileRet ScriptableAssetFile::ExportFile(const Path& path, const ExportFileContext& Item)
{
	std::filesystem::copy_file(path, Item.Output, std::filesystem::copy_options::overwrite_existing);

	UCode::ScirptableObjectData V;
	UCode::ScirptableObjectData::FromFile(V, path);

	UC::ScirptableObject _Object;
	_Object.LoadScript(V);

	_Object.SaveTo(V, USerializerType::Bytes);

	//UCode::ScirptableObjectData::ToFile(Item.Output, V, USerializerType::Bytes);

	ExportFileRet r;
	r._UID = V._UID;
	r.outpath = Item.Output;
	return r;
}
Optional<GetUIDInfo> ScriptableAssetFile::GetFileUID(UEditorGetUIDContext& context)
{
	UC::ScirptableObjectData setting;

	if (UC::ScirptableObjectData::FromFile(setting, context.AssetPath))
	{
		GetUIDInfo val;
		val._MainAssetID = setting._UID;
		return val;
	}
	return {};
}
EditorEnd


