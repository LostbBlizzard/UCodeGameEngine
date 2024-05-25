#include "TilePaletteData.hpp"

namespace YAML
{
	template<>
    struct convert<UCodeEditor::TileData>
    {
        using TypePtr = UCodeEditor::TileData;
        static Node encode(const TypePtr& rhs) 
		{

			UCode::USerializer serializer(UCode::USerializerType::YAML);
			rhs.PushData(serializer);

			auto& maker = serializer.Get_TextMaker();
			UCode::String r = maker.c_str();

			
			return YAML::Load(r);
        }

		static bool decode(const Node& node, TypePtr& rhs)
		{
			UCode::UDeserializer serializer("");
			serializer.Get_TextReader() = node;

			return TypePtr::FromString(rhs, serializer);
		}
    };


}
CoreStart

template<>
struct BitData<UCodeEditor::TileData>
{
	using TypePtr = UCodeEditor::TileData;
	static void ToBytes(BitMaker& This, const TypePtr& Value)
	{
		UCode::USerializer serializer(UCode::USerializerType::Bytes);
		Value.PushData(serializer);

		Vector<UCode::Byte> bytes;
		serializer.ToBytes(bytes, false);

		This.WriteBytes(bytes.data(), bytes.size());
	}
	
	static void FromBytes(BitReader& This, TypePtr& Out)
	{
		UCode::UDeserializer serializer(BytesView::Make((Byte*)This.Get_OffsetDataPointer(),This.GetBytesSize()));

		TypePtr::FromString(Out, serializer);
	
		This.Addoffset(serializer.Get_BitReader().GetBytesOffset());
	}
};

CoreEnd
EditorStart
void TilePalette::PushData(USerializer& node) const
{
	node.Write("_UID", _UID);
}
bool TilePalette::FromString(TilePalette& out, UDeserializer& text)
{
	text.ReadType("_UID", out._UID);
	return true;
}
bool TilePalette::FromFile(TilePalette& out, const Path& Path)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V);
	if (A)
	{
		return FromString(out, V);
	}
	return A;
}
bool TilePalette::ToFile(const Path& path, const TilePalette& data, USerializerType Type)
{
	USerializer V(Type);
	data.PushData(V);
	return V.ToFile(path);
}

void TileData::PushData(USerializer& node) const
{
	node.Write("_UID", _UID);
	_Data.PushData(node);
}
bool TileData::FromString(TileData& out, UDeserializer& text)
{
	text.ReadType("_UID", out._UID);
	return UC::TileData::FromString(out._Data, text);
}
bool TileData::FromFile(TileData& out, const Path& Path)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V);
	if (A)
	{
		return FromString(out, V);
	}
	return A;
}
bool TileData::ToFile(const Path& path, const TileData& data, USerializerType Type)
{
	USerializer V(Type);
	data.PushData(V);
	return V.ToFile(path);
}


void TileDataPack::PushData(USerializer& node) const
{
	node.Write("Tiles", List);
}

bool TileDataPack::FromString(TileDataPack& out, UDeserializer& text)
{
	text.ReadType("Tiles", out.List, out.List);
	return true;
}

bool TileDataPack::FromFile(TileDataPack& out, const Path& Path)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V);
	if (A)
	{
		return FromString(out, V);
	}
	return A;
}

bool TileDataPack::ToFile(const Path& path, const TileDataPack& data, USerializerType Type)
{
	USerializer V(Type);
	data.PushData(V);
	return V.ToFile(path);
}
EditorEnd

