#pragma once

#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "UCodeRunTime/UDefs.hpp"
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
	UCODE_ENGINE_FORCE_INLINE static Endian Get_CPU_Endian()
	{
		if (_CPUEndian == Endian::NaN)
		{
			_CPUEndian = _GetEndian();
		}

		return _CPUEndian;
	}
	UCODE_ENGINE_FORCE_INLINE static Endian _GetEndian()
	{
		union
		{
			SInt32 NumValue;
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
	static_assert(sizeof(Byte) == 1, " 'Byte' is not 1 bytes");
	static_assert(sizeof(UInt16) == 2, " 'UInt16' is not 2 bytes");
	static_assert(sizeof(UInt32) == 4, " 'UInt32' is not 4 bytes");
	static_assert(sizeof(UInt64) == 8, " 'UInt64' is not 8 bytes");

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



	UCODE_ENGINE_FORCE_INLINE Byte GetBytes(bool Value)
	{
		return (Byte)Value;
	}
	UCODE_ENGINE_FORCE_INLINE bool BytesToBool(const void* DataBytes, size_t Index)
	{
		Byte B = ((Byte*)DataBytes)[Index];
		return B;
	}
	UCODE_ENGINE_FORCE_INLINE Byte GetBytes(signed char Value)
	{
		return GetBytes((unsigned char)Value);
	}
	UCODE_ENGINE_FORCE_INLINE signed char BytesToSChar(const void* DataBytes, size_t Index)
	{
		return BytesToUChar(DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE Byte GetBytes(char Value)
	{
		return GetBytes((unsigned char)Value);
	}
	UCODE_ENGINE_FORCE_INLINE char BytesToChar(const void* DataBytes, size_t Index)
	{
		return BytesToUChar(DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE Byte GetBytes(unsigned char Value)
	{
		return Value;
	}
	UCODE_ENGINE_FORCE_INLINE unsigned char BytesToUChar(const void* DataBytes, size_t Index)
	{
		Byte B = ((const Byte*)DataBytes)[Index];
		return B;
	}

	Byte16 GetBytes(SInt16 Value);
	SInt16 BytesToInt16(const void* DataBytes, size_t Index);

	UCODE_ENGINE_FORCE_INLINE Byte16 GetBytes(UInt16 Value) { return GetBytes(*(SInt16*)&Value); }
	UCODE_ENGINE_FORCE_INLINE UInt16 BytesToUInt16(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt16(DataBytes, Index);
		return *(UInt16*)&V;
	}

	Byte32 GetBytes(SInt32 Value);
	SInt32 BytesToInt(const void* DataBytes, size_t Index);

	UCODE_ENGINE_FORCE_INLINE Byte32 GetBytes(UInt32 Value) { return GetBytes(*(int*)&Value); }
	UCODE_ENGINE_FORCE_INLINE UInt32 BytesToUInt(const void* DataBytes, size_t Index) {
		auto V = BytesToInt(DataBytes, Index);
		return *(unsigned int*)&V;
	}

	UCODE_ENGINE_FORCE_INLINE Byte32 GetBytes(float32 Value) { return GetBytes(*(int*)&Value); }
	UCODE_ENGINE_FORCE_INLINE float32 BytesTofloat(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt(DataBytes, Index);
		return *(float32*)&V;
	}



	Byte64 GetBytes(SInt64 Value);
	SInt64 BytesToInt64(const void* DataBytes, size_t Index);

	UCODE_ENGINE_FORCE_INLINE Byte64 GetBytes(UInt64 Value) { return GetBytes(*(SInt64*)&Value); }
	UCODE_ENGINE_FORCE_INLINE UInt64 BytesToUInt64(const void* DataBytes, size_t Index) {
		auto V = BytesToInt64(DataBytes, Index);
		return *(UInt64*)&V;
	}

	UCODE_ENGINE_FORCE_INLINE Byte64 GetBytes(float64 Value) { return GetBytes(*(SInt64*)&Value); }
	UCODE_ENGINE_FORCE_INLINE float64 BytesTofloat64(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt64(DataBytes, Index);
		return *(float64*)&V;
	}
	//Helpers
	UCODE_ENGINE_FORCE_INLINE void BytesToBool(const void* DataBytes, size_t Index, bool* OutPut)
	{
		*OutPut = BytesToBool(DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void BytesToChar(const void* DataBytes, size_t Index, signed char* OutPut)
	{
		*OutPut = BytesToSChar(DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void BytesToChar(const void* DataBytes, size_t Index, char* OutPut)
	{
		*OutPut = BytesToChar(DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void BytesToChar(const void* DataBytes, size_t Index, unsigned char* OutPut)
	{
		*OutPut = BytesToChar(DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void BytesToInt16(const void* DataBytes, size_t Index, SInt16* OutPut)
	{
		*OutPut = BytesToInt16(DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void BytesToInt16(const void* DataBytes, size_t Index, UInt16* OutPut)
	{
		*OutPut = BytesToInt16(DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void BytesToInt(const void* DataBytes, size_t Index, int* OutPut)
	{
		*OutPut = BytesToInt(DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void BytesToInt(const void* DataBytes, size_t Index, unsigned int* OutPut)
	{
		*OutPut = BytesToInt(DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void BytesTofloat(const void* DataBytes, size_t Index, float32* OutPut)
	{
		*OutPut = BytesTofloat(DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void BytesToInt64(const void* DataBytes, size_t Index, SInt64* OutPut)
	{
		*OutPut = BytesToInt64(DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void BytesToInt64(const void* DataBytes, size_t Index, UInt64* OutPut)
	{
		*OutPut = BytesToUInt64(DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void BytesTofloat64(const void* DataBytes, size_t Index, float64* OutPut)
	{
		*OutPut = BytesTofloat64(DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const bool Value, void* DataBytes, size_t Index)
	{
		((Byte*)DataBytes)[Index] = GetBytes(Value);
	}
	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const signed char Value, void* DataBytes, size_t Index)
	{
		((Byte*)DataBytes)[Index] = GetBytes(Value);
	}
	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const char Value, void* DataBytes, size_t Index)
	{
		((Byte*)DataBytes)[Index] = GetBytes(Value);
	}
	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const unsigned char Value, void* DataBytes, size_t Index)
	{
		((Byte*)DataBytes)[Index] = GetBytes(Value);
	}

	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const SInt16 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const UInt16 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const UInt32 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const SInt32 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const float32 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const SInt64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}
	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const UInt64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}

	UCODE_ENGINE_FORCE_INLINE void MoveBytes(const float64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((Byte*)DataBytes, Index);
	}

};

inline static BitConverter littleEndianConverter = BitConverter(BitConverter::Endian::little);
inline static BitConverter BigEndianConverter = BitConverter(BitConverter::Endian::Big);
inline static BitConverter NativeEndianConverter = BitConverter(BitConverter::Get_CPU_Endian());
CoreEnd
