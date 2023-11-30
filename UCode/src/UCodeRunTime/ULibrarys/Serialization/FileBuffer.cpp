#include "FileBuffer.hpp"
#include <filesystem>
#include <UCodeRunTime/CoreSystems/GameFiles.hpp>

#if UCodeGameEnginePlatformWindows
#include <Windows.h>
#include <shlobj.h>
#endif // UCode_Build_Windows_OS
CoreStart
namespace fs = std::filesystem;
FileBuffer::FileBuffer()
{

}
bool FileBuffer::Open(const Path& path)
{
	Close();

	_File.open(path, std::ios::in | std::ios::binary);
	auto r = _File.is_open();
	if (r) {
		_FileSize = fs::file_size(path);
	}return r;
}
void FileBuffer::Close()
{
	if (IsOpen())
	{
		_File.close();
		_FileSize = 0;
		_FullFileInMem = false;
	}
}
size_t FileBuffer::Get_BufferSize()
{
	size_t BufferSize =0;
	#if UCodeGameEnginePlatformWindows
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);

	BufferSize = sysInfo.dwPageSize;
	#endif // UCode_Build_Windows_OS
	return BufferSize * 8;
}
void FileBuffer::BufferCheck()
{
	if (_Buffer == nullptr)
	{
		size_t BufferSize = Get_BufferSize();


		_Buffer = std::make_unique<Byte[]>(BufferSize);
		_BufferPos = 0;
		_BufferSize = BufferSize;
	}
}
void FileBuffer::CanFitFullFileCheck()
{
	if (_FileSize <= _BufferSize)
	{
		if (_FullFileInMem == false)
		{
			_Buffer = std::make_unique<Byte[]>(_FileSize);
			_BufferSize = _FileSize;
			_BufferPos = 0;

			_File.read((char*)_Buffer.get(), _FileSize);
			_FullFileInMem = true;
		}

	

	}
}
void FileBuffer::ReadBytes(Byte* bytes, size_t Count)
{
	BufferCheck();
	CanFitFullFileCheck();


	if (_FullFileInMem)
	{
		memcpy(bytes, &_Buffer[_BufferStart+_BufferPos], Count);
	}
	else
	{
		Byte* Ptr = GetBufferoffset();
		_File.read((char*)bytes, Count);
	}
	_BufferPos += Count;
	if (_BufferPos == _BufferSize)
	{
		_BufferPos = 0;
	}
}
void FileBuffer::ReadType(GameFileIndex& Out)
{
	ReadType(Out.FileFullName);

	u64 V = 0;

	ReadType(V);

	Out.FileOffset = V;

	ReadType(V);

	Out.FileSize = V;
}

void FileBuffer::Set_FileReadIndex(size_t Index)
{
	if (_FullFileInMem)
	{
		_BufferPos = Index;
	}
	else
	{
		_File.seekg(_BufferStart + Index);
	}
}

void FileBuffer::ReadFullFile(Vector<Byte>& OutBuffer)
{
	Set_FileReadIndex(0);
	if (_FullFileInMem && _BufferSize)
	{
		OutBuffer.reserve(_BufferSize);
		memcpy(&OutBuffer[0], _Buffer.get(), _BufferSize);
	}
	else
	{
		if (_FileSize)
		{
			OutBuffer.resize(_FileSize);
			_File.read((char*)&OutBuffer[0], _FileSize);
		}
	}
}

void FileBuffer::ReadFullFile(String& OutBuffer)
{
	Set_FileReadIndex(0);
	if (_FullFileInMem && _BufferSize)
	{
		OutBuffer.reserve(_BufferSize);
		memcpy(&OutBuffer[0], _Buffer.get(), _BufferSize);
	}
	else
	{
		if (_FileSize)
		{
			OutBuffer.resize(_FileSize);
			_File.read((char*)&OutBuffer[0], _FileSize);
		}
	}
}

FileBuffer::~FileBuffer()
{
	Close();
}
CoreEnd