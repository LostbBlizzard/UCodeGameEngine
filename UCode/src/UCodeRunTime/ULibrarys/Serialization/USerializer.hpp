#pragma once
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>

#include "BitMaker.hpp"
#include "Yaml_Implementation/BasicTypes.hpp"
#include "Yaml_Implementation/Yaml_NotIMPlEMENTED.hpp"
#include "Bit_Implementation/BitData_NotIMPlEMENTED.hpp"
CoreStart


#define UCode_YAML_UCharFix(x)  ( (unsigned int) x)
#define UCode_YAML_CharFix(x)  ( (signed int) x)

using USerializerType_t = u8;
enum class USerializerType : USerializerType_t
{
	Bytes,
	YAML,
	BytesSafeMode = YAML,

	Default = YAML,
	Fastest = Bytes,
	Readable = YAML,
};
class USerializer
{
public:
	USerializer(USerializerType Type = USerializerType::Default);
	~USerializer();
	
	void StartObject(){}
	void EndObject(){}

	template<typename T>
	void Write(const char* FieldName,const T& Value)
	{
		switch (_Type)
		{
		case UCode::USerializerType::Bytes:
			_BitMaker.WriteType(Value);
			break;
		case UCode::USerializerType::YAML:
			_TextMaker << YAML::Key << FieldName << YAML::Value << Value;
			break;
		default:
			break;
		}
	}

	
	void Write(const char* FieldName, const unsigned char& Value)
	{
		switch (_Type)
		{
		case UCode::USerializerType::Bytes:
			_BitMaker.WriteType(Value);
			break;
		case UCode::USerializerType::YAML:
			_TextMaker << YAML::Key << FieldName << YAML::Value << UCode_YAML_UCharFix(Value);
			break;
		default:
			break;
		}
	}

	
	void Write(const char* FieldName, const signed char& Value)
	{
		switch (_Type)
		{
		case UCode::USerializerType::Bytes:
			_BitMaker.WriteType(Value);
			break;
		case UCode::USerializerType::YAML:
			_TextMaker << YAML::Key << FieldName << YAML::Value << UCode_YAML_CharFix(Value);
			break;
		default:
			break;
		}
	}


	void Reset();


	UCodeGEForceinlne USerializerType Get_Type(){return _Type;}
	UCodeGameEngineNoDiscard bool ToFile(const Path& Path, bool WithSerializerType = true);
	void ToString(String& Out, bool WithSerializerType = false);
	void ToBytes(Vector<Byte>& Out,bool WithSerializerType =false);
	void ToStream(std::ostream& Out, bool WithSerializerType = true);

	
	BitMaker& Get_BitMaker()
	{
		if (_Type != USerializerType::Bytes)
		{
			UCodeGEThrow("Bad Cast");
		}
		return _BitMaker;
	}
	YAML::Emitter& Get_TextMaker()
	{
		if (_Type != USerializerType::YAML)
		{
			UCodeGEThrow("Bad Cast");
		}
		return  _TextMaker;
	}
private:
	USerializerType _Type = USerializerType::Default;

	//Bytes
	BitMaker _BitMaker;
	//Yaml
	YAML::Emitter _TextMaker;
	void DoneWriteing();
};
class UDeserializer
{
public:
	UDeserializer();
	UDeserializer(const StringView string);
	UDeserializer(const BytesView Bytes);
	~UDeserializer();

	void StartObject() {}
	void EndObject() {}

	void SetBytes(const BytesView Bytes);
	void SetYAMLString(const StringView string);

	void SetData(const BytesView Bytes);//the byte[0] is SerializerType 
	void SetData(const BytesView Bytes,USerializerType type);

	UCodeGameEngineNoDiscard static bool FromFile(const Path& Path, UDeserializer& Out);//Note the File must be made from USerializer::ToFile
	//

	template<typename T>
	void ReadType(const char* FieldName,T& Out)
	{
		switch (_Type)
		{
		case UCode::USerializerType::Bytes:
			_BitReader.ReadType(Out);
			break;
		case UCode::USerializerType::YAML:
			Out = _YamlReader[FieldName].as<T>(Out);
			break;
		default:
			break;
		}
		
	}

	template<typename T> void ReadType(const char* FieldName, T& Out, const T& fallback)
	{
		switch (_Type)
		{
		case UCode::USerializerType::Bytes:
			_BitReader.ReadType(Out,fallback);
			break;
		case UCode::USerializerType::YAML:
			Out = _YamlReader[FieldName].as<T>(fallback);
			break;
		default:
			break;
		}
	}
	bool HasField(const char* FieldName) const
	{
		if (_Type == USerializerType::YAML)
		{
			return (bool)_YamlReader[FieldName];
		}
		return true;
	}

	void SetBytesPtr(Unique_Bytes&& Bits)
	{
		_Bytes = std::move(Bits);
	}
	USerializerType Get_Mode() const
	{
		return  _Type;
	}
	BitReader& Get_BitReader()
	{
		if (_Type != USerializerType::Bytes)
		{
			UCodeGEThrow("Bad Cast");
		}
		return  _BitReader;
	}
	YAML::Node& Get_TextReader()
	{
		if (_Type != USerializerType::YAML)
		{
			UCodeGEThrow("Bad Cast");
		}
		return _YamlReader;
	}
private:
	USerializerType _Type = USerializerType::Default;
	//Bytes
	BitReader _BitReader;
	//Yaml
	YAML::Node _YamlReader;
	//
	Unique_Bytes _Bytes;
};

inline String ToString(YAML::Node& node)
{
    String r;
    for (YAML::iterator it = node.begin(); it != node.end(); ++it)
    {
        if (r.size())
        {
            r += "\n";
        }

        r += it->first.as<std::string>();
        r += ": ";
        r += it->second.as<std::string>();

       
    }
      
    return r;
}
CoreEnd
