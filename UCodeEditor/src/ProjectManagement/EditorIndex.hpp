#pragma once
#include "../Typedefs.hpp"

#include <FileWatcher/FileWatcher.h>
EditorStart

enum class ChangedFileType
{
	FileAdded,
	FileRemoved,
	FileUpdated,
};
class EditorIndex
{
public:
	struct IndexFile
	{
		String RelativeAssetName;
		String RelativePath;
		u64 FileSize = 0;
		u64 FileLastUpdatedTime = 0;
		u64 FileHash = 0;

		Optional<UID> UserID;
 	};

	Vector<IndexFile> _Files;


	
	struct ChangedFile
	{
		ChangedFileType Type;
		Path RelativePath;
	};

	
	void FindFilesRelativePath(StringView relativepath,Vector<const IndexFile*>& out) const
	{
		out.clear();
		for (auto& Item : _Files)
		{
			if (Item.RelativePath == relativepath)
			{
				out.push_back(&Item);
			}
		}
	}
	void FindFilesRelativePath(StringView relativepath,Vector<IndexFile*>& out) 
	{
		return ((const EditorIndex*)this)->FindFilesRelativePath(relativepath,*(Vector<const IndexFile*>*)& out);
	}
	Vector<IndexFile*> FindFilesRelativePath(StringView relativepath) 
	{
		Vector<IndexFile*> r;
		FindFilesRelativePath(relativepath, r);
		return r;
	}
	OptionalRef<IndexFile> FindFileRelativeAssetName(StringView relativeassetname)
	{
		for (auto& Item : _Files)
		{
			if (Item.RelativeAssetName == relativeassetname)
			{
				return UCode::Optionalref(Item);
			}
		}

		return {};
	}
	const OptionalRef<IndexFile> FindFileRelativeAssetName(StringView relativeassetname) const
	{
		for (auto& Item : _Files)
		{
			if (Item.RelativeAssetName == relativeassetname)
			{
				return UCode::Optionalref(Item);
			}
		}

		return {};
	}
	void RemoveIndexFilesRelativePath(StringView relativepath)
	{
		_Files.erase(std::remove_if(
			_Files.begin(), _Files.end(),
			[&relativepath](const IndexFile& x) {
				return x.RelativePath ==relativepath; // put your condition here
			}), _Files.end());

	}

	Vector<ChangedFile> GetDiffFromDir(const Path& path) const;

	static bool FromFile(EditorIndex& Out, const Path& path);
	static bool ToFile(const EditorIndex& Out, const Path& path);
	static void UpdateFile(IndexFile& file, const Path& path, const String& relativepath);

	static bool FromBytes(EditorIndex& Out, const BytesView Bytes);
	static Unique_Bytes ToBytes(const EditorIndex& Out);

	static inline const char* FileName = "Index.data";
};


class FileWatcher :FW::FileWatchListener
{
public:
	using FileUpdate = void (*)(void* This,const Path& path, ChangedFileType Type);

	void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
		FW::Action action) override;
	ChangedFileType ToType(FW::Action action);

	void Step();

	void init(const Path& path);
	~FileWatcher();

	void CloseWatcher();

	void* ThisPointer = nullptr;
	FileUpdate CallBack = nullptr;
private:
	FW::FileWatcher fileWatcher;
	FW::WatchID watchID = 0;
};

EditorEnd