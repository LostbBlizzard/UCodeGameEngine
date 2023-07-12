#pragma once
#include "../Editor/EditorNamespace.hpp"
#include "Typedefs.hpp"
EditorStart
class UPlugin
{
public:
	inline static const char* FileEx = "UPlugin";
	inline static const char* FileExWithDot = ".UPlugin";

	struct FileIndex
	{
		Path FileFullName;
		UID FileInternalID;
		size_t FileOffset = 0;
		size_t FileSize = 0;
		

		inline Path Get_FileName() const
		{
			return FileFullName.filename();
		}
		inline Path Get_FileExtension() const
		{
			return FileFullName.extension();
		}
		inline Path Get_FileDirectory() const
		{
			return FileFullName.root_directory();
		}

	};

	String PluginName;
	Version PluginVersion;
	UInt64 BuildNumber;
	Vector<FileIndex> Offsets;
	Vector<Byte> _Data;


	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path& path, const UPlugin& Data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const  Path& path, UPlugin& Data);
	UCODE_EDITOR_NODISCARD static Unique_Bytes ToBits(const UPlugin& Data);
	UCODE_EDITOR_NODISCARD static bool FromBits(const BytesView input, UPlugin& Data);
};
EditorEnd
