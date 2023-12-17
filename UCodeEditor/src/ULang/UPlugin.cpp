#include "UPlugin.hpp"

#include <Serialization.hpp>

EditorStart

#define UPluginSIGNATURE "LBPlugin"
const char* FileSignature = UCodeGESignature;//T=char [30]
UCODE_EDITOR_NODISCARD bool UPlugin::WriteToFile(const Path& path, const UPlugin& Data)
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
UCODE_EDITOR_NODISCARD bool UPlugin::ReadFromFile(const  Path& path, UPlugin& Data)
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
UCODE_EDITOR_NODISCARD Unique_Bytes UPlugin::ToBits(const UPlugin& Data)
{
	BitMaker bit;
	bit.WriteType(UPluginSIGNATURE);
	bit.WriteType(Data.PluginName);
	bit.WriteType(Data.PluginVersion);
	bit.WriteType(Data.BuildNumber);

	bit.WriteType((BitMaker::SizeAsBits)Data.Offsets.size());
	for (auto& Item : Data.Offsets)
	{
		bit.WriteType(Item.FileFullName);
		bit.WriteType(Item.FileInternalID);
		bit.WriteType((BitMaker::SizeAsBits)Item.FileOffset);
		bit.WriteType((BitMaker::SizeAsBits)Item.FileSize);
	}

	bit.WriteType(Data._Data);

	Unique_Bytes R;
	R.Copyfrom(std::move(bit.Get_Bytes()));
	return R;
}
UCODE_EDITOR_NODISCARD bool UPlugin::FromBits(const BytesView input, UPlugin& Data)
{
	BitReader bit;
	bit.SetBytes(input);

	{
		String SIGNATURE;
		bit.ReadType(SIGNATURE, SIGNATURE);

		if (SIGNATURE != UPluginSIGNATURE)
		{
			return false;
		}
	}
	bit.ReadType(Data.PluginName, Data.PluginName);
	bit.ReadType(Data.PluginVersion, Data.PluginVersion);
	bit.ReadType(Data.BuildNumber, Data.BuildNumber);

	BitMaker::SizeAsBits Count = 0;
	bit.ReadType(Count, Count);
	Data.Offsets.reserve(Count);

	for (size_t i = 0; i < Count; i++)
	{
		FileIndex Item;
		bit.ReadType(Item.FileFullName, Item.FileFullName);
		bit.ReadType(Item.FileInternalID, Item.FileInternalID);
		bit.ReadType(Item.FileOffset, Item.FileOffset);
		bit.ReadType(Item.FileSize, Item.FileSize);

		Data.Offsets.push_back(std::move(Item));
	}

	return true;
}
EditorEnd