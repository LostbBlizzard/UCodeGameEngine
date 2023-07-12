#pragma once
#include "BitConverter.hpp"
#include "UCodeRunTime/RunTimeBasicTypes.hpp"
CoreStart

//Not By default this will be useing little  Endian

template<typename T>
struct BitData
{

}; 

class BitMaker
{
public:
	using SizeAsBits = UInt32;
	BitMaker() :_Bytes(), Converter(&littleEndianConverter)
	{
	}
	BitMaker(size_t BitSize) :_Bytes(BitSize), Converter(&littleEndianConverter)
	{

	}
	BitMaker(BitConverter* converter,size_t BitSize = 0) :_Bytes(BitSize), Converter(converter)
	{

	}
	~BitMaker()
	{

	}

	UCODE_ENGINE_FORCE_INLINE void Resize(size_t NewSize){ _Bytes.resize(NewSize);}
	UCODE_ENGINE_FORCE_INLINE void Clear(){ _Bytes.clear();}
	UCODE_ENGINE_FORCE_INLINE auto& Get_Bytes() { return _Bytes; }
	UCODE_ENGINE_FORCE_INLINE size_t Size() {return _Bytes.size(); }

	void WriteBytes(const Byte* Bits, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			_Bytes.push_back(Bits[i]);
		}
	}

	template<typename T>
	UCODE_ENGINE_FORCE_INLINE void WriteTypeAsBytes(const T& Value){WriteBytes((const Byte*)&Value, sizeof(Value));}


	template<typename T>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const T& Value){ BitData<T>::ToBytes(*this, Value);}


	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const bool& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Byte& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const SInt8& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const char& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const UInt16& Value) { WriteTypeAsBytes(Converter->GetBytes(Value));}

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const SInt16& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }
	
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const UInt32& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }
	
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const SInt32& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }
	
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const UInt64& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const SInt64& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }
	
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const float32& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const float64& Value) { WriteTypeAsBytes(Converter->GetBytes(Value)); }


	//
	
	UCODE_ENGINE_FORCE_INLINE void WriteType(const char* Value)
	{ 
		size_t size = strlen(Value);
		WriteType((SizeAsBits)size);
		WriteBytes((const Byte*)Value, size);
	}

	template<typename T>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Span<T> Value)
	{
		size_t size = Value.Size();
		WriteType((SizeAsBits)size);
		for (size_t i = 0; i < size; i++)
		{
			WriteType(Value[i]);
		}
	}

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const String& Value) 
	{
		size_t size = Value.size();
		WriteType((SizeAsBits)size);
		WriteBytes((const Byte*)Value.c_str(), size);
	}

	template<typename T>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Vector<T>& Value) 
	{  
		WriteType(Span<T>::Make(Value.data(), Value.size()));
	}

	template<typename T>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Ref<T>& Value)
	{
		const auto& Obj = Value.get();
		WriteType(Obj);
	}
	//
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Vec2& Value)
	{
		WriteType(Value.X);
		WriteType(Value.Y);
	}
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Vec2i& Value)
	{
		WriteType(Value.X);
		WriteType(Value.Y);
	}
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Vec3& Value)
	{
		WriteType(Value.X);
		WriteType(Value.Y);
		WriteType(Value.Z);
	}
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Vec3i& Value)
	{
		WriteType(Value.X);
		WriteType(Value.Y);
		WriteType(Value.Z);
	}

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Color& Value)
	{
		WriteType(Value.R);
		WriteType(Value.G);
		WriteType(Value.B);
		WriteType(Value.A);
	}
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Color32& Value)
	{
		WriteType(Value.R);
		WriteType(Value.G);
		WriteType(Value.B);
		WriteType(Value.A);
	}

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Color24& Value)
	{
		WriteType(Value.R);
		WriteType(Value.G);
		WriteType(Value.B);
	}
	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const ColorRGB& Value)
	{
		WriteType(Value.R);
		WriteType(Value.G);
		WriteType(Value.B);
	}

	template<>
	UCODE_ENGINE_FORCE_INLINE void WriteType(const Version& Value)
	{
		WriteType(Value.Major);
		WriteType(Value.Minor);
		WriteType(Value.Revision);
	}
	

	template<> UCODE_ENGINE_FORCE_INLINE void WriteType(const Path& Value)
	{
		WriteType(Value.generic_string());

	}

	template<> UCODE_ENGINE_FORCE_INLINE void WriteType(const UID& Value)
	{
		WriteType(Value.Get_Value());

	}
