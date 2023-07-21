#include "UPluginManager.hpp"
#include <Serialization.hpp>
EditorStart
UCODE_EDITOR_NODISCARD bool PluginIndex::WriteToFile(const Path& path, const PluginIndex& Data)
{
	auto File = std::ofstream(path, std::ios_base::binary);
	if (File.is_open())
	{
		Unique_Bytes Bits = ToBits(Data);
		//
		File.write((char*)Bits.Data(), Bits.Size());

		File.close();
		return true;
	}
	return false;
}

UCODE_EDITOR_NODISCARD bool PluginIndex::ReadFromFile(const Path& path, PluginIndex& Data)
{
	auto File = std::ifstream(path, std::ios_base::binary);
	if (File.is_open())
	{
		Unique_Bytes Bits;
		File.seekg(0, File.end);
		Bits.Resize(File.tellg());
		File.seekg(0, File.beg);
		File.read((char*)Bits.Data(), Bits.Size());
		File.close();

		return FromBits(Bits.AsView(), Data);
	}
	return false;
}

UCODE_EDITOR_NODISCARD Unique_Bytes PluginIndex::ToBits(const PluginIndex& Data)
{
	BitMaker bit;
	bit.WriteType((BitReader::SizeAsBits)Data._Plugins.size());
	for (auto& Item : Data._Plugins)
	{
		bit.WriteType(Item.PluginName);
		bit.WriteType(Item.PluginVersion);
		bit.WriteType(Item.BuildNumber);
		bit.WriteType(Item.FilePath);
	}


	//
	Unique_Bytes R;
	R.Copyfrom(std::move(bit.Get_Bytes()));
	return R;
}

UCODE_EDITOR_NODISCARD bool PluginIndex::FromBits(const BytesView input,PluginIndex& Data)
{
	BitReader bit;
	bit.SetBytes(input);

	BitReader::SizeAsBits Size =0;
	bit.ReadType(Size, Size);

	Data._Plugins.reserve(Size);
	for (size_t i = 0; i < Size; i++)
	{
		PluginInfo Item;
		bit.ReadType(Item.PluginName, Item.PluginName);
		bit.ReadType(Item.PluginVersion, Item.PluginVersion);
		bit.ReadType(Item.BuildNumber, Item.BuildNumber);
		bit.ReadType(Item.FilePath, Item.FilePath);

		Data._Plugins.push_back(std::move(Item));
	}

	return true;
}


Path UPluginManager::GetPluginIndexPath()
{
	return UGlobalState::GetPluginDirectory() / PluginIndex::FileName;
}

Path UPluginManager::GetPluginDownloadsPath()
{
	return UGlobalState::GetPluginDirectory() / "Plugins";
}

PluginIndex UPluginManager::GetPluginIndex()
{
	PluginIndex r;
	PluginIndex::ReadFromFile(GetPluginIndexPath(), r);
	return r;
}

void UPluginManager::SavePluginIndex(const PluginIndex& Value)
{
	PluginIndex::WriteToFile(GetPluginIndexPath(), Value);
}

EditorEnd

