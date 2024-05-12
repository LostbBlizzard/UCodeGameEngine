#include "RawEntityData.hpp"
CoreStart
RawEntityData::RawEntityData()
{
}
RawEntityData::RawEntityData(const UID& id,const UCode::Entity* Entity,USerializerType type) :_Data(),_UID(id)
{
	UCode::Scene2dData::SaveEntityData(Entity, _Data,type);
}

RawEntityData::RawEntityData(const UID& id,const UCode::Scene2dData::Entity_Data& Entity):_Data(Entity),_UID(id)
{
}

bool RawEntityData::WriteToFile(const Path&  Path, const RawEntityData& Data,USerializerType type)
{
	auto File = std::ofstream(Path);
	if (File.is_open())
	{
		USerializer output(type);

		ToBits(output, Data);
		//
		output.ToStream(File, true);

		File.close();
		return true;
	}
	return false;
}
bool RawEntityData::ReadFromFile(const Path& Path, RawEntityData& Data)
{
	UDeserializer input;
	if (UDeserializer::FromFile(Path, input)) 
	{
		FromBits(input, Data);

		return true;
	}
	return false;
}
void RawEntityData::ToBits(USerializer& output, const RawEntityData& Data)
{
	output.Write("_UID", Data._UID);
	output.Write("_Data", Data._Data);
}
void RawEntityData::FromBits(UDeserializer& input, RawEntityData& Data)
{
	input.ReadType("_UID", Data._UID);
	input.ReadType("_Data", Data._Data);
}


CoreEnd

