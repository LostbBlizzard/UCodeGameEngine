#include "RttrSerializer.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp"
#include "../Serialization/Yaml_Implementation/AsssetPtr.hpp"
#include "../Serialization/Bit_Implementation/AsssetPtr.hpp"

CoreStart

void RttrSerializer::Write(USerializer& serializer,const char* FieldName, rttr::variant& value)
{
	if (value.get_type() == rttr::type::get<bool>()){serializer.Write(FieldName, value.get_value<bool>());}
	else if (value.get_type() == rttr::type::get<i8>()){serializer.Write(FieldName, value.get_value<i8>());}
	else if (value.get_type() == rttr::type::get<i16>()){serializer.Write(FieldName, value.get_value<i16>());}
	else if (value.get_type() == rttr::type::get<i32>()){serializer.Write(FieldName, value.get_value<i32>());}
	else if (value.get_type() == rttr::type::get<i64>()){serializer.Write(FieldName, value.get_value<i64>());}
	else if (value.get_type() == rttr::type::get<u8>()){serializer.Write(FieldName, value.get_value<u8>());}
	else if (value.get_type() == rttr::type::get<u16>()){serializer.Write(FieldName, value.get_value<u16>());}
	else if (value.get_type() == rttr::type::get<u32>()){serializer.Write(FieldName, value.get_value<u32>());}
	else if (value.get_type() == rttr::type::get<u64>()){serializer.Write(FieldName, value.get_value<u64>());}
	else if (value.get_type() == rttr::type::get<f32>()){serializer.Write(FieldName, value.get_value<f32>());}
	else if (value.get_type() == rttr::type::get<f64>()){serializer.Write(FieldName, value.get_value<f64>());}

	//UCode Types

	else if (value.get_type() == rttr::type::get<Color>()){serializer.Write(FieldName, value.get_value<Color>());}
	else if (value.get_type() == rttr::type::get<ColorRGB>()){serializer.Write(FieldName, value.get_value<Color24>());}
	else if (value.get_type() == rttr::type::get<Color32>()){serializer.Write(FieldName, value.get_value<Color32>());}
	else if (value.get_type() == rttr::type::get<Color24>()){serializer.Write(FieldName, value.get_value<Color24>());}
	
	else if (value.get_type() == rttr::type::get<Vec2>()){serializer.Write(FieldName, value.get_value<Vec2>());}
	else if (value.get_type() == rttr::type::get<Vec2i>()){serializer.Write(FieldName, value.get_value<Vec2i>());}
	else if (value.get_type() == rttr::type::get<Vec3>()){serializer.Write(FieldName, value.get_value<Vec3>());}
	else if (value.get_type() == rttr::type::get<Vec3i>()){serializer.Write(FieldName, value.get_value<Vec3i>());}

	//AssetTypes
	else if (value.get_type() == rttr::type::get<UID>()){serializer.Write(FieldName, value.get_value<UID>());}
	else if (value.get_type() == rttr::type::get<SpritePtr>()){serializer.Write(FieldName, value.get_value<SpritePtr>());}
	else
	{
		UCodeGEUnreachable();
	}
}
void RttrSerializer::Read(UDeserializer& serializer, const char* FieldName, rttr::instance& value, const rttr::property& pro)
{
	//C++ Base Types
	if (pro.get_type() == rttr::type::get<bool>()) { auto obj = bool(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<i8>()) { auto obj = i8(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<i16>()) { auto obj = i16(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<i32>()) { auto obj = i32(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<i64>()) { auto obj = i64(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<u8>()) { auto obj = u8(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<u16>()) { auto obj = u16(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<u32>()) { auto obj = u32(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<u64>()) { auto obj = u64(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<f32>()) { auto obj = f32(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<f64>()) { auto obj = f64(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	//C++ Stl Types

	//UCode Types
	if (pro.get_type() == rttr::type::get<Vec2>()) { auto obj = Vec2(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<Vec2i>()) { auto obj = Vec2i(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<Vec3>()) { auto obj = Vec3(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<Vec3i>()) { auto obj = Vec3i(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<Color>()) { auto obj = Color(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<ColorRGB>()) { auto obj = ColorRGB(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<Color24>()) { auto obj = Color24(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<Color32>()) { auto obj = Color32(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	//AssetTypes 
	if (pro.get_type() == rttr::type::get<UID>()) { auto obj = UID(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }
	if (pro.get_type() == rttr::type::get<SpritePtr>()) { auto obj = SpritePtr(); serializer.ReadType(FieldName, obj, obj); pro.set_value(value, obj); }


}
bool RttrSerializer::IsSame(rttr::type T, rttr::variant& A, rttr::variant& B)
{
	//C++ Base Types
	if (T == rttr::type::get<bool>()) { return A.get_value<bool>() == B.get_value<bool>(); }
	if (T == rttr::type::get<i8>()) { return A.get_value<i8>() == B.get_value<i8>(); }
	if (T == rttr::type::get<i16>()) { return A.get_value<i16>() == B.get_value<i16>(); }
	if (T == rttr::type::get<i32>()) { return A.get_value<i32>() == B.get_value<i32>(); }
	if (T == rttr::type::get<i64>()) { return A.get_value<i64>() == B.get_value<i64>(); }

	if (T == rttr::type::get<u8>()) { return A.get_value<u8>() == B.get_value<u8>(); }
	if (T == rttr::type::get<u16>()) { return A.get_value<u16>() == B.get_value<u16>(); }
	if (T == rttr::type::get<u32>()) { return A.get_value<u32>() == B.get_value<u32>(); }
	if (T == rttr::type::get<u64>()) { return A.get_value<u64>() == B.get_value<u64>(); }

	//C++ Stl Types

	//UCode Types
	if (T == rttr::type::get<Vec2>()) { return A.get_value<Vec2>() == B.get_value<Vec2>(); }
	if (T == rttr::type::get<Vec2i>()) { return A.get_value<Vec2i>() == B.get_value<Vec2i>(); }
	if (T == rttr::type::get<Vec3>()) { return A.get_value<Vec3>() == B.get_value<Vec3>(); }
	if (T == rttr::type::get<Vec3i>()) { return A.get_value<Vec3i>() == B.get_value<Vec3i>(); }
	if (T == rttr::type::get<Color>()) { return A.get_value<Color>() == B.get_value<Color>(); }
	if (T == rttr::type::get<ColorRGB>()) { return A.get_value<ColorRGB>() == B.get_value<ColorRGB>(); }
	if (T == rttr::type::get<Color24>()) { return A.get_value<Color24>() == B.get_value<Color24>(); }
	if (T == rttr::type::get<Color32>()) { return A.get_value<Color32>() == B.get_value<Color32>(); }
	//AssetTypes 

	if (T == rttr::type::get<UID>()) { return A.get_value<UID>() == B.get_value<UID>(); }
	if (T == rttr::type::get<SpritePtr>()) { return A.get_value<SpritePtr>() == B.get_value<SpritePtr>(); }

	UCodeGEUnreachable();
}
CoreEnd


