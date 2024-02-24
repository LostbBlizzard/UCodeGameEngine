#pragma once
#include "../Editor/EditorNamespace.hpp"
#include "Typedefs.hpp"
#include "Serialization.hpp"
EditorStart


struct UPluginID
{
	String DeveloperName;
	String PluginName;
	Version VersonNumber;

	void To(BitMaker& serializer) const
	{
		serializer.WriteType(DeveloperName);
		serializer.WriteType(PluginName);
		serializer.WriteType(VersonNumber);
	}
	void From(BitReader& serializer)
	{
		serializer.ReadType(DeveloperName,String());
		serializer.ReadType(PluginName,String());
		serializer.ReadType(VersonNumber,Version());
	}
};
EditorEnd

CoreStart

template<>
struct BitData<UCodeEditor::UPluginID>
{
 	static void ToBytes(BitMaker& serializer,const UCodeEditor::UPluginID& Plugin)
	{
		Plugin.To(serializer);
	}
	static void FromBytes(BitReader& serializer, UCodeEditor::UPluginID& Plugin)
	{
		Plugin.From(serializer);
	}

};

CoreEnd

namespace YAML
{
	template<>
	struct convert<UCodeEditor::UPluginID>
	{
		static Node encode(const UCodeEditor::UPluginID& rhs) {
			Node node;
			node.push_back(rhs.DeveloperName);
			node.push_back(rhs.PluginName);
			node.push_back(rhs.VersonNumber);
			return node;
		}

		static bool decode(const Node& node, UCodeEditor::UPluginID& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.DeveloperName = node[0].as<UCode::String>();
			rhs.PluginName = node[1].as<UCode::String>();
			rhs.VersonNumber = node[2].as<UCode::Version>();
			return true;
		}
	};
}

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

	UPluginID PluginID;
	Vector<FileIndex> Offsets;
	Vector<Byte> _Data;


	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path& path, const UPlugin& Data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const  Path& path, UPlugin& Data);
	UCODE_EDITOR_NODISCARD static Unique_Bytes ToBits(const UPlugin& Data);
	UCODE_EDITOR_NODISCARD static bool FromBits(const BytesView input, UPlugin& Data);
};

struct PluginDataBase
{
	struct PluginData
	{
		String DeveloperName;
		String PluginName;
		Path FileLocation;
	};
};



EditorEnd

