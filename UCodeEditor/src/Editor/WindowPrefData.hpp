#pragma once


#include "Editor/EditorNamespace.hpp"
#include <UCodeRunTime/ULibrarys/Serialization/USerializer.hpp>
#include <Serialization.hpp>
#include <Typedefs.hpp>
EditorStart
class WindowPrefData
{
public:
	static const char* FileName;
	struct WindowData
	{
	public:
		String _Windowid;
		Vector<Byte> _WindowData;
		String _ImguiName;
		static void PushData(USerializer& USerializer,const WindowData& Data);
		static void PullData(UDeserializer& USerializer,WindowData& Out);
	};
	Vector< WindowData> _Windows;

	

	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path& Path, const WindowPrefData& data,USerializerType type = USerializerType::Default);
	static void ToBits(USerializer& output, const WindowPrefData& data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const Path& Path, WindowPrefData& Out);
	static void FromBits(UDeserializer& input, WindowPrefData& Out);
};

EditorEnd