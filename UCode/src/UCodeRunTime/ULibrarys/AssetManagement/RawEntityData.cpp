#include "RawEntityData.hpp"
CoreStart
RawEntityData::RawEntityData()
{
}
RawEntityData::RawEntityData(const UID& id,const UCode::Entity* Entity) :_Data(),_UID(id)
{
	UCode::Scene2dData::SaveEntityData(Entity, _Data);
}

RawEntityData::RawEntityData(const UID& id,const UCode::Scene2dData::Entity_Data& Entity):_Data(Entity),_UID(id)
{
}

bool RawEntityData::WriteToFile(const Path&  Path, const RawEntityData& Data)
{
	std::ofstream File(Path);
	if (File.is_open()) {
		YAML::Emitter Node(File);

		Node << YAML::BeginMap;
		Node << YAML::Key << "_UID" << YAML::Value << Data._UID;
		Node << YAML::Key << "_Data" << YAML::Value << Data._Data;
		Node << YAML::EndMap;
		File.close();
		return true;
	}
	return false;
}
bool RawEntityData::ReadFromFile(const Path&  Path, RawEntityData& Data)
{
	std::ifstream File(Path);
	if (File.is_open())
	{
		YAML::Node Node(YAML::Load(File));

		Data._UID = Node["_UID"].as<UID>();
		Data._Data = Node["_Data"].as<UCode::Scene2dData::Entity_Data>();
		
		File.close();
		return true;
	}
	return false;
}
CoreEnd

