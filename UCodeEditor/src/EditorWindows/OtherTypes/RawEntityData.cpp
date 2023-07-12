#include "RawEntityData.hpp"
EditorStart
RawEntityData::RawEntityData()
{
}
RawEntityData::RawEntityData(const UCode::Entity* Entity) :_Data()
{
	UCode::Scene2dData::SaveEntityData(Entity, _Data);
}

RawEntityData::RawEntityData(const UCode::Scene2dData::Entity_Data& Entity):_Data(Entity)
{
}

bool RawEntityData::WriteToFile(const Path&  Path, const RawEntityData& Data)
{
	std::ofstream File(Path);
	if (File.is_open()) {
		YAML::Emitter Node(File);

		Node << YAML::BeginMap;
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

		Data._Data = Node["_Data"].as<UCode::Scene2dData::Entity_Data>();
		
		File.close();
		return true;
	}
	return false;
}

EditorEnd

