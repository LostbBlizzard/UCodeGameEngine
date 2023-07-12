#include "ImageData.hpp"
#include <UCodeRunTime/ULibrarys/Serialization_Library.h>

namespace YAML 
{
	
}

EditorStart

bool TextureData::WriteToFile(const Path& Path, const TextureData& Data, USerializerType type)
{
	auto File = std::ofstream(Path);
	if (File.is_open())
	{
		USerializer output(type);

		ToBits(output, Data);
		//
		output.ToStream(File, true);

		File.close();
		return true;
	}
	return false;
}

void TextureData::ToBits(USerializer& output, const TextureData& Data)
{
	output.Write("_Value", Data._Value);
	//output.Write("_Sprites", Data._Sprites);
}

bool TextureData::ReadFromFile(const Path& Path, TextureData& Data)
{
	std::ifstream File(Path);
	if (File.is_open()) 
	{
		UDeserializer input;
		FromBits(input, Data);

		File.close();
		return true;
	}
	return false;
}

void TextureData::FromBits(UCodeEditor::UDeserializer& input, UCodeEditor::TextureData& Data)
{
	input.ReadType("_Value", Data._Value);
	//input.ReadType("_Sprites", Data._Sprites);
}

EditorEnd


CoreStart
void BitData<UCodeEditor::TextureData>::ToBytes(BitMaker& This, const _Type& Value)
{
	UCODE_ENGINE_IMPlEMENTED_LATER;
	
	//This.WriteType(Value._Value);
	//This.WriteType(Value._Sprites);
}

void BitData<UCodeEditor::TextureData>::FromBytes(BitReader& This, _Type& Out)
{
	UCODE_ENGINE_IMPlEMENTED_LATER;
	//This.ReadType(Out._Value);
	//This.ReadType(Out._Sprites);
}
void BitData<UCodeEditor::TextureData::SpriteData>::ToBytes(BitMaker& This, const _Type& Value)
{
	UCODE_ENGINE_IMPlEMENTED_LATER;
	//This.WriteType(Value._Value);
	//This.WriteType(Value._Sprites);
}

void BitData<UCodeEditor::TextureData::SpriteData>::FromBytes(BitReader& This, _Type& Out)
{
	UCODE_ENGINE_IMPlEMENTED_LATER;
	//This.ReadType(Out._Value);
	//This.ReadType(Out._Sprites);
}
CoreEnd
