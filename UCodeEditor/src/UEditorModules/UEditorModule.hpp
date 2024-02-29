#pragma once

#include "../Typedefs.hpp"
//#include "ProjectManagement/EditorIndex.hpp"
#include "UCodeRunTime/Core/UComponentsID.hpp"
#include "EditorWindows/BasicWindows/InspectWindow.hpp"
EditorStart

struct ExportChacheFile
{
	struct GetState_t;
	struct OldState
	{
		u64 FileSize;
		u64 FileLastUpdated;
		u64 FileHash;

		
		
		static OldState GetStateNofile(const Path& path, bool FileIsText);
		static GetState_t GetState(const Path& path, bool FileIsText);
		bool operator==(const OldState& Other) const
		{
			return FileSize == Other.FileSize
				&& FileLastUpdated == Other.FileLastUpdated
				&& FileHash == Other.FileHash;
		}
		bool operator!=(const OldState& Other) const
		{
			return !(this->operator==(Other));
		}
	};
	struct DidnotOpenFile {};
	struct GetState_t
	{

		OldState State;
		Variant<DidnotOpenFile, Byte, String> File;
	};
	
	static bool ToFile(const Path& path, const ExportChacheFile& file);
	
	static bool FromFile(const Path& path, ExportChacheFile& file);
	static Unique_Bytes ToBytes(const ExportChacheFile& file);
	static bool FromBytes(const BytesView& bytes, ExportChacheFile& file);

	Unordered_map<PathString, OldState> Info;
};

struct UEditorAssetDataConext
{
	void* ObjectPtr = nullptr;
	Vec2 ButtionSize;
	std::function<UID()> _newuid;
};

struct ExportFileContext
{
	Path Output;
	Path ChashPath;
};

struct ExportSetting
{
	//target.bitmode:[64,32]
	//target.platform[windows,linux,mac,other]
	String Tag;
	Optional<String> Value;



	const Optional<bool> Asbool() const
	{
		if (Value.has_value())
		{
			auto& val = Value.value();
			if (val == "true" || val == "false")
			{
				return val == "true";
			}
			return {};
		}
		return {};
	}
	const Optional<int> AsInt() const
	{
		if (Value.has_value())
		{
			char* endptr;
			int result = strtol(Value.value().c_str(), &endptr, 10);
			if (*endptr != '\0') {
				return {};
			}
			return result;
		}
		return {};
	}
	const Optional<StringView> AsString() const
	{
		if (Value.has_value())
		{
			return Value.value();
		}
		return {};
	}
};

struct ExportSettings
{
	Vector<ExportSetting> Settings;

	const ExportSetting* IfHasSetting(const StringView tag) const
	{
		for (auto& Item : Settings)
		{
			if (Item.Tag == tag)
			{
				return &Item;
			}
		}
		return nullptr;
	}

	Optional<int> IfHasSettingInt(const StringView tag) const
	{
		if (auto v = IfHasSetting(tag))
		{
			return v->AsInt();
		}
		return {};
	}
	Optional<bool> IfHasSettingBool(const StringView tag) const
	{
		if (auto v = IfHasSetting(tag))
		{
			return v->Asbool();
		}
		return {};
	}
	Optional<StringView> IfHasSettingStr(const StringView tag) const
	{
		if (auto v = IfHasSetting(tag))
		{
			return v->AsString();
		}
		return {};
	}

	ExportSetting* HasSetting(const StringView tag)
	{
		for (auto& Item : Settings)
		{
			if (Item.Tag == tag)
			{
				return &Item;
			}
		}
		return nullptr;
	}

	void AddSetting(const StringView setting)
	{

		ExportSetting set;
		set.Tag = setting;

		Settings.push_back(std::move(set));
	}
	void AddSetting(const StringView setting, int value)
	{
		ExportSetting set;
		set.Tag = setting;
		set.Value = std::to_string(value);

		Settings.push_back(std::move(set));
	}
	void AddSetting(const StringView setting, const StringView value)
	{
		ExportSetting set;
		set.Tag = setting;
		set.Value = value;

		Settings.push_back(std::move(set));
	}
	void AddSetting(const StringView setting, bool value)
	{
		ExportSetting set;
		set.Tag = setting;
		set.Value = value ? "true" : "false";

		Settings.push_back(std::move(set));
	}
};

struct ExportEditorContext
{
	Path AssetPath;
	Path TemporaryGlobalPath;
	Path TemporaryPlatfromPathSpecificPath;
	ExportChacheFile* ChachInfo = nullptr;
	ExportSettings settings;
};
struct ExportEditorReturn
{
	Path OutputModuleFile;//is GameFile
};


struct UEditorAssetFileInitContext
{

};
struct UEditorAssetFileSaveFileContext
{

};

struct UEditorAssetDrawInspectContext
{
	InspectWindow::InspectDrawer* Drawer;

	std::function<UID()> _newuid;

	UID GetNewUID()
	{
		return _newuid();
	}
};
struct UEditorAssetDrawButtionContext
{
	void* ObjectPtr = nullptr;
	Vec2 ButtionSize;
	std::function<UID()> _newuid;
};
struct UEditorGetUIDContext
{
	Path AssetPath;

	std::function<UID()> _newuid;

	UID GetNewUID()
	{
		return _newuid();
	}
};

