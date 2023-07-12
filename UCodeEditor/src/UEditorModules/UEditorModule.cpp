#include "UEditorModule.hpp"

#include "Modules/CodeModule.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "Modules/CoreModule.hpp"
#include <filesystem>
EditorStart

void UEditorModules::Init()
{

	static bool InitValue = false;


	if (InitValue == false)
	{
		InitValue = false;
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
optional<size_t> UEditorModule::GetAssetDataUsingExt(const Path& ExtWithDot)
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

optional<size_t> UEditorModule::GetComponentData(const UCode::UComponentsID& ID)
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
		File.write((const char*)Bits.Pointer.get(), Bits.Size);
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
		Bits.Size = File.tellg();
		File.seekg(0, File.beg);
		Bits.Pointer = std::make_unique<Byte[]>(Bits.Size);
		File.read((char*)Bits.Pointer.get(), Bits.Size);

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
		bits.WriteType(Item._Key.generic_string());

		bits.WriteType(Item._Value.FileSize);

		bits.WriteType(Item._Value.FileLastUpdated);

		bits.WriteType(Item._Value.FileHash);
	}

	auto V = Unique_Bytes();
	V.Pointer.reset(new Byte[bits.Size()]);
	V.Size = bits.Size();
	memcpy(V.Pointer.get(), bits.Get_Bytes().data(), bits.Size());
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

		file.Info.AddValue(V0, V1);
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