private:
	Vector<Byte> _Bytes;
	BitConverter* Converter =nullptr;
};
class BitReader
{
public:
	using SizeAsBits = BitMaker::SizeAsBits;
	BitReader(): _Converter(&littleEndianConverter)
	{

	}
	BitReader(const Byte* Bytes, size_t Size): _Converter(&littleEndianConverter)
	{
		SetBytes(Bytes, Size);
	}
	~BitReader()
	{

	}

	UCODE_ENGINE_FORCE_INLINE void SetBytes(const BytesView Bytes)
	{
		SetBytes(Bytes.Data(), Bytes.Size());
	}
	UCODE_ENGINE_FORCE_INLINE void SetBytes(const Byte* Bytes,size_t Size)
	{
		_Bytes = Bytes;
		_BytesSize = Size;
		Resetoffset();
	}
	UCODE_ENGINE_FORCE_INLINE void Resetoffset()
	{
		_BitOffset = 0;
	}


	const void* Get_OffsetDataPointer()
	{
		return &_Bytes[_BitOffset];
	} 
	void Addoffset(size_t Offset)
	{
		_BitOffset += Offset;
	}
	void ReadBytes(void* Out, const void* fallback,size_t BytesSize)
	{
		try
		{
			ReadBytes(Out,BytesSize);
		}
		catch (const std::exception&)
		{
			memcpy(Out, fallback, BytesSize);
		}
	}
	void ReadBytes(void* Out, size_t BytesSize)
	{
		SizeCheck(BytesSize);
		memcpy(Out, _Bytes, BytesSize);
		_BitOffset += BytesSize;
	}

	template<typename T>
	void ReadType(T& Out)
	{
		BitData<T>::FromBytes(*this,Out);
	}

	template<typename T> void ReadType(T& Out,const T& fallback)
	{
		try
		{
			ReadType(Out);
		}
		catch (const std::exception&)
		{
			Out = fallback;
		}
	}	
	
