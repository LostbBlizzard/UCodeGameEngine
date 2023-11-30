#pragma once


#include "../Core/GameRunTime.hpp"
#include "DebugingPackets.hpp"

#include "UCodeRunTime/ULibrarys/Networking/TcpClient.hpp"
#include "UCodeRunTime/ULibrarys/Networking/TcpSever.hpp"
CoreStart


//the thing reading the Debuging info

struct DebugingSever;
struct DebugingClient
{
	friend DebugingSever;
	DebugingClient()
	{

	}
	~DebugingClient()
	{

	}


	void Connet(DebugingSever* Sever);
	void Connet(const UCode::Ip_t& SeverIp= UCode::default_address, UCode::Port_t SeverPort = UCode::default_port);
	void Disconnect();

	void Update();

	bool IsConneted();

	
	
private:
	struct NetworkSeverData
	{
		UCode::Ip_t SeverIp = UCode::default_address;
		UCode::Port_t  SeverPort = UCode::default_port;
		TcpClient Connection;
	};
	struct None{};
	Variant<None,DebugingSever*, NetworkSeverData> SeverConnection;

	Vector<DebugingSeverPacket> ReadPackets;
	Vector<DebugingClientPacket> WritePackets;


	//
	void UpdateSendData();
	void UpdateReadData();
	void SendToSever(DebugingClientPacket&& Packet);
	void ReadPacket(DebugingSeverPacket&& Packet);
};

//the thing outputing the Debuging info
struct DebugingSever 
{
	friend DebugingClient;
	struct GameRunTimeInfo
	{
		GameRunTime* Runtime;
		String RunTimeName;
	};


	void StartSever(const UCode::Ip_t& SeverIp = UCode::default_address, UCode::Port_t SeverPort = UCode::default_port);
	void StartSever(DebugingClient* client);

	void CloseSever();

	void Update();
	
	bool IsSeverRuning();

	void AddRunTimeToDebug(GameRunTimeInfo Info)
	{
		gameruntimes.push_back(Info);
	}
	void RemoveRunTime(GameRunTime* toremove)
	{
		for (size_t i = 0; i < gameruntimes.size(); i++)
		{
			auto& Item = gameruntimes[i];
			if (Item.Runtime == toremove)
			{
				gameruntimes.erase(gameruntimes.begin() + i);
			}
		}
	}
private:
	struct None{};
	struct NetworkSeverData
	{
		String SeverIp = "0.0.0.0";
		int SeverPort = 80;
		TcpSever _Sever;


	};
	Variant<None,DebugingClient*, NetworkSeverData> ClientConnection;
	
	Vector<DebugingClientPacket> ReadPackets;
	Vector<DebugingSeverPacket> WritePackets;

	struct ClientInfo
	{

	};
	ClientInfo _ClientInfo;

	void NextPacketAll(DebugingSeverPacket&& Packet);
	void UpdateSendData();
	void SendToAllClient(DebugingSeverPacket&& Packet);
	void UpdateReadData();
	void FromClient(DebugingClient* client, DebugingClientPacket&& Packet);


	
	Vector<GameRunTimeInfo> gameruntimes;
};


class Debuging :private System
{
public:
	DebugingSever debugsever;
	
	static Debuging* Get(Gamelibrary* lib);
	static Debuging* Find(const Gamelibrary* lib);
private:
	Debuging(Gamelibrary* lib);
};

CoreEnd