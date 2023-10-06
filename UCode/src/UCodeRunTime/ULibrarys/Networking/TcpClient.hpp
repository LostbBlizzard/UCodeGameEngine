#pragma once

#include <UCodeRunTime/Core/CoreNamespace.hpp>

#include "NetTypedef.h"
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/ULibrarys/Serialization/BitMaker.hpp>

#include <MinimalSocket/tcp/TcpClient.h>
CoreStart

class TcpClient
{
public:
	using OnConnet = Delegate<void>;
	using OnConnetFail = Delegate<void>;
	using OnGetBytes = Delegate<void,const BytesView>;
	using OnDisconnet = Delegate<void>;

	static constexpr size_t MaxReadBufferSize = 1024;
	TcpClient();
	~TcpClient();
	TcpClient(TcpClient&& Other) = default;
	TcpClient& operator=(TcpClient&& Other) = default;


	void Connet(const Ip_t& IP = default_address, Port_t Port = default_port);

	void Disconnet();

	void Step();

	bool IsConneted();


	void SendBytes(const BitMaker& bits)
	{
		auto Bytes = bits.Get_Bytes();
		SendBytes(BytesView::Make(Bytes.data(), Bytes.size()));
	}
	void SendBytes(const BytesView Bytes);

	void Set_OnConnet(OnConnet Value)
	{
		_OnConnet = Value;
	}
	void Set_OnConnetFail(OnConnetFail Value)
	{
		_OnConnetFail = Value;
	}
	void Set_GetBytes(OnGetBytes Value)
	{
		_GetBytes = Value;
	}
	void Set_OnDisconnet(OnDisconnet Value)
	{
		_OnDisconnet = Value;
	}
private:
	Unique_ptr<MinimalSocket::tcp::TcpClient> _Base;
	Vector<Byte> _RevecivedBytes;
	OnConnet _OnConnet;
	OnConnetFail _OnConnetFail;
	OnGetBytes _GetBytes;
	OnDisconnet _OnDisconnet;
};

CoreEnd

