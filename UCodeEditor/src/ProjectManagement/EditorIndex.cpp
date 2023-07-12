#include "EditorIndex.hpp"
#include <filesystem>
#include <fstream>
#include <UCodeRunTime/ULibrarys/Serialization_Library.h>
//#include <fileapi.h>
EditorStart


void EditorIndex::GetDataFromDir(const Path& path)
{
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

	_Files.clear();
	for (const auto& dirEntry : recursive_directory_iterator(path)) 
	{
		if (dirEntry.is_regular_file())
		{
			IndexFile Index;
			Index.FileLastUpdatedTime = *(UInt64*)&dirEntry.last_write_time();
			Index.FileHash = 0;
			Index.FileSize = (UInt64)dirEntry.file_size();
			Index.RelativePath = dirEntry.path().generic_string().substr(path.native().size());
		
			_Files.push_back(std::move(Index));
		}
	}
}
Vector< EditorIndex::ChangedFile> EditorIndex::GetDiffFromDir(const Path& path) const
{
	Vector<ChangedFile> R;


	return R;
}

bool EditorIndex::FromFile(EditorIndex& Out, const Path& path)
{
	std::ifstream File;
	File.open(path, std::ios::binary);
	if (File.is_open())
	{
		Unique_Bytes Bits;
		File.seekg(0, File.end);
		Bits.Size = File.tellg();
		File.seekg(0, File.beg);
		Bits.Pointer = std::make_unique<Byte[]>(Bits.Size);
		File.read((char*)Bits.Pointer.get(), Bits.Size);

		return  FromBytes(Out,Bits.AsView());
	}
	else
	{
		return false;
	}
}
bool EditorIndex::ToFile(const EditorIndex& Out, const Path& path)
{

	std::ofstream File;
	File.open(path,std::ios_base::binary);
	if (File.is_open()) 
	{
		auto Bits = ToBytes(Out);
		File.write((const char*)Bits.Pointer.get(), Bits.Size);
		File.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool EditorIndex::FromBytes(EditorIndex& Out, const BytesView Bytes)
{
	UCode::BitReader bits;
	bits.SetBytes(Bytes);

	UCode::BitReader::SizeAsBits Size = 0;
	bits.ReadType(Size, Size);

	Out._Files.resize(Size);
	for (size_t i = 0; i < (size_t)Size; i++)
	{
		IndexFile F;

		bits.ReadType(F.RelativePath, F.RelativePath);

		bits.ReadType(F.FileSize, F.FileSize);

		bits.ReadType(F.FileLastUpdatedTime, F.FileLastUpdatedTime);

		bits.ReadType(F.FileHash, F.FileHash);

		bits.ReadType(F.UserID, F.UserID);

		Out._Files[i] = std::move(F);
	}

	return false;
}

Unique_Bytes EditorIndex::ToBytes(const EditorIndex& Out)
{
	UCode::BitMaker bit;
	bit.WriteType((UCode::BitMaker::SizeAsBits)Out._Files.size());
	for (size_t i = 0; i < Out._Files.size(); i++)
	{
		auto& Item = Out._Files[i];

		bit.WriteType(Item.RelativePath);

		bit.WriteType(Item.FileSize);

		bit.WriteType(Item.FileLastUpdatedTime);

		bit.WriteType(Item.FileHash);

		bit.WriteType(Item.UserID);
	}


	Unique_Bytes R;
	R.Pointer.reset(new Byte[bit.Size()]);
	R.Size = bit.Size();

	memcpy(R.Pointer.get(), bit.Get_Bytes().data(), bit.Size());

	return R;
}

void FileWatcher::handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action)
{
	if (CallBack)
	{
		CallBack(ThisPointer, filename, ToType(action));
	}
}

ChangedFileType FileWatcher::ToType(FW::Action action)
{
	switch (action)
	{
	case FW::Actions::Add:return ChangedFileType::FileAdded;
	case FW::Actions::Delete:return ChangedFileType::FileRemoved;
	case FW::Actions::Modified:return ChangedFileType::FileUpdated;
	default:
		throw std::exception("bad path");
		break;
	}
}

void FileWatcher::Step()
{
	fileWatcher.update();
}

void FileWatcher::init(const Path& path)
{
	watchID = fileWatcher.addWatch(path.c_str(), this, true);
}

FileWatcher::~FileWatcher()
{
	
}

void FileWatcher::CloseWatcher()
{
	fileWatcher.removeWatch(watchID);
	watchID = 0;
}

EditorEnd

