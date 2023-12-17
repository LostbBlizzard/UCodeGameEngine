#pragma once
#include "UCodeRunTime/ULibrarys/AssetManagement/UID.hpp"
#include "UCodeRunTime/BasicTypes/ManagedPtr.hpp"
CoreStart

template<typename AssetType, typename AssetBase>
struct AssetPtr
{
public:
	using State_t = Byte;
	using ManagedAssetPtr = ManagedPtr< AssetType>;
	enum class State : State_t
	{
		Null,
		Raw,
		UID,
		ManagedPtr,
	};
	AssetPtr() :_State(State::Null)
	{

	}
	AssetPtr(const UID& ID) :_State(State::UID), _UID(ID)
	{

	}
	AssetPtr(const ManagedAssetPtr& data) :_State(State::ManagedPtr),_Managed(data)
	{

	}
	AssetPtr(AssetBase* data) :_State(State::Raw), _Raw(data)
	{

	}
	~AssetPtr()
	{
		switch (_State)
		{
		case State::Null:
		case State::Raw:
			break;
		case State::UID:
			_UID.~UID();
			break;
		case State::ManagedPtr:
			_Managed.~ManagedPtr<AssetType>();
			break;
		default:
			break;
		}
	}
	AssetPtr(const AssetPtr& ToCopy) { this->operator=(ToCopy); }
	AssetPtr& operator=(const AssetPtr& ToCopy)
	{
		this->~AssetPtr();
		_State = ToCopy._State;
		switch (_State)
		{
		case State::Null:
			break;
		case State::Raw:
			_Raw = ToCopy._Raw;
			break;
		case State::UID:
			_UID = ToCopy._UID;
			break;
		case State::ManagedPtr:
			_Managed = ToCopy._Managed;
			break;
		default:
			UCodeGEUnreachable();
			break;
		}
		return *this;
	}

	UCodeGEForceinlne AssetPtr& operator=(AssetType* ToCopy) { return *this = AssetPtr(ToCopy); }
	UCodeGEForceinlne AssetPtr& operator=(const UID& ToCopy) { return *this = AssetPtr(ToCopy); }
	UCodeGEForceinlne AssetPtr& operator=(const ManagedAssetPtr& ToCopy) { return *this = AssetPtr(ToCopy); }

	UCodeGEForceinlne State Get_State() const { return _State; }

	
	UID Get_UID() const
	{
		if (_State == State::UID)
		{
			return  _UID;
		}
		else
		{
			UCodeGEThrow("Cant Return A UID");
			return {};
		}
	}
	ManagedAssetPtr Get_Managed() const
	{
		if (_State == State::ManagedPtr)
		{
			return  _Managed;
		}
		else
		{
			UCodeGEThrow("Cant Return ManagedAssetPtr");
			return {};
		}
	}
	ManagedAssetPtr Get_ManagedOr(ManagedAssetPtr Other) const
	{
		if (_State == State::ManagedPtr)
		{
			return  _Managed;
		}
		else
		{
			return  Other;
		}
	}

	bool Has_Asset() const
	{
		return false;
	}

	bool Has_UID() const
	{
		return _State == State::UID;
	}
	
	AssetBase* Get_Asset()const
	{
		return nullptr;
	}
	AssetBase* Get_AssetOr(AssetBase* Other)const
	{
		if (Has_Asset())
		{
			return Get_Asset();
		}
		return  Other;
	}
private:
	State _State;
	union
	{
		AssetBase* _Raw;
		UID _UID;
		ManagedAssetPtr _Managed;
	};

};
CoreEnd