struct ExportFileRet
{
	bool WasUpdated = false;
	Optional<UID> _UID;
};
class UEditorAssetFile
{
public:
	Path FileFullPath;
	Optional<Path> FileMetaFullPath;//if  UEditorAssetFileData::FileMetaExtWithDot HasValue
	Path TemporaryPath;

	virtual ~UEditorAssetFile()
	{

	}

	virtual void Init(const UEditorAssetFileInitContext& Context)
	{

	}

	virtual void SaveFile(const  UEditorAssetFileSaveFileContext& Context)
	{

	}

	virtual bool DrawButtion(const UEditorAssetDrawButtionContext& Item)
	{
		return false;
	}

	virtual void DrawInspect(const UEditorAssetDrawInspectContext& Item)
	{

	}

	//called only if UEditorAssetFileData::CallLiveingAssetsWhenUpdated
	virtual void FileUpdated()
	{

	}

	//called only if UEditorAssetFileData::CallLiveingAssetsWhenMetaWasUpdated
	virtual void MetaFileUpdated()
	{

	}
	virtual ExportFileRet ExportFile(const ExportFileContext& Item)
	{
		return ExportFileRet();
	}
};



class UEditorAssetFileData
{
public:
	Path FileExtWithDot;
	Optional<Path> FileMetaExtWithDot;
	bool CanHaveLiveingAssets = false;


	bool CallLiveingAssetsWhenUpdated = false;
	bool CallLiveingAssetsWhenMetaWasUpdated = false;
	bool CallLiveingAssetsWhenExport = false;
	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile()
	{
		return {};
	}

	virtual void FileUpdated(const Path& path)
	{

	}


	//Called if CanHaveLiveingAssets is false
	virtual bool Draw(UEditorAssetDataConext& Data, const Path& path);


	//Called if FileMetaExtWithDot HasValue
	virtual void MetaFileInit(const Path& path)
	{

	}
	virtual void MetaFileUpdated(const Path& path)
	{

	}
	virtual ExportFileRet ExportFile(const Path& path,const ExportFileContext& Item)
	{
		return  ExportFileRet();
	}

	virtual Optional<UID> GetFileUID(UEditorGetUIDContext& context)
	{
		return {};
	}

	virtual void OnFileRemoved(const Path& path)
	{

	}

	
	virtual void OnFileRenamed(const Path& oldpath,const Path& newpath)
	{

	}

	inline const static String DefaultMetaFileExtWithDot = ".meta";
};



struct UEditorComponentDrawData
{
	InspectWindow::InspectDrawer* Draw = nullptr;
};

struct UEditorComponentDrawTreeData
{

};

class UEditorComponentData
{
public:
	String CompoentType;

	virtual void DrawAsTree(const UEditorComponentDrawTreeData& Data,UCode::Compoent* Value) const
	{

	}
	virtual void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const
	{

	}
};

using EditorModuleID = String;

struct ExportError
{
	String message;

	Optional<Path> filepath;
	Optional<size_t> lineNumber;
};
struct ExportErrors
{
	Vector<ExportError> Errors;
	void Add(ExportErrors&& error)
	{
		for (auto& Item : error.Errors) 
		{
			Errors.push_back(std::move(Item));
		}
	}
	void AddError(const Path& path,const StringView message)
	{
		ExportError v;
		v.filepath = path;
		v.message = message;

		Errors.push_back(std::move(v));
	}
	void AddError(const Path& path, const StringView message,size_t LineNumber)
	{
		ExportError v;
		v.filepath = path;
		v.message = message;
		v.lineNumber = LineNumber;

		Errors.push_back(std::move(v));
	}
	void AddError(const StringView message)
	{
		ExportError v;
		v.message = message;

		Errors.push_back(std::move(v));
	}
};

class UEditorModule
{
public:
	UEditorModule(){}
	virtual ~UEditorModule(){}

	EditorModuleID ID;
	Vector<EditorModuleID> Dependencies;



	virtual const Span<UEditorAssetFileData*> GetAssetData()
	{
		return {};
	}
	virtual const Span<UEditorComponentData*> GetComponentsData()
	{
		return {};
	}
	struct FileUpdateData
	{
		Path path;
		ChangedFileType type;
	};
	void FileUpdate(const FileUpdateData& file)
	{
		Vector < FileUpdateData> Tep;
		Tep.push_back(file);
		FilesUpdated(Tep);
	}

	

	virtual void FilesUpdated(const Vector<FileUpdateData>& paths)
	{

	}

	virtual Result<Vector<ExportEditorReturn>,ExportErrors> ExportSystems(const ExportEditorContext& Context)
	{
		return {};
	}

	Result<ExportEditorReturn, ExportErrors> ExportEditor(ExportEditorContext& Context);
	
	Optional<size_t> GetAssetDataUsingExt(const Path& ExtWithDot);
	Optional<size_t> GetComponentData(const UCode::UComponentsID& ID);
private:
};


class UEditorModules
{
public:
	static void Init();
	static const Span<UEditorModule*> GetModules();


	struct FromExt
	{
		UEditorModule* Ptr =nullptr;
		Optional<size_t> Index;
	};
	static  FromExt GetModuleFromFileExt(const Path& ExtWithDot);

	static UEditorComponentData* GetComponentData(const UCode::UComponentsID& ID);
private:
	inline static Array<Unique_ptr<UEditorModule>, 2> _Modules;
};
EditorEnd

