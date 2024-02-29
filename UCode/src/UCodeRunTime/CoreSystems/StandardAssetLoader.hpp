#pragma once
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
CoreStart
class StandardAssetLoader :public AssetLoader
{
public:
	GameFiles* gamefiles = nullptr;

	struct UIdMap
	{
		UnorderedMap<UID, Path> _Paths;

		void Serialize(USerializer& Out)
		{
			Out.Write("_Size", (BitMaker::SizeAsBits)_Paths.size());

			for (auto& Item : _Paths)
			{
				Out.Write("_UID", Item.first);
				Out.Write("_Path", Item.second.generic_string());
			}
		}
		void Deserialize(UDeserializer& Out)
		{
			BitMaker::SizeAsBits S = 0;
			Out.ReadType("_Size", S);
			_Paths.reserve(S);

			for (size_t i = 0; i < S; i++)
			{
				UID V1;
				Path V2;
				Out.ReadType("_UID", V1);
				Out.ReadType("_Path", V2);

				_Paths.AddValue(V1,std::move(V2));
			}
		}
		~UIdMap()
		{

		}

		inline static const char* FileWithDot = "UID.data";
	};
	struct LoadOnStart
	{
		Vector<UID> _LoadList;

		void Serialize(USerializer& Out)
		{
			Out.Write("_LoadList",_LoadList);	
		}
		void Deserialize(UDeserializer& Out)
		{
			Out.ReadType("_LoadList", _LoadList);
		}
		~LoadOnStart()
		{

		}

		inline static const char* FileWithDot = "LoadOnStart.data";
	};

	inline void Init(GameFiles* files, UCode::AssetManager* Manager)
	{
		gamefiles = files;
		_Manager = Manager;
	
		auto byte = gamefiles->ReadGameFileAsBytes(GameFilesData::GetUCodeDir() / Path(UIdMap::FileWithDot).native());
		
		UDeserializer v;
		v.SetBytes(byte.AsView());
		_uids.Deserialize(v);

	}

	inline const GameFilesData& Get_GameFile()
	{
		return gamefiles->Get_FilesData();
	}

	Optional<Unique_ptr<Asset>> LoadAsset(const UID& Path) override
	{
		auto v = GetUIDToPath(Path);
		if (v.has_value()) {
			return LoadAsset(v.value());
		}
		else
		{
			return {};
		}
	}
	Optional<Unique_ptr<Asset>> LoadAsset(const Path& Path) override;
private:
	UIdMap _uids;
	Optional<Path> GetUIDToPath(const UID& id)
	{
		if (_uids._Paths.HasValue(id))
		{
			return _uids._Paths.GetValue(id);
		}
		return {};
	}
};
CoreEnd

