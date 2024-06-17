#include "ColorData.hpp"
#include <UCodeRunTime/ULibrarys/Serialization_Library.h>
EditorStart


bool ColorData::WriteToFile(const  Path& Path, const ColorData& Data, USerializerType type)
{
	auto File = std::ofstream(Path);
	if (File.is_open())
	{
		USerializer output(type);

		ToBits(output, Data);

		//
		output.ToStream(File, false);

		File.close();
		return true;
	}
	return false;
}

void ColorData::ToBits(UCodeEditor::USerializer& output, const UCodeEditor::ColorData& Data)
{
	output.Write("_Color", Data._Color);
}

 bool ColorData::ReadFromFile(const  Path& Path, ColorData& Data, USerializerType type)
{
	 UDeserializer input;
	 if (!UDeserializer::FromFile(Path, input,type))
	 {
		 return false;
	 }
	 FromBits(input, Data);
	 return true;
}

 void ColorData::FromBits(UCodeEditor::UDeserializer& input, UCodeEditor::ColorData& Data)
 {

	 input.ReadType("_Color", Data._Color);
 }

EditorEnd
