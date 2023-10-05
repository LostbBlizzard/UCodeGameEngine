#include "USerializer.hpp"

CoreStart

USerializer::USerializer(USerializerType Type) :_Type(Type)
{
	_TextMaker << YAML::BeginMap;
}
USerializer::~USerializer()
{
}

void USerializer::Reset()
{
	switch (_Type)
	{
	case UCode::USerializerType::Bytes:
		_BitMaker.Clear();
		break;
	case UCode::USerializerType::YAML: 
	{
		_TextMaker.~Emitter();
		new (&_TextMaker) YAML::Emitter();
		_TextMaker << YAML::BeginMap;
	}break;
	default:
		UCodeGameEngineUnreachble;
		break;
	}

}

bool USerializer::ToFile(const Path& Path, bool WithSerializerType)
{
	std::ofstream myfile(Path, std::ios::out | std::ios::binary);
	if (myfile.is_open())
	{
		ToStream(myfile, WithSerializerType);
		myfile.close();
		return true;
	}
	return false;
}
void USerializer::ToString(String& Out, bool WithSerializerType)
{
	DoneWriteing();

	if (WithSerializerType)
	{
		Out.push_back(*(char*)&_Type);
	}

	switch (_Type)
	{
	case UCode::USerializerType::Bytes:
	{
		auto& Bits = _BitMaker.Get_Bytes();
		for (size_t i = 0; i < Bits.size(); i++)
		{
			Out.push_back(Bits[i]);
		}
	}	break;
	case UCode::USerializerType::YAML:
	{
		if ((String)_TextMaker.c_str() != "{}") {
			Out += _TextMaker.c_str();
		}
	}
	break;
	default:
		UCodeGameEngineUnreachble();
		break;
	}

}

UCodeGameEngineNoDiscard void USerializer::ToBytes(Vector<Byte>& Out, bool WithSerializerType)
{
	String buff;
	ToString(buff, WithSerializerType);


	Out.resize(buff.size());
	memcpy(Out.data(), buff.data(), buff.size());
}

void USerializer::ToStream(std::ostream& Out, bool WithSerializerType)
{
	DoneWriteing();

	if (WithSerializerType)
	{
		Out << *(USerializerType_t*)&_Type;
	}

	switch (_Type)
	{
	case UCode::USerializerType::Bytes:
	{
		auto& Bits = _BitMaker.Get_Bytes();
		for (size_t i = 0; i < Bits.size(); i++)
		{
			Out << Bits[i];
		}
	}	
	break;
	case UCode::USerializerType::YAML:
	{
		if ((String)_TextMaker.c_str() != "{}") {
			Out << _TextMaker.c_str();
		}
	}
	break;
	default:
		UCodeGameEngineUnreachble;
		break;
	}
}

void USerializer::DoneWriteing()
{
	_TextMaker << YAML::EndMap;
}



UDeserializer::UDeserializer()
{
}

UDeserializer::UDeserializer(const StringView string)
{
	SetYAMLString(string);
}

UDeserializer::UDeserializer(const BytesView Bytes)
{
	SetBytes(Bytes);
}

UDeserializer::~UDeserializer()
{

}

void UDeserializer::SetBytes(const BytesView Bytes)
{
	_Type = USerializerType::Bytes;
	_BitReader.SetBytes(Bytes);
	
}

void UDeserializer::SetYAMLString(const StringView string)
{
	_Type = USerializerType::YAML;
	_YamlReader = YAML::Load((String)string);
}

void UDeserializer::SetData(const BytesView Bytes)
{
	if (Bytes.Size())
	{
		union 
		{
			Byte Type;
			USerializerType SerializerType;
		};
		Type = Bytes[0];
	
		const BytesView Bytes2 = BytesView::Make(Bytes.Data() + 1,Bytes.Size() -1);

		switch (SerializerType)
		{
		case UCode::USerializerType::Bytes:
			SetBytes(Bytes2);
			break;
		case UCode::USerializerType::YAML:
			SetYAMLString(StringView((char*)Bytes2.Data(), Bytes2.Size()));
			break;
		default:
			break;
		}

	}
}

UCodeGameEngineNoDiscard bool UDeserializer::FromFile(const Path& Path, UDeserializer& Out)
{
	String Text;
	String line;
	std::ifstream File(Path, std::ios::binary);
	if (File.is_open())
	{
		Unique_Bytes Bits;
		File.seekg(0, File.end);
		Bits.Resize(File.tellg());
		File.seekg(0, File.beg);
		File.read((char*)Bits.Data(), Bits.Size());
		

		Out.SetData(Bits.AsView());
		Out.SetBytesPtr(std::move(Bits));

		return true;
	}
	return false;
}
CoreEnd


