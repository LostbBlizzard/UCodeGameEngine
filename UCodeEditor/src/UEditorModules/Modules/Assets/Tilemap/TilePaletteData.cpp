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

EditorEnd