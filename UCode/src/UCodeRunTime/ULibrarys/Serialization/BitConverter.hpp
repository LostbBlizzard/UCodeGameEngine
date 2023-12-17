#pragma once

#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "UCodeRunTime/UDefs.hpp"
#include <UCodeRunTime/BasicTypes.hpp>
CoreStart



class BitConverter
{
public:
	enum class Endian : Byte
	{
		NaN,
		little,
		Big,
	};
	BitConverter(Endian Endian) :InputOutEndian(Endian)
	{

	}
	Endian InputOutEndian = Endian::little;
	inline static Endian _CPUEndian = BitConverter::Endian::NaN;
	UCodeGEForceinlne static Endian Get_CPU_Endian()
	{
		if (_CPUEndian == Endian::NaN)
		{
			_CPUEndian = _GetEndian();
		}

		return _CPUEndian;
	}
	UCodeGEForceinlne static Endian _GetEndian()
	{
		union
		{
			i32 NumValue;
			Byte _Bytes[4];
		};
		NumValue = 1;

		if (_Bytes[0] == 1)
		{
			return Endian::little;
		}
		else
		{
			return Endian::Big;
		}
		return Endian::NaN;
	}
	
	
	
	
	
	//
	
	struct Byte16
	{
		Byte A, B;
		inline void MoveValues(Byte* P, size_t Indexofset)
		{
			Byte* outv = &(P)[Indexofset];
			outv[0] = A;
			outv[1] = B;
		}
		inline Byte16 FlpBytes()
		{
			return { B,A };
		}
	};
	struct Byte32
	{
		Byte A, B, C, D;
		inline void MoveValues(Byte* P, size_t Indexofset)
		{
			Byte* outv = &(P)[Indexofset];
			outv[0] = A;
			outv[1] = B;
			outv[2] = C;
			outv[3] = D;
		}
		inline Byte32 FlpBytes()
		{
			return { D,C,B,A };
		}
	};
	struct Byte64
	{
		Byte A, B, C, D,
			E, F, G, H;
		inline void MoveValues(Byte* P, size_t Indexofset)
		{
			Byte* outv = &(P)[Indexofset];
			outv[0] = A;
			outv[1] = B;
			outv[2] = C;
			outv[3] = D;

			outv[4] = E;
			outv[5] = F;
			outv[6] = G;
			outv[7] = H;
		}
		inline Byte64 FlpBytes()
		{
			return
			{
				H,G,F,E,
				D,C,B,A
			};
		}
	};



	UCodeGEForceinlne Byte GetBytes(bool Value)
	{
		return (Byte)Value;
	}
	UCodeGEForceinlne bool BytesToBool(const void* DataBytes, size_t Index)
	{
		Byte B = ((Byte*)DataBytes)[Index];
		return B;
	}
	UCodeGEForceinlne Byte GetBytes(signed char Value)
	{
		return GetBytes((unsigned char)Value);
	}
	UCodeGEForceinlne signed char BytesToSChar(const void* DataBytes, size_t Index)
	{
		return BytesToUChar(DataBytes, Index);
	}

	UCodeGEForceinlne Byte GetBytes(char Value)
	{
		return GetBytes((unsigned char)Value);
	}
	UCodeGEForceinlne char BytesToChar(const void* DataBytes, size_t Index)
	{
		return BytesToUChar(DataBytes, Index);
	}

	UCodeGEForceinlne Byte GetBytes(unsigned char Value)
	{
		return Value;
	}
	UCodeGEForceinlne unsigned char BytesToUChar(const void* DataBytes, size_t Index)
	{
		Byte B = ((const Byte*)DataBytes)[Index];
		return B;
	}

	Byte16 GetBytes(i16 Value);
	i16 BytesToInt16(const void* DataBytes, size_t Index);

	UCodeGEForceinlne Byte16 GetBytes(u16 Value) { return GetBytes(*(i16*)&Value); }
	UCodeGEForceinlne u16 BytesToUInt16(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt16(DataBytes, Index);
		return *(u16*)&V;
	}

	Byte32 GetBytes(i32 Value);
	i32 BytesToInt(const void* DataBytes, size_t Index);

	UCodeGEForceinlne Byte32 GetBytes(u32 Value) { return GetBytes(*(int*)&Value); }
	UCodeGEForceinlne u32 BytesToUInt(const void* DataBytes, size_t Index) {
		auto V = BytesToInt(DataBytes, Index);
		return *(unsigned int*)&V;
	}

	UCodeGEForceinlne Byte32 GetBytes(f32 Value) { return GetBytes(*(int*)&Value); }
	UCodeGEForceinlne f32 BytesTofloat(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt(DataBytes, Index);
		return *(f32*)&V;
	}



