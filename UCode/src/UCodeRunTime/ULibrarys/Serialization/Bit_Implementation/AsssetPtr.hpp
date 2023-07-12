#pragma once
#include "UCodeRunTime/ULibrarys/Serialization/BitMaker.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetPtr.hpp"
CoreStart


template<typename Asset, typename AssetBase>
struct BitData<UCode::AssetPtr<Asset,AssetBase>>
{
	using TypePtr = UCode::AssetPtr<Asset,AssetBase>;
	static void ToBytes(BitMaker& This,const TypePtr& Value)
	{
		auto tepv = Value.Get_State();

		bool SetAsAsset = false;
		if (tepv == TypePtr::State::ManagedPtr)
		{
			if (Value.Get_Managed().Has_Value())
			{
				if (Value.Get_Managed().Get_Value()->Uid.has_value())
				{

					tepv = TypePtr::State::UID;
					SetAsAsset = true;
				}
			}
		}
		This.WriteType(*(TypePtr::State_t*)&tepv);
		switch (Value.Get_State())
		{
		case TypePtr::State::UID:
			This.WriteType(Value.Get_UID());
			break;
		default:
			break;
		}

		if (SetAsAsset)
		{
			This.WriteType(Value.Get_Managed().Get_Value()->Uid.value());
		}
	}
	static void FromBytes(BitReader& This, TypePtr& Out)
	{
		TypePtr::State V= TypePtr::State::Null;
		This.ReadType(*(TypePtr::State_t*)&V, *(TypePtr::State_t*)&V);
		switch (V)
		{
		case TypePtr::State::UID:
		{
			UID tep = UID();
			This.ReadType(tep, tep);
			Out = tep;
		}
		break;
		default:
			break;
		}
	}
};

template<>
struct BitData<UCode::UID>
{
	using Type = UCode::UID;
	static void ToBytes(BitMaker& This, const Type& Value)
	{
		This.WriteBytes((const Byte*)&Value,sizeof(UID::UID_t));
	}
	static void FromBytes(BitReader& This, Type& Out)
	{
		Byte* pointer = (Byte*)&Out;
		for (size_t i = 0; i < sizeof(UID::UID_t); i++)
		{
			This.ReadType<Byte>(pointer[i], pointer[i]);
		}
	}
};

CoreEnd