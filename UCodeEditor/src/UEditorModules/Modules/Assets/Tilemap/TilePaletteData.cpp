#include "TilePaletteData.hpp"
EditorStart
void TilePalette::PushData(USerializer& node) const
{
	node.Write("_UID", _UID);
	node.Write("_List", _List);
}
bool TilePalette::FromString(TilePalette& out, UDeserializer& text)
{
	text.ReadType("_UID", out._UID);
	text.ReadType("_List", out._List);
	return true;
}
bool TilePalette::FromFile(TilePalette& out, const Path& Path, USerializerType Type)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V,Type);
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
	return V.ToFile(path,false);
}

void TileData::PushData(USerializer& node) const
{
	node.Write("_UID", _UID);
	this->_Base.PushData(node);
}
bool TileData::FromString(TileData& out, UDeserializer& text)
{
	text.ReadType("_UID", out._UID);
	out.Uid = out.Uid;
	return UC::TileData::FromString(out._Base, text);
}
bool TileData::FromFile(TileData& out, const Path& Path,USerializerType Type)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V,Type);
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
	return V.ToFile(path,false);
}


void TileDataPack::PushData(USerializer& node) const
{
	node.Write("_UID", _UID);
	node.Write("TextureAtlas", _BaseTexture);
	node.Write("Tiles", List);
}

bool TileDataPack::FromString(TileDataPack& out, UDeserializer& text)
{
	text.ReadType("_UID", out._UID, out._UID);
	text.ReadType("TextureAtlas", out._BaseTexture,out._BaseTexture);
	text.ReadType("Tiles", out.List, out.List);
	return true;
}

bool TileDataPack::FromFile(TileDataPack& out, const Path& Path, USerializerType Type)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V,Type);
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
	return V.ToFile(path,false);
}
EditorEnd