	UCODE_ENGINE_FORCE_INLINE void SizeCheck(size_t Size)
	{
		if (_BitOffset + Size > _BytesSize)
		{
			throw std::exception("No More Values");
		}
	}
	//
	template<> void ReadType(Byte& Out)
	{
		SizeCheck(sizeof(Byte));
		Out = _Converter->BytesToUChar(_Bytes, _BitOffset);
		_BitOffset += sizeof(Byte);
	}
	template<> void ReadType(SInt8& Out)
	{
		SizeCheck(sizeof(SInt8));
		Out = _Converter->BytesToSChar(_Bytes, _BitOffset);
		_BitOffset += sizeof(SInt8);
	}
	template<> void ReadType(char& Out)
	{
		SizeCheck(sizeof(char));
		_Converter->BytesToChar(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(char);
	}
	template<> void ReadType(bool& Out)
	{
		SizeCheck(sizeof(bool));
		_Converter->BytesToBool(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(bool);
	}


	template<> void ReadType(UInt16& Out)
	{
		SizeCheck(sizeof(UInt16));
		_Converter->BytesToInt16(_Bytes, _BitOffset,&Out);
		_BitOffset += sizeof(UInt16);
	}
	template<> void ReadType(SInt16& Out)
	{
		SizeCheck(sizeof(SInt16));
		_Converter->BytesToInt16(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(SInt16);
	}

	template<> void ReadType(UInt32& Out)
	{
		SizeCheck(sizeof(UInt32));
		_Converter->BytesToInt(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(UInt32);
	}
	template<> void ReadType(SInt32& Out)
	{
		SizeCheck(sizeof(SInt32));
		_Converter->BytesToInt(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(SInt32);
	}

	template<> void ReadType(UInt64& Out)
	{
		SizeCheck(sizeof(UInt64));
		_Converter->BytesToInt64(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(UInt64);
	}
	template<> void ReadType(SInt64& Out)
	{
		SizeCheck(sizeof(SInt64));
		_Converter->BytesToInt64(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(SInt64);
	}

	
	template<> void ReadType(float32& Out)
	{
		SizeCheck(sizeof(float32));
		_Converter->BytesTofloat(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(float32);
	}

	template<> void ReadType(float64& Out)
	{
		SizeCheck(sizeof(float64));
		_Converter->BytesTofloat64(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(float64);
	}
	
	template<> void ReadType(String& Out)
	{
		SizeAsBits StrSize;
		ReadType<SizeAsBits>(StrSize, 0);
		size_t Size = StrSize;

		Out.resize(Size);
		for (size_t i = 0; i < Size; i++)
		{
			ReadType(Out[i]);
		}
		
	}

	template<typename T> Unique_Span<T> ReadSpan()
	{
		SizeAsBits StrSize;
		ReadType<SizeAsBits>(StrSize, 0);
		size_t Size = StrSize;

		Unique_Span<T> r;
		r.Size = Size;
		r.Pointer = std::make_unique<T[]>(Size);

		for (size_t i = 0; i < Size; i++)
		{
			ReadType(r[i]);
		}

		return r;

	}

	template<typename T> void ReadType(Vector<T>& Out)
	{
		SizeAsBits StrSize;
		ReadType<SizeAsBits>(StrSize, 0);
		size_t Size = StrSize;

		Out.resize(StrSize);
		for (size_t i = 0; i < Size; i++)
		{
			ReadType(Out[i]);
		}
	}

	template<typename T> void ReadType(Ref<T>& Out)
	{
		ReadType(Out.get(), Out.get());
	}

	template<> void ReadType(Vec2& Out)
	{
		ReadType(Out.X, Out.X);
		ReadType(Out.Y, Out.Y);
	}
	template<> void ReadType(Vec2i& Out)
	{
		ReadType(Out.X, Out.X);
		ReadType(Out.Y, Out.Y);
	}
	template<> void ReadType(Vec3& Out)
	{
		ReadType(Out.X, Out.X);
		ReadType(Out.Y, Out.Y);
		ReadType(Out.Z, Out.Z);
	}
	template<> void ReadType(Vec3i& Out)
	{
		ReadType(Out.X, Out.X);
		ReadType(Out.Y, Out.Y);
		ReadType(Out.Z, Out.Z);
	}
	template<> void ReadType(Color& Out)
	{
		ReadType(Out.R, Out.R);
		ReadType(Out.G, Out.G);
		ReadType(Out.B, Out.B);
		ReadType(Out.A, Out.A);
	}
	template<> void ReadType(Color32& Out)
	{
		ReadType(Out.R, Out.R);
		ReadType(Out.G, Out.G);
		ReadType(Out.B, Out.B);
		ReadType(Out.A, Out.A);
	}

	template<> void ReadType(ColorRGB& Out)
	{
		ReadType(Out.R, Out.R);
		ReadType(Out.G, Out.G);
		ReadType(Out.B, Out.B);
	}
	template<> void ReadType(Color24& Out)
	{
		ReadType(Out.R, Out.R);
		ReadType(Out.G, Out.G);
		ReadType(Out.B, Out.B);
	}
	template<> void ReadType(Path& Out)
	{
		String V;
		ReadType(V);
		Out = V;
	}
	template<> UCODE_ENGINE_FORCE_INLINE void ReadType(UID& Out)
	{
		UID::UID_t b = 0;
		ReadType(b,b);
		Out.Set_Value(b);
	}

	template<> UCODE_ENGINE_FORCE_INLINE void ReadType(Version& Out)
	{
		ReadType(Out.Major, Out.Major);
		ReadType(Out.Minor, Out.Minor);
		ReadType(Out.Revision, Out.Revision);
	}
	
private:
	const Byte* _Bytes = nullptr;
	size_t _BytesSize = 0;
	size_t _BitOffset = 0;
	BitConverter* _Converter =nullptr;
};
CoreEnd

