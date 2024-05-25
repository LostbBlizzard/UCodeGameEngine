#include "TilePaletteData.hpp"

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

