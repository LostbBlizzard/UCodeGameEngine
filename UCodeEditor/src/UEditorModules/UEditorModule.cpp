#include "UEditorModule.hpp"

#include "Modules/CodeModule.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "Modules/CoreModule.hpp"
#include <filesystem>
#include <UCodeRunTime/CoreSystems/StandardAssetLoader.hpp>
EditorStart

void UEditorModules::Init()
{

	static bool InitValue = false;


	if (InitValue == false)
	{
		InitValue = true;
		_Modules[0] =Unique_ptr<UEditorModule>(new CodeModule());
		_Modules[1] = Unique_ptr<UEditorModule>(new CoreModule());
	}
}

const Span<UEditorModule*> UEditorModules::GetModules()
{
	Init();

	return Span<UEditorModule*>((UEditorModule**)_Modules.data(),_Modules.size());
}

UEditorModules::FromExt UEditorModules::GetModuleFromFileExt(const Path& ExtWithDot)
{
	auto List = GetModules();
	for (size_t i = 0; i < List.Size(); i++)
	{
		auto& Item = List[i];
		auto Op = Item->GetAssetDataUsingExt(ExtWithDot);
		if (Op.has_value())
		{
			FromExt R;
			R.Index = Op;
			R.Ptr = Item;

			return R;
		}
	}
	return {};
}

UEditorComponentData* UEditorModules::GetComponentData(const UCode::UComponentsID& ID)
{
	auto List = GetModules();
	for (size_t i = 0; i < List.Size(); i++)
	{
		auto& Item = List[i];
		auto Op = Item->GetComponentData(ID);
		if (Op.has_value())
		{
			return Item->GetComponentsData()[Op.value()];
		}
	}
	return nullptr;
}

bool UEditorAssetFileData::Draw(UEditorAssetDataConext& Data, const Path& path)
{
	if (ImGuIHelper::ImageButton(Data.ObjectPtr, AppFiles::sprite::Entity, *(ImVec2*)&Data.ButtionSize))
	{

		return false;
	}
}

size_t GetPageSizeE()
{
	return 4096;
}


Result<ExportEditorReturn, ExportErrors> UEditorModule::ExportEditor(ExportEditorContext& Context)
{

	namespace fs = std::filesystem;

	auto Buffer = GetAssetData();

	ExportEditorReturn r;
	r.OutputModuleFile = (Context.TemporaryGlobalPath / this->ID).native() + Path(UCode::GameFilesData::FileExtWithDot).native();
	UCode::GameFilesData V;
	UCode::StandardAssetLoader::UIdMap IDMaps;
	
	fs::create_directories(r.OutputModuleFile.parent_path());

	for (const auto& dirEntry : fs::recursive_directory_iterator(Context.AssetPath))
	{
		if (dirEntry.is_regular_file()) 
		{
			auto& path = dirEntry.path();
			auto Relat = FileHelper::ToRelativePath(Context.AssetPath, path);
			auto v = GetAssetDataUsingExt(path.extension());
			if (v.has_value())
			{
				auto V1 = v.value();

				auto Buffer = GetAssetData();
				auto& AssetData = Buffer[V1];


				ExportFileContext Exfile;
				Exfile.ChashPath = Context.TemporaryGlobalPath / "Assets" / Relat;
				Exfile.Output = Exfile.ChashPath.native() + Path(".out").native();
				


				fs::create_directories(Exfile.Output.parent_path());

				ExportFileRet outfile;
				if (AssetData->CallLiveingAssetsWhenExport)
				{
					auto V = AssetData->GetMakeNewAssetFile();

					UEditorAssetFileInitContext initCon;
					V->FileFullPath = path;
					V->Init(initCon);

					outfile = V->ExportFile(Exfile);

				}
				else
				{
					outfile = AssetData->ExportFile(path, Exfile);
				}
				if (fs::exists(Exfile.Output)) {
					auto bytes = UCode::GameFiles::ReadFileAsBytes(Exfile.Output);
					UCode::GameFileIndex index;
					index.FileOffset = V._Data.size();
					index.FileFullName = Relat;
					index.FileSize = bytes.Size();

					if (outfile._UID.has_value())
					{
						IDMaps._Paths.AddValue(outfile._UID.value(),index.FileFullName);
					}


					V.Offsets.push_back(std::move(index));


					for (size_t i = 0; i < bytes.Size(); i++)
					{
						auto& Item = bytes[i];

						V._Data.push_back(Item);
					}
				}
			}
		}
	}


	auto v = ExportSystems(Context);

	if (v.IsError() && v.GetError().Errors.size())
	{
		return v.GetError();
	}
	
	if (v.IsValue()) {
		for (auto& Item : v.GetValue())
		{
			UCode::GameFilesData v;
			UCode::FileBuffer buffer;
			UCode::GameFilesData::ReadFileKeepOpen(Item.OutputModuleFile, buffer, v);

			for (auto& file : v.Offsets)
			{
				UCode::GameFileIndex index;
				index.FileOffset = V._Data.size();
				index.FileFullName = file.FileFullName;
				index.FileSize = file.FileSize;


				V.Offsets.push_back(std::move(index));

				buffer.Set_FileReadIndex(file.FileOffset);

				Vector<Byte> bytes;
				bytes.resize(file.FileSize);

				buffer.ReadBytes(bytes.data(), file.FileSize);
				for (size_t i = 0; i < bytes.size(); i++)
				{
					auto& Item = bytes[i];

					V._Data.push_back(Item);
				}
			}
		}
	}

	{
		UCode::USerializer serializer(USerializerType::Bytes);

		IDMaps.Serialize(serializer);
		
		auto bytes = serializer.Get_BitMaker().Get_Bytes();
		

		UCode::GameFileIndex index;
		index.FileOffset = V._Data.size();
		index.FileFullName = UCode::GameFilesData::GetUCodeDir() / UCode::StandardAssetLoader::UIdMap::FileWithDot;
		index.FileSize = bytes.size();

		V.Offsets.push_back(std::move(index));
		
		for (size_t i = 0; i < bytes.size(); i++)
		{
			auto& Item = bytes[i];

			V._Data.push_back(Item);
		}
	}

	{
		UCode::USerializer serializer(USerializerType::Bytes);

		UCode::GameFilesData::Serialize(V,serializer);

		serializer.ToFile(r.OutputModuleFile);
	}
	return r;
}
Optional<size_t> UEditorModule::GetAssetDataUsingExt(const Path& ExtWithDot)
{
	auto Buffer = GetAssetData();

	for (size_t i = 0; i < Buffer.Size(); i++)
	{
		auto& Item = Buffer[i];
		if (Item->FileExtWithDot == ExtWithDot)
		{
			return i;
		}
	}

	return {};
}