	Byte64 GetBytes(i64 Value);
	i64 BytesToInt64(const void* DataBytes, size_t Index);

	UCodeGEForceinlne Byte64 GetBytes(u64 Value) { return GetBytes(*(i64*)&Value); }
	UCodeGEForceinlne u64 BytesToUInt64(const void* DataBytes, size_t Index) {
		auto V = BytesToInt64(DataBytes, Index);
		return *(u64*)&V;
	}

	UCodeGEForceinlne Byte64 GetBytes(f64 Value) { return GetBytes(*(i64*)&Value); }
	UCodeGEForceinlne f64 BytesTofloat64(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt64(DataBytes, Index);
		return *(f64*)&V;
	}
	//Helpers
	UCodeGEForceinlne void BytesToBool(const void* DataBytes, size_t Index, bool* OutPut)
	{
		*OutPut = BytesToBool(DataBytes, Index);
	}
	UCodeGEForceinlne void BytesToChar(const void* DataBytes, size_t Index, signed char* OutPut)
	{
		*OutPut = BytesToSChar(DataBytes, Index);
	}
	UCodeGEForceinlne void BytesToChar(const void* DataBytes, size_t Index, char* OutPut)
	{
		*OutPut = BytesToChar(DataBytes, Index);
	}
	UCodeGEForceinlne void BytesToChar(const void* DataBytes, size_t Index, unsigned char* OutPut)
	{
		*OutPut = BytesToChar(DataBytes, Index);
	}

	UCodeGEForceinlne void BytesToInt16(const void* DataBytes, size_t Index, i16* OutPut)
	{
		*OutPut = BytesToInt16(DataBytes, Index);
	}
	UCodeGEForceinlne void BytesToInt16(const void* DataBytes, size_t Index, u16* OutPut)
	{
		*OutPut = BytesToInt16(DataBytes, Index);
	}

	UCodeGEForceinlne void BytesToInt(const void* DataBytes, size_t Index, int* OutPut)
	{
		*OutPut = BytesToInt(DataBytes, Index);
	}
	UCodeGEForceinlne void BytesToInt(const void* DataBytes, size_t Index, unsigned int* OutPut)
	{
		*OutPut = BytesToInt(DataBytes, Index);
	}

	UCodeGEForceinlne void BytesTofloat(const void* DataBytes, size_t Index, f32* OutPut)
	{
		*OutPut = BytesTofloat(DataBytes, Index);
	}

	UCodeGEForceinlne void BytesToInt64(const void* DataBytes, size_t Index, i64* OutPut)
	{
		*OutPut = BytesToInt64(DataBytes, Index);
	}
	UCodeGEForceinlne void BytesToInt64(const void* DataBytes, size_t Index, u64* OutPut)
	{
		*OutPut = BytesToUInt64(DataBytes, Index);
	}

	UCodeGEForceinlne void BytesTofloat64(const void* DataBytes, size_t Index, f64* OutPut)
	{
		*OutPut = BytesTofloat64(DataBytes, Index);
	}

	UCodeGEForceinlne void MoveBytes(const bool Value, void* DataBytes, size_t Index)
	{
		((Byte*)DataBytes)[Index] = GetBytes(Value);
	}
	UCodeGEForceinlne void MoveBytes(const signed char Value, void* DataBytes, size_t Index)
	{
		((Byte*)DataBytes)[Index] = GetBytes(Value);
	}
	UCodeGEForceinlne void MoveBytes(const char Value, void* DataBytes, size_t Index)
	{
		((Byte*)DataBytes)[Index] = GetBytes(Value);
	}
	UCodeGEForceinlne void MoveBytes(const unsigned char Value, void* DataBytes, size_t Index)
	{
		((Byte*)DataBytes)[Index] = GetBytes(Value);
	}

	UCodeGEForceinlne void MoveBytes(const i16 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}
	UCodeGEForceinlne void MoveBytes(const u16 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}

	UCodeGEForceinlne void MoveBytes(const u32 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}
	UCodeGEForceinlne void MoveBytes(const i32 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}
	UCodeGEForceinlne void MoveBytes(const f32 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}

	UCodeGEForceinlne void MoveBytes(const i64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}
	UCodeGEForceinlne void MoveBytes(const u64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}

	UCodeGEForceinlne void MoveBytes(const f64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}

};

inline static BitConverter littleEndianConverter = BitConverter(BitConverter::Endian::little);
inline static BitConverter BigEndianConverter = BitConverter(BitConverter::Endian::Big);
inline static BitConverter NativeEndianConverter = BitConverter(BitConverter::Get_CPU_Endian());
CoreEnd
