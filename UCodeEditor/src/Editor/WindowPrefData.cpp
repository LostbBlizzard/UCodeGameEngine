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
	output.Write("_Windows.size", (BitMaker::SizeAsBits)data._Windows.size());
	for (auto& Item : data._Windows)
	{
		WindowData::PushData(output, Item);
	}
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
	BitMaker::SizeAsBits WindowsCount = 0;
	input.ReadType("_Windows.size", WindowsCount, WindowsCount);

	Out._Windows.resize(WindowsCount);
	for (size_t i = 0; i < WindowsCount; i++)
	{
		WindowData::PullData(input, Out._Windows[i]);
	}
}

void WindowPrefData::WindowData::PushData(USerializer& USerializer, const WindowData& Data)
{
	USerializer.StartObject();
	
	USerializer.Write("_Windowid", Data._Windowid);
	USerializer.Write("_ImguiName", Data._ImguiName);

	if (USerializer.Get_Type() == USerializerType::YAML) 
	{
		
		String Tep;
		for (size_t i = 0; i < Data._WindowData.size(); i++)
		{
			Tep.push_back((char)Data._WindowData[i]);
		}
		USerializer.Write("_WindowData", Tep);
	}
	else
	{
		USerializer.Write("_WindowData", Data._WindowData);
	}

	USerializer.EndObject();
}

void WindowPrefData::WindowData::PullData(UDeserializer& USerializer,WindowData& Out)
{
	USerializer.StartObject();

	USerializer.ReadType("_Windowid", Out._Windowid, Out._Windowid);
	USerializer.ReadType("_ImguiName", Out._ImguiName, Out._ImguiName);

	if (USerializer.Get_Mode() == USerializerType::Bytes) {
		USerializer.ReadType("_WindowData", Out._WindowData, Out._WindowData);
	}
	else
	{
		String buffer;
		USerializer.ReadType("_WindowData", buffer, buffer);

		Out._WindowData.resize(buffer.size());
		memcpy(Out._WindowData.data(), buffer.data(), buffer.size());
	}
	USerializer.EndObject();
}
EditorEnd
