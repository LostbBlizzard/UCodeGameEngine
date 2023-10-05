#pragma once

#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <fstream>
CoreStart

struct FileBuffer
{
	FileBuffer();
	~FileBuffer();
	FileBuffer(FileBuffer&& source);
	FileBuffer& operator=(FileBuffer&& source);
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
	void Set_FileReadIndex(size_t Index);
	void ReadFullFile(Vector<Byte>& OutBuffer);
	void ReadFullFile(String& OutBuffer);
	Byte ReadByte();
private:
	void BufferCheck();
	Unique_array<Byte> _Buffer = nullptr;
	size_t _BufferSize = 0;
	size_t _BufferPos = 0;
	std::ifstream _File;
	size_t _FileSize = 0;
	bool _FullFileInMem = false;
	static size_t Get_BufferSize();
	void CanFitFullFileCheck();
	Byte* GetBufferoffset()
	{
		Byte* r = _Buffer.get() + _BufferPos;
		return r;
	}
};

CoreEnd