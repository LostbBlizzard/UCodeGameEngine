#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>

#include "NetTypedef.h"
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/ULibrarys/Serialization/BitMaker.hpp>

#include <MinimalSocket/tcp/TcpServer.h>
CoreStart

class TcpSever
{
public:
	
	class Client
	{
	public:
		friend  TcpSever;

		void SendToClient(const BitMaker& Value)
		{
			auto& Bytes = Value.Get_Bytes();
			SendToClient(BytesView::Make(Bytes.data(), Bytes.size()));
		}
		void SendToClient(const BytesView Value)
		{
			_Base->send(MinimalSocket::ConstBuffer{ (const char*)Value.Data(),Value.Size() });
		}


		//0 -> MaxClients -1
		size_t Get_ClientIndex()
		{
			return _ClientIndex;
		}
		void Disconnet()
		{

		}
		Client()
			: _Base(nullptr), _ClientIndex(0)
		{

		}
	private:

		Client(size_t ClientIndex)
			: _Base(nullptr), _ClientIndex(ClientIndex)
		{

		}
		Client(size_t ClientIndex, MinimalSocket::tcp::TcpConnection&& Base)
			: _Base(std::make_unique< MinimalSocket::tcp::TcpConnection>(std::move(Base))), _ClientIndex(ClientIndex)
		{

		}
		Unique_ptr<MinimalSocket::tcp::TcpConnection> _Base;
		size_t _ClientIndex;
	};
	using OnNewClient = Delegate<void,Client&>;
	using OnClientGetBytes = Delegate<void, Client&,const BytesView>;
	using OnClientDisconnet = Delegate<void, Client&>;

	static constexpr size_t MaxReadBufferSize = 1024;

	TcpSever();
	~TcpSever();
	TcpSever(TcpSever&& Other) = default;
	TcpSever& operator=(TcpSever&& Other) = default;

	void StartSever(const Ip_t& IP = default_address, Port_t Port = default_port);

	void Step();

	void Set_MaxClients(size_t Value)
	{
		MaxClients = Value;
	}
	size_t Get_MaxClients()
	{
		return MaxClients;
	}

	bool IsSeverRuning()
	{
		return _Base.wasOpened();
	}

	void CloseSever();


	void SendToClientAll(const BitMaker& Value)
	{
		auto& Bytes = Value.Get_Bytes();
		SendToClientAll(BytesView::Make(Bytes.data(), Bytes.size()));
	}
	void SendToClientAll(const BytesView Value)
	{
		for (auto& Item : _Clients)
		{
			Item.SendToClient(Value);
		}
	}

	Vector<Client>& Get_Clients()
	{
		return _Clients;
	}
	const Vector<Client>& Get_Clients() const
	{
		return _Clients;
	}

	void Set_GetBytes(OnClientGetBytes Value)
	{
		_ReceiveBytes = Value;
	}
	void Set_NewClient(OnNewClient Value)
	{
		_Client = Value;
	}
	void Set_ClientDisconnet(OnClientDisconnet  Value)
	{
		_Disconnet = Value;
	}
	Client& Get_Client(size_t ClientIndex)
	{
		return _Clients[ClientIndex];
	}
	const Client& Get_Client(size_t ClientIndex) const
	{
		return _Clients[ClientIndex];
	}


private:
	size_t MaxClients = 1;
	MinimalSocket::tcp::TcpServer _Base;
	Vector<Client> _Clients;
	Vector<Byte> _ReviedBytes;
	OnNewClient _Client;
	OnClientGetBytes _ReceiveBytes;
	OnClientDisconnet _Disconnet;

	
};
CoreEnd