#include "EditorIndex.hpp"
#include <filesystem>
#include <fstream>
#include <UCodeRunTime/ULibrarys/Serialization_Library.h>
//#include <fileapi.h>
#include "Helper/FileHelper.hpp"
EditorStart


Vector< EditorIndex::ChangedFile> EditorIndex::GetDiffFromDir(const Path& path) const
{
	Vector<ChangedFile> R;

	Unordered_map<const IndexFile*, bool> hasfiles;
	hasfiles.reserve(_Files.size());

	for (auto& Item : _Files)
	{
		hasfiles.AddValue(&Item, false);
	}



	for (auto Item : std::filesystem::recursive_directory_iterator(path))
	{
		if (!Item.is_regular_file())
		{
			continue;
		}
		auto p = Item.path();
		auto relpath = FileHelper::ToRelativePath(path, p);

		auto info = FindFileRelativePath(relpath.generic_string());

		if (info.has_value())
		{
			hasfiles.GetValue(&info.value()) = true;

			auto& oldinfo = info.value();

			bool hasbeenupdated = false;

			u64 lastwritetime =std::filesystem::last_write_time(p).time_since_epoch().count();
			if (oldinfo.FileLastUpdatedTime == lastwritetime)
			{
				hasbeenupdated = true;
			}
			else if (oldinfo.FileSize == std::filesystem::file_size(p))
			{
				hasbeenupdated = true;
			}

			if (hasbeenupdated)
			{
				ChangedFile f;
				f.RelativePath = relpath;
				f.Type = ChangedFileType::FileUpdated;
				R.push_back(std::move(f));
			}

		}
		else
		{
			ChangedFile f;
			f.RelativePath = relpath;
			f.Type = ChangedFileType::FileAdded;
			R.push_back(std::move(f));
		}
	}

	for (auto& Item : hasfiles)
	{
		if (Item.second == false)
		{
			ChangedFile f;
			f.RelativePath = Item.first->RelativePath;
			f.Type = ChangedFileType::FileRemoved;

			R.push_back(std::move(f));
		}
	}

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
		Bits.Resize(File.tellg());
		File.seekg(0, File.beg);
		File.read((char*)Bits.Data(), Bits.Size());

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
		File.write((const char*)Bits.Data(), Bits.Size());
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
	R.Copyfrom(std::move(bit.Get_Bytes()));
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
		UCodeGEUnreachable();
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

