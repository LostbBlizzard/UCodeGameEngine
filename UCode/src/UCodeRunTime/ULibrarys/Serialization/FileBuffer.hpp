#pragma once

#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/UID.hpp>
#include <fstream>
CoreStart


struct GameFileIndex;

struct FileBuffer
{
	FileBuffer();
	~FileBuffer();
	FileBuffer(FileBuffer&& source) = default;
	FileBuffer& operator=(FileBuffer&& source) = default;
	bool Open(const Path& path);
	void Close();
	bool IsOpen()
	{
		return _File.is_open();
	}

	UCodeGameEngineForceinlne auto Get_FileSize()
	{
		return _FileSize;
	}
	UCodeGameEngineForceinlne size_t Get_FileReadIndex()
	{
		if (_FullFileInMem)
		{
			return _BufferPos;
		}
		else 
		{
			return _File.tellg();
		}
	}
	void Set_FileReadStartIndex(size_t Index)
	{
		_BufferStart = Index;
	}

	void Set_FileReadIndex(size_t Index);
	void ReadFullFile(Vector<Byte>& OutBuffer);
	void ReadFullFile(String& OutBuffer);

	void ReadBytes(Byte* bytes, size_t Count);
	Byte ReadByte()
	{
		Byte r;
		ReadBytes(&r, 1);
		return r;
	}
	using SizeBit = u32;
	
	void ReadType(String& Out)
	{
		SizeBit Size = 0;
		ReadType(Size);


		Out.resize(Size);

		for (size_t i = 0; i < Size; i++)
		{
			ReadType(Out[i]);
		}
	}
	template<typename T>
	void ReadType(Vector<T>& Out)
	{
		SizeBit Size=0;
		ReadType(Size);


		Out.resize(Size);

		for (size_t i = 0; i < Size; i++)
		{
			ReadType(Out[i]);
		}
	}
	void ReadType(Path& Out)
	{
		String V;
		ReadType(V);
		Out = std::move(V);
	}


	void ReadType(u8& Out)
	{
		Out =ReadByte();
	}
	void ReadType(char& Out)
	{
		Out = ReadByte();
	}
	void ReadType(u16& Out)
	{
		ReadBytes((Byte*)&Out, sizeof(Out));
	}
	void ReadType(u32& Out)
	{
		ReadBytes((Byte*)&Out, sizeof(Out));
	}
	void ReadType(u64& Out)
	{
		ReadBytes((Byte*)&Out, sizeof(Out));
	}
	void ReadType(i8& Out)
	{
		ReadBytes((Byte*)&Out, sizeof(Out));
	}
	void ReadType(i16& Out)
	{
		ReadBytes((Byte*)&Out, sizeof(Out));
	}
	void ReadType(i32& Out)
	{
		ReadBytes((Byte*)&Out, sizeof(Out));
	}
	void ReadType(i64& Out)
	{
		ReadBytes((Byte*)&Out, sizeof(Out));
	}
	void ReadType(UID& Out)
	{
		UID::UID_t V =0;
		ReadType(V);
		Out.Set_Value(V);
	}
	
	
	void ReadType(GameFileIndex& Out);
private:
	void BufferCheck();
	Unique_array<Byte> _Buffer = nullptr;
	size_t _BufferSize = 0;
	size_t _BufferPos = 0;
	size_t _BufferStart = 0;
	std::ifstream _File;
	size_t _FileSize = 0;
	bool _FullFileInMem = false;
	static size_t Get_BufferSize();
	void CanFitFullFileCheck();
	Byte* GetBufferoffset()
	{
		Byte* r = _Buffer.get() + _BufferStart + _BufferPos;
		return r;
	}
};

CoreEnd