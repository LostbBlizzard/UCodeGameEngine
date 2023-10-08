#pragma once
#include "../Core/GameRunTime.hpp"
#include "UCodeRunTime/ULibrarys/Serialization_Library.h"
CoreStart

using DebugingClientPacketType_t = int;
enum class DebugingClientPacketType :DebugingClientPacketType_t
{
	Null,
};



using DebugingSeverPacketType_t = int;
enum class DebugingSeverPacketType :DebugingSeverPacketType_t
{
	Null,
	OnDebugLog,
};


struct DebugingLogPacket
{
	static constexpr DebugingSeverPacketType PacketType = DebugingSeverPacketType::OnDebugLog;

	String MGS;

	Unique_Bytes ToBytes() const
	{
		Unique_Bytes R;
		BitMaker m;
		ToBytes(m);

		R.Copyfrom(std::move(m.Get_Bytes()));
		return R;
	}
	static bool ReadBytes(const BytesView View, DebugingLogPacket& Out)
	{
		BitReader r;
		r.SetBytes(View);
		return ReadBytes(r,Out);
	}


	void ToBytes(BitMaker& maker) const
	{
		maker.WriteType(MGS);
	}
	static bool ReadBytes(BitReader& reader,DebugingLogPacket& Out)
	{
		reader.ReadType(Out.MGS, String());
		return true;
	}
};



struct DebugingClientPacket
{
	DebugingClientPacketType PacketType = DebugingClientPacketType::Null;
	Vector<Byte> Data;

	DebugingClientPacket(){}

	Unique_Bytes ToBytes() const
	{
		Unique_Bytes R;
		BitMaker m;
		m.WriteType((DebugingClientPacketType_t)PacketType);
		m.WriteType((BitMaker::SizeAsBits)Data.size());
		m.WriteBytes(Data.data(), Data.size());

		R.Copyfrom(std::move(m.Get_Bytes()));
		return R;
	}
	static DebugingClientPacket FromBytes(const BytesView bytes)
	{
		DebugingClientPacket r;
		BitReader m;
		m.SetBytes(bytes);

		m.ReadType(*(DebugingSeverPacketType_t*)&r.PacketType, (DebugingSeverPacketType_t)DebugingSeverPacketType::Null);

		BitReader::SizeAsBits BuffSize = 0;
		m.ReadType(BuffSize, BuffSize);

		r.Data.resize(BuffSize);
		m.ReadBytes(r.Data.data(), r.Data.size());

		return r;
	}

	template<typename T> void Set(const T& Value)
	{
		PacketType = T::PacketType;
		Data = Value.ToBytes();
	}
	template<typename T> Optional<T> As() const
	{
		
		#if UCodeGameEngineDEBUG
		if (PacketType != T::PacketType)
		{
			UCodeGameEngineThrowException("Bad cast");
		}
		#endif // DEBUG
		T R;
		if (T::ReadBytes(Data, R))
		{
			return R;
		}
		return {};
	}
};
struct DebugingSeverPacket
{
	DebugingSeverPacket() {}


	DebugingSeverPacketType PacketType = DebugingSeverPacketType::Null;
	Vector<Byte> Data;


	template<typename T> void Set(const T& Value)
	{
		PacketType = T::PacketType;
		Data = Value.ToBytes();
	}
	template<typename T> Optional<T> As() const
	{
#		ifdef UCodeGameEngineDEBUG
		if (PacketType != T::PacketType)
		{
			UCodeGameEngineThrowException("Bad cast");
		}
		#endif // DEBUG
		T R;
		if (T::ReadBytes(Data, R))
		{
			return R;
		}
		return {};
	}

	Unique_Bytes ToBytes() const
	{
		Unique_Bytes R;
		BitMaker m;
		m.WriteType((DebugingSeverPacketType_t)PacketType);
		m.WriteType((BitMaker::SizeAsBits)Data.size());
		m.WriteBytes(Data.data(), Data.size());

		R.Copyfrom(std::move(m.Get_Bytes()));
		return R;
	}
	static DebugingSeverPacket FromBytes(const BytesView bytes)
	{
		DebugingSeverPacket r;
		BitReader m;
		m.SetBytes(bytes);

		m.ReadType(*(DebugingSeverPacketType_t*)&r.PacketType, (DebugingSeverPacketType_t)DebugingSeverPacketType::Null);

		BitReader::SizeAsBits BuffSize = 0;
		m.ReadType(BuffSize, BuffSize);
		
		r.Data.resize(BuffSize);
		m.ReadBytes(r.Data.data(), r.Data.size());

		return r;
	}
};
CoreEnd