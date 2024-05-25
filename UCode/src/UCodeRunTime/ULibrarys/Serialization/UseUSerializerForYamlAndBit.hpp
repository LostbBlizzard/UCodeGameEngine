#pragma once

#define UseUSerializerForYamlAndBit(TypeFullName) \
namespace YAML \
{ \
	template<>  \
    struct convert< TypeFullName > \
    { \
        using TypePtr = UCodeEditor::TileData;  \
		inline static Node encode(const TypePtr& rhs) \
		{ \
			UCode::USerializer serializer(UCode::USerializerType::YAML); \
			rhs.PushData(serializer); \
			auto& maker = serializer.Get_TextMaker(); \
			UCode::String r = maker.c_str(); \
			return YAML::Load(r); \
        } \
		inline static bool decode(const Node& node, TypePtr& rhs) \
		{ \
			UCode::UDeserializer serializer(""); \
			serializer.Get_TextReader() = node; \
			return TypePtr::FromString(rhs, serializer); \
		} \
    }; \
} \
CoreStart \
template<> \
struct BitData< TypeFullName > \
{ \
	using TypePtr = TypeFullName; \
	inline static void ToBytes(BitMaker& This, const TypePtr& Value) \
	{ \
		UCode::USerializer serializer(UCode::USerializerType::Bytes); \
		Value.PushData(serializer); \
		Vector<UCode::Byte> bytes; \
		serializer.ToBytes(bytes, false); \
		This.WriteBytes(bytes.data(), bytes.size()); \
	} \
	inline static void FromBytes(BitReader& This, TypePtr& Out) \
	{ \
		UCode::UDeserializer serializer(BytesView::Make((Byte*)This.Get_OffsetDataPointer(),This.GetBytesSize())); \
		TypePtr::FromString(Out, serializer); \
		This.Addoffset(serializer.Get_BitReader().GetBytesOffset()); \
	} \
}; \
CoreEnd \
