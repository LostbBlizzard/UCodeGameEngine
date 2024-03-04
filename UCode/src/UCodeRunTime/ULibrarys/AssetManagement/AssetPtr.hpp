#pragma once
#include "UCodeRunTime/ULibrarys/AssetManagement/UID.hpp"
#include "UCodeRunTime/BasicTypes/ManagedPtr.hpp"
#include "UCodeRunTime/BasicTypes/Variant.hpp"
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
	AssetPtr() 
	{

	}
	AssetPtr(const UID& ID):
		_Base(ID)
	{

	}
	AssetPtr(const ManagedAssetPtr& data):
		_Base(data)
	{

	}
	AssetPtr(AssetBase* data):	
		_Base(data)
	{

	}
	~AssetPtr()
	{
		
	}
	AssetPtr(const AssetPtr& ToCopy) = default;
	AssetPtr& operator=(const AssetPtr& ToCopy) = default;	
	AssetPtr(AssetPtr&& ToCopy) = default;
	AssetPtr& operator=(AssetPtr&& ToCopy) = default;


	UCodeGEForceinlne AssetPtr& operator=(AssetType* ToCopy) { _Base = ToCopy;return *this; }
	UCodeGEForceinlne AssetPtr& operator=(const UID& ToCopy) { _Base = ToCopy;return *this; }
	UCodeGEForceinlne AssetPtr& operator=(const ManagedAssetPtr& ToCopy) {_Base = ToCopy;return *this;}

	UCodeGEForceinlne State Get_State() const 
	{
		if (_Base.IsType<UID>())
		{
			return State::UID;
		}
		else if (_Base.IsType<ManagedAssetPtr>())
		{
			return State::ManagedPtr;
		}
		else if (_Base.IsType<AssetBase*>())
		{
			return State::Raw;
		}
		else
		{
			return State::Null;
		}
	}

	
	UID Get_UID() const
	{
		UCodeGEAssert(Has_UID());

		if (Get_State() == State::UID)
		{
			return _Base.GetType<UID>();
		}
		else if (Get_State() == State::ManagedPtr)
		{
			return _Base.GetType<ManagedAssetPtr>().Get_Value()->Uid.value();
		}
		else
		{
			#if UCodeGEDebug
			UCodeGEThrow("Cant Return A UID");
			#endif
			return {};
		}
	}
	ManagedAssetPtr Get_Managed() const
	{
		if (Get_State() == State::ManagedPtr)
		{
			return _Base.GetType<ManagedAssetPtr>();
		}
		else
		{
			#if UCodeGEDebug
			UCodeGEThrow("Cant Return ManagedAssetPtr");
			#endif
			UCodeGEUnreachable();
		}
	}
	ManagedAssetPtr Get_ManagedOr(ManagedAssetPtr Other) const
	{
		if (Get_State() == State::ManagedPtr)
		{
			return _Base.GetType<ManagedAssetPtr>();
		}
		else
		{
			return  Other;
		}
	}

	bool Has_Asset() const
	{
		return Get_State() == State::ManagedPtr;
	}

	bool Has_UID() const
	{
		return Get_State() == State::UID || 
			(Get_State() == State::ManagedPtr &&
				_Base.GetType<ManagedAssetPtr>().Has_Value() &&
				 _Base.GetType<ManagedAssetPtr>().Get_Value()->Uid.has_value());
	}
	
	AssetBase* Get_Asset()const
	{
		UCodeGEAssert(Has_Asset())

		if (Get_State() == State::ManagedPtr)
		{
			return &_Base.GetType<ManagedAssetPtr>().Get_Value()->_Base;
		}
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
	Variant<AssetBase*, UID, ManagedAssetPtr> _Base;
};
CoreEnd