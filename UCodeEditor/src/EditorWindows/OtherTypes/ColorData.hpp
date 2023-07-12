#pragma once
#include <UCodeRunTime/RunTimeBasicTypes.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
#include <Editor/EditorNamespace.hpp>
#include <Typedefs.hpp>

#include "Serialization.hpp"
EditorStart
class ColorData
{
public:
	inline static const char* FileExt = "UColor";
	inline static const char* FileExtDot = ".UColor";

	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path&  path, const ColorData& Data, USerializerType type);
	static void ToBits(USerializer& output, const ColorData& Data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const  Path& Path, ColorData& Data);

	static void FromBits(UDeserializer& input,ColorData& Data);

	UCode::Color _Color;
};
EditorEnd