Optional<size_t> UEditorModule::GetComponentData(const UCode::UComponentsID& ID)
{
	auto Buffer = GetComponentsData();

	for (size_t i = 0; i < Buffer.Size(); i++)
	{
		auto& Item = Buffer[i];
		if (Item->CompoentType == ID)
		{
			return i;
		}
	}

	return {};
}

bool ExportChacheFile::ToFile(const Path& path, const ExportChacheFile& file)
{
	std::ofstream File;
	File.open(path, std::ios_base::binary);
	if (File.is_open())
	{
		auto Bits = ToBytes(file);
		File.write((const char*)Bits.Data(), Bits.Size());
		File.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool ExportChacheFile::FromFile(const Path& path, ExportChacheFile& file)
{
	std::ifstream File;
	File.open(path, std::ios::binary);
	if (File.is_open())
	{
		Unique_Bytes Bits;
		File.seekg(0, File.end);
		Bits.Resize(File.tellg());
		File.seekg(0, File.beg);
		File.read((char*)Bits.Data(), Bits.Size());

		return  FromBytes(Bits.AsView(), file);
	}
	else
	{
		return false;
	}
}
Unique_Bytes ExportChacheFile::ToBytes(const ExportChacheFile& file)
{
	BitMaker bits;

	bits.WriteType((BitMaker::SizeAsBits)file.Info.size());
	for (auto& Item : file.Info)
	{
		auto& _Value = Item.second;
		auto& _Key = Item.first;

		bits.WriteType(Path(_Key).generic_string());

		bits.WriteType(_Value.FileSize);

		bits.WriteType(_Value.FileLastUpdated);

		bits.WriteType(_Value.FileHash);
	}

	auto V = Unique_Bytes();
	V.Copyfrom(bits.Get_Bytes());
	return V;
}
bool ExportChacheFile::FromBytes(const BytesView& bytes, ExportChacheFile& file)
{
	BitReader bits;
	bits.SetBytes(bytes);
	BitMaker::SizeAsBits S = 0;
	bits.ReadType(S,S);

	file.Info.reserve(S);
	for (size_t i = 0; i < S; i++)
	{
		String V0;
		bits.ReadType(V0,V0);

		OldState V1;
		bits.ReadType(V1.FileSize,V1.FileSize);
		bits.ReadType(V1.FileLastUpdated, V1.FileLastUpdated);
		bits.ReadType(V1.FileHash, V1.FileHash);

		file.Info.AddValue(Path(V0).native(),V1);
	}

	return true;
}
ExportChacheFile::OldState ExportChacheFile::OldState::GetStateNofile(const Path& path, bool FileIsText)
{
	ExportChacheFile::OldState R;
	R.FileSize =std::filesystem::file_size(path);
	R.FileLastUpdated= std::filesystem::last_write_time(path).time_since_epoch().count();
	R.FileHash = 0;


	return R;
}

ExportChacheFile::GetState_t ExportChacheFile::OldState::GetState(const Path& path, bool FileIsText)
{
	ExportChacheFile::GetState_t R;
	R.State.FileSize = std::filesystem::file_size(path);
	R.State.FileLastUpdated = std::filesystem::last_write_time(path).time_since_epoch().count();
	R.State.FileHash = 0;


	return R;
}


EditorEnd

