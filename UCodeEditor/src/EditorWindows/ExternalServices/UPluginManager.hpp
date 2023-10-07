#pragma once
#include <Typedefs.hpp>

#include "UGlobalState.hpp"
EditorStart


struct PluginIndex
{
	struct PluginInfo
	{
		String PluginName;
		Version PluginVersion;
		u64 BuildNumber;

		Path FilePath;
	};

	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path& path, const PluginIndex& Data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const  Path& path, PluginIndex& Data);
	UCODE_EDITOR_NODISCARD static Unique_Bytes ToBits(const PluginIndex& Data);
	UCODE_EDITOR_NODISCARD static bool FromBits(const BytesView input,PluginIndex& Data);

	inline static const char* FileName = "PluginIndex.data";

	Vector<PluginInfo> _Plugins;
};
class UPluginManager
{
	
	Path GetPluginIndexPath();

	Path GetPluginDownloadsPath();

	PluginIndex GetPluginIndex();
	void SavePluginIndex(const 	PluginIndex& Value);
}; 

EditorEnd
