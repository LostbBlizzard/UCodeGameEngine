#include "WindowPrefData.hpp"


EditorStart
const char* WindowPrefData::FileName = "WindowPrefData.Pref";
bool WindowPrefData::WriteToFile(const Path& Path, const WindowPrefData& data, USerializerType type)
{
	auto File = std::ofstream(Path);
	if (File.is_open())
	{
		USerializer output(type);


		ToBits(output, data);

		//
		output.ToStream(File, true);
		
		File.close();
		return true;
	}
	return false;
}

void WindowPrefData::ToBits(USerializer& output, const WindowPrefData& data)
{
	output.Write("_Windows", data._Windows);	
}

bool WindowPrefData::ReadFromFile(const Path& Path, WindowPrefData& Out)
{
	UDeserializer input;
	if (!UDeserializer::FromFile(Path, input))
	{
		return false;
	}

	FromBits(input, Out);

	return true;
}

void WindowPrefData::FromBits(UDeserializer& input, WindowPrefData& Out)
{
	input.ReadType("_Windows", Out._Windows, Out._Windows);
}

EditorEnd
