#pragma once


#include "Editor/EditorNamespace.hpp"
#include <UCodeRunTime/ULibrarys/Serialization/USerializer.hpp>
#include <Serialization.hpp>
#include <Typedefs.hpp>
#include <UCodeRunTime/ULibrarys/Serialization/SerlizeableType.hpp>

EditorStart
struct WindowDataForWindowPrefData
{
public:
	String _Windowid;
	String _WindowData;
	String _ImguiName;
};
EditorEnd

MakeSerlizeType(UCodeEditor::WindowDataForWindowPrefData,
		Field("_Windowid", _this->_Windowid);
		Field("_WindowData", _this->_WindowData);
		Field("_ImguiName", _this->_ImguiName);
);

EditorStart
class WindowPrefData
{
public:
	static const char* FileName;
	
	Vector<WindowDataForWindowPrefData> _Windows;

	

	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path& Path, const WindowPrefData& data,USerializerType type = USerializerType::Default);
	static void ToBits(USerializer& output, const WindowPrefData& data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const Path& Path, WindowPrefData& Out);
	static void FromBits(UDeserializer& input, WindowPrefData& Out);
};

EditorEnd