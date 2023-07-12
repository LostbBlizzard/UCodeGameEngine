#pragma once
#include <UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp>
#include <Editor/EditorNamespace.hpp>
#include <Serialization.hpp>
#include <Typedefs.hpp>




EditorStart
class TextureData
{
public:
	struct SpriteData
	{
		//UCode::SpriteMetaData Meta;
		//UCode::SpriteSizeData Size;
	};	
	inline static const char* FileExt = "UImage";
	inline static const char* FileExtDot = ".UImage";

	UCode::UID _Value;
	UCode::Vector<SpriteData> _Sprites;

	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path& path, const TextureData& Data, USerializerType type=USerializerType::Default);
	static void ToBits(USerializer& output, const TextureData& Data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const Path& path, TextureData& Data);
	static void FromBits(UDeserializer& input, TextureData& Data);
};
EditorEnd

CoreStart


template<>
struct BitData<UCodeEditor::TextureData>
{
	using _Type = UCodeEditor::TextureData;
	static void ToBytes(BitMaker& This, const _Type& Value);
	static void FromBytes(BitReader& This, _Type& Out);
};


template<>
struct BitData<UCodeEditor::TextureData::SpriteData>
{
	using _Type = UCodeEditor::TextureData::SpriteData;
	static void ToBytes(BitMaker& This, const _Type& Value);
	static void FromBytes(BitReader& This, _Type& Out);
};

CoreEnd
