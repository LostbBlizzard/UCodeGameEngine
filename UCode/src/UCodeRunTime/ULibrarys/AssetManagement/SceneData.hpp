#pragma once
#include <UCodeRunTime/CoreRunTime.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/ULibrarys/Serialization_Library.h>
#include "UID.hpp"
CoreStart
class Scene2dData
{
public:
	inline static const char* FileExt = "UScene";
	inline static const char* FileExtDot = ".UScene";
	struct Compoent_Data
	{
		String _CompoentType;
		String _Data;
		Compoent_Data()
		{

		}
		Compoent_Data(const String& id,const String& ptr) : _CompoentType(id), _Data(ptr)
		{

		}
	};
	struct Entity_Data
	{
		Vec3 _LocalPosition;
		Vec3 _LocalRotation;
		Vec3 _LocalScale;

		
		Vector<Compoent_Data> _Compoents;
		Vector<Entity_Data> _Entitys;
		String _Name;
	};
	Scene2dData();

	void PushData(USerializer& node);
	static bool FromString(Scene2dData& out, UDeserializer& text);

	static bool FromFile(Scene2dData& out, const Path& Path);
	static bool ToFile(const Path& path, Scene2dData& data, USerializerType Type);
	inline static Entity* CloneEntity(Entity* entity)
	{
		return CloneEntity(entity, entity->Get_Scene());
	}
	static Entity* CloneEntity(Entity* entity, RunTimeScene* Scene);
	static void SaveScene(const RunTimeScene* Scene,Scene2dData& Out, USerializerType type = USerializerType::Default);
	static void SaveEntityData(const Entity* Item, Entity_Data& Entity, USerializerType type = USerializerType::Default);
	static bool SaveCompoentData(const Compoent* Item, Compoent_Data& Out,USerializerType type = USerializerType::Default);
	static bool SaveCompoentDataAsText(const Compoent* Item, USerializer& serializer);

	static RunTimeScene* LoadScene(GameRunTime* runtime, const Scene2dData& Data);
	static RunTimeScene* AsyncLoadScene(GameRunTime* runtime, const Scene2dData& Data,bool RunAsMultThread =false);
	
	static void LoadEntity(Entity* Entity, const Entity_Data& Item);
	static Compoent* LoadCompoent(Entity* Entity, const Compoent_Data& Item);
	

	String _Name;
	UID _UID;
	Vector<Entity_Data> _Entitys;

};


template<> struct BitData<Scene2dData>
{
	using _Type = Scene2dData;
	static void ToBytes(BitMaker& This, const _Type& Value)
	{
		This.WriteType(Value._Name);
		This.WriteType(Value._UID);
		This.WriteType(Value._Entitys);
	}
	static void FromBytes(BitReader& This, _Type& Out)
	{
		This.ReadType(Out._Name, Out._Name);
		This.ReadType(Out._UID, Out._UID);
		This.ReadType(Out._Entitys, Out._Entitys);
	}
};

template<> struct BitData<Scene2dData::Entity_Data>
{
	using _Type = Scene2dData::Entity_Data;
	static void ToBytes(BitMaker& This, const _Type& Value)
	{
		This.WriteType(Value._LocalPosition);
		This.WriteType(Value._LocalRotation);
		This.WriteType(Value._LocalScale);

		This.WriteType(Value._Compoents);

		This.WriteType(Value._Entitys);

		This.WriteType(Value._Name);
	}
	static void FromBytes(BitReader& This, _Type& Out)
	{
		This.ReadType(Out._LocalPosition, Out._LocalPosition);
		This.ReadType(Out._LocalRotation, Out._LocalRotation);
		This.ReadType(Out._LocalScale, Out._LocalScale);


		This.ReadType(Out._Compoents, Out._Compoents);

		This.ReadType(Out._Entitys, Out._Entitys);

		This.ReadType(Out._Name, Out._Name);
	}
};
template<> struct BitData<Scene2dData::Compoent_Data>
{
	using _Type = Scene2dData::Compoent_Data;
	static void ToBytes(BitMaker& This, const _Type& Value)
	{
		This.WriteType(Value._CompoentType);
		This.WriteType(Value._Data);
	}
	static void FromBytes(BitReader& This, _Type& Out)
	{
		This.ReadType(Out._CompoentType, Out._CompoentType);
		This.ReadType(Out._Data, Out._Data);
	}
};
CoreEnd

namespace YAML {//This should be moved somewere else
	template<> struct convert<UCode::Scene2dData::Entity_Data> {
		static Node encode(const UCode::Scene2dData::Entity_Data& rhs) {
			Node node;
			node["_LocalPosition"] = rhs._LocalPosition;
			node["_LocalRotation"] = rhs._LocalRotation;
			node["_LocalScale"] = rhs._LocalScale;


			node["_Compoents"] = rhs._Compoents;
			node["_Entitys"] = rhs._Entitys;

			node["_Name"] = rhs._Name;

			return node;
		}

		static bool decode(const Node& node, UCode::Scene2dData::Entity_Data& rhs)
		{
			rhs._LocalPosition = node["_LocalPosition"].as<UCode::Vec3>(rhs._LocalPosition);
			rhs._LocalRotation = node["_LocalRotation"].as<UCode::Vec3>(rhs._LocalRotation);
			rhs._LocalScale = node["_LocalScale"].as<UCode::Vec3>(rhs._LocalScale);


			rhs._Compoents = node["_Compoents"].as<UCode::Vector<UCode::Scene2dData::Compoent_Data>>(rhs._Compoents);

			rhs._Entitys = node["_Entitys"].as<UCode::Vector<UCode::Scene2dData::Entity_Data>>(rhs._Entitys);

			rhs._Name = node["_Name"].as<UCode::String>(rhs._Name);
			return true;
		}
	};

	template<> struct convert<UCode::Scene2dData::Compoent_Data> {
		static Node encode(const UCode::Scene2dData::Compoent_Data& rhs) {
			Node node;
		 	node["_TypeID"] = rhs._CompoentType;
			node["_Data"] = YAML::Load(rhs._Data.substr(1));


			return node;
		}

		static bool decode(const Node& node, UCode::Scene2dData::Compoent_Data& rhs) {


			rhs._CompoentType = node["_TypeID"].as<UCode::String>(rhs._CompoentType);

			auto& DataNode = node["_Data"];
			YAML::Emitter E;
			E << DataNode;

			rhs._Data += (char)(UCode::USerializerType::YAML);
			rhs._Data += E.c_str();
			return true;
		}
	};
};