#include "BitConverter.hpp"
CoreStart
using Byte = UCode::Byte;
using Byte16 = BitConverter::Byte16;
using Byte32 = BitConverter::Byte32;
using Byte64 = BitConverter::Byte64;



#define G_FixEndianess  const Endian ThisEndian = Get_CPU_Endian();\
const Endian OutEndian = InputOutEndian;\
if (ThisEndian != OutEndian) {return R.FlpBytes();}\

#define R_FixEndianess const Endian ThisEndian = Get_CPU_Endian(); \
const Endian InputEndian = InputOutEndian;\
if (ThisEndian != InputEndian) {auto v = bits.FlpBytes();bits =v; }\

Byte32 BitConverter::GetBytes(SInt32 Value)
{
	Byte32 R;
	Byte* VPointer = (Byte*)&Value;

	R.A = VPointer[0];
	R.B = VPointer[1];
	R.C = VPointer[2];
	R.D = VPointer[3];



	G_FixEndianess
	

	return R;
}
SInt32 BitConverter::BytesToInt(const void* DataBytes, size_t Index)
{
	Byte* B = ((Byte*)DataBytes) + Index;
	Byte32 bits = *(Byte32*)B;
	int* R = (int*)&bits;

	R_FixEndianess
	

	return *R;
}

Byte16 BitConverter::GetBytes(SInt16 Value)
{
	Byte16 R;
	Byte* VPointer = (Byte*)&Value;

	R.A = VPointer[0];
	R.B = VPointer[1];
	
	G_FixEndianess
	return R;
}
SInt16 BitConverter::BytesToInt16(const void* DataBytes, size_t Index)
{
	Byte* B = ((Byte*)DataBytes) + Index;
	Byte16 bits = *(Byte16*)B;
	SInt16* R = (SInt16*)&bits;

	R_FixEndianess
	return *R;
}

Byte64 BitConverter::GetBytes(SInt64 Value)
{
	Byte64 R;
	Byte* VPointer = (Byte*)&Value;

	R.A = VPointer[0];
	R.B = VPointer[1];
	R.C = VPointer[2];
	R.D = VPointer[3];

	R.E = VPointer[4];
	R.F = VPointer[5];
	R.G = VPointer[6];
	R.H = VPointer[7];

	G_FixEndianess
	return R;
}
SInt64 BitConverter::BytesToInt64(const void* DataBytes, size_t Index)
{
	Byte* B = ((Byte*)DataBytes) + Index;
	Byte64 bits = *(Byte64*)B;
	SInt64* R = (SInt64*)&bits;

	R_FixEndianess
	return *R;
}
CoreEnd