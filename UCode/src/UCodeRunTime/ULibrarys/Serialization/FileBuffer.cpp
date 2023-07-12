#include "FileBuffer.hpp"
#include <filesystem>

#ifdef UCode_Build_Windows_OS
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
	size_t BufferSize;
#ifdef UCode_Build_Windows_OS
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);

	BufferSize = sysInfo.dwPageSize;
#endif // UCode_Build_Windows_OS
	return BufferSize;
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
		if (_Buffer == nullptr)
		{
			_Buffer =std::make_unique<Byte[]>(_FileSize);
			_BufferSize = _FileSize;
			_BufferPos = 0;
		}

		_File.read((char*)_Buffer.get(), _FileSize);
		_FullFileInMem = true;

	}
}
Byte FileBuffer::ReadByte()
{
	CanFitFullFileCheck();
	BufferCheck();


	Byte V;
	if (_FullFileInMem)
	{
		V = _Buffer[_BufferPos];
	}
	else
	{
		Byte* Ptr = GetBufferoffset();
		_File.read((char*)Ptr, 1);

		V = _Buffer[_BufferPos];
	}
	_BufferPos += 1;
	if (_BufferPos == _BufferSize)
	{
		_BufferPos = 0;
	}
	return V;
}

void FileBuffer::Set_FileReadIndex(size_t Index)
{
	if (_FullFileInMem)
	{
		_BufferPos = Index;
	}
	else
	{
		_File.seekg(Index);
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