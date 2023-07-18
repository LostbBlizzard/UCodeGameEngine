#include "BookOfDebuging.hpp"

#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart

void DebugingClient::Connet(DebugingSever* Sever)
{
	#ifdef DEBUG
	if (IsConneted())
	{
		throw std::exception("Is Conneted");
	}
	#endif // DEBUG

	SeverConnection = Sever;
}

void DebugingClient::Connet(const UCode::Ip_t& SeverIp, UCode::Port_t SeverPort)
{
#ifdef DEBUG
	if (IsConneted())
	{
		throw std::exception("Is Conneted");
	}
#endif // DEBUG

	NetworkSeverData V;
	V.SeverIp = SeverIp;
	V.SeverPort = SeverPort;
	V.Connection = std::move(TcpClient());


	V.Connection.Set_GetBytes([this](const BytesView bytes)
	{
			ReadPackets.push_back(DebugingSeverPacket::FromBytes(bytes));
	});

	V.Connection.Connet(SeverIp, SeverPort);


	SeverConnection =std::move(V);
}

void DebugingClient::Disconnect()
{

}
void DebugingClient::Update()
{
#ifdef DEBUG
	if (!IsConneted())
	{
		throw std::exception("Is not Conneted");
	}
#endif // DEBUG
	UpdateReadData();
	UpdateSendData();
}

bool DebugingClient::IsConneted()
{
	if (SeverConnection.IsType<None>())
	{
		return false;
	}
	else if (auto Val = SeverConnection.IfType<DebugingSever*>())
	{
		return *Val;
	}
	else if (auto Val = SeverConnection.IfType<NetworkSeverData>())
	{
		return Val->Connection.IsConneted();
	}
}
void DebugingClient::UpdateReadData()
{
	if (auto Val = SeverConnection.IfType<NetworkSeverData>())
	{
		Val->Connection.Step();
	}
	else if (auto Val = SeverConnection.IfType<DebugingSever*>())
	{
		throw std::exception("not added");
		//(*Val)->
	}

	auto Packets = std::move(ReadPackets);

	for (auto& Item : Packets)
	{
		ReadPacket(std::move(Item));
	}
}
void DebugingClient::UpdateSendData()
{
	auto Packets = std::move(WritePackets);


	for (auto& Item : Packets)
	{
		SendToSever(std::move(Item));
	}
}
void DebugingClient::SendToSever(DebugingClientPacket&& Packet)
{
	if (auto Val = SeverConnection.IfType<NetworkSeverData>())
	{
		auto Bits = Packet.ToBytes();
		Val->Connection.SendBytes(Bits.AsView());
	}
	else if (auto Val = SeverConnection.IfType<DebugingSever*>())
	{
		(*Val)->FromClient(this, std::move(Packet));
	}
}
void DebugingClient::ReadPacket(DebugingSeverPacket&& Packet)
{
	ReadPackets.push_back(std::move(Packet));
}
void DebugingSever::StartSever(const UCode::Ip_t& SeverIp, UCode::Port_t SeverPort)
{
#ifdef DEBUG
	if (!IsSeverRuning())
	{
		throw std::exception("Sever is runing");
	}
#endif // DEBUG

	NetworkSeverData net;
	net.SeverIp = SeverIp;
	net.SeverPort = SeverPort;
	net._Sever = TcpSever();
	net._Sever.Set_MaxClients(1);

	net._Sever.Set_GetBytes([this](TcpSever::Client& client, const BytesView bytes)
		{
			ReadPackets.push_back(DebugingClientPacket::FromBytes(bytes));
		});
	net._Sever.StartSever(SeverIp, SeverPort);
}

void DebugingSever::StartSever(DebugingClient* client)
{
#ifdef DEBUG
	if (!IsSeverRuning())
	{
		throw std::exception("Sever is runing");
	}
#endif // DEBUG

	ClientConnection = client;
}

void DebugingSever::CloseSever()
{
	ClientConnection = None();
}

void DebugingSever::Update()
{
	UpdateReadData();
	UpdateSendData();
}

bool DebugingSever::IsSeverRuning()
{
	if (auto Val = ClientConnection.IfType< DebugingClient*>())
	{
		return (*Val)->IsConneted();
	}
	else if (auto Val = ClientConnection.IfType<NetworkSeverData>())
	{
		return Val->_Sever.IsSeverRuning();
	}

	return false;
}


void DebugingSever::NextPacketAll(DebugingSeverPacket&& Packet)
{
	WritePackets.push_back(Packet);
}

void DebugingSever::UpdateSendData()
{
	auto Packets = std::move(WritePackets);
	for (auto& Item : Packets)
	{
		SendToAllClient(std::move(Item));
	}
}

void DebugingSever::SendToAllClient(DebugingSeverPacket&& Packet)
{
	if (auto Val = ClientConnection.IfType<NetworkSeverData>())
	{
		auto Bits = Packet.ToBytes();
		Val->_Sever.SendToClientAll(Bits.AsView());
	}
	else if (auto Val = ClientConnection.IfType<DebugingClient*>())
	{
		(*Val)->ReadPacket(std::move(Packet));
	}
}

void DebugingSever::UpdateReadData()
{
	if (auto Val = ClientConnection.IfType<NetworkSeverData>())
	{
		Val->_Sever.Step();
	}
	auto Packets = std::move(ReadPackets);
}

void DebugingSever::FromClient(DebugingClient* client, DebugingClientPacket&& Packet)
{

#ifdef DEBUG
	if (!IsSeverRuning())
	{
		throw std::exception("Sever not runing");
	}
	if (auto V = ClientConnection.IfType<DebugingClient*>())
	{
		if (*V != client)
		{
			throw std::exception("bad client");
		}
	}
	else
	{
		throw std::exception("is net sever not interal sever");
	}
#endif // DEBUG
	ReadPackets.push_back(std::move(Packet));
}



constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::BookOfDebuging;
BookOfDebuging* BookOfDebuging::Get(Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	if (V) { return (BookOfDebuging*)V; }

	BookOfDebuging* r = new BookOfDebuging(lib);;

	lib->MoveBook(r);
	lib->SetStaticComponent(KeyIdex, r);
	return r;
}

BookOfDebuging* BookOfDebuging::Find(const Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	return (BookOfDebuging*)V;
}
BookOfDebuging::BookOfDebuging(Gamelibrary* lib) :libraryBook(lib)
{

}
CoreEnd