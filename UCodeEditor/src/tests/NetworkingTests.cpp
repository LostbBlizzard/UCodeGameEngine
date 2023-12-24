#pragma once
#include "NetworkingTests.hpp"

#include "UCodeRunTime/ULibrarys/Network_Library.hpp"

EditorTestStart
bool NetTest_1()
{
	int Count = 0;

	
	UCode::TcpSever sever;
	sever.Set_NewClient([&](UCode::TcpSever::Client& V)
	{
			Count++;
			V.SendToClient("Hi Client");
	});

	sever.Set_GetBytes([&](UCode::TcpSever::Client& V,const UCode::BytesView bytes)
	{
			Count++;
			V.SendToClient(bytes);
	});

	sever.StartSever();

	UCode::TcpClient client;
	client.Set_OnConnet([&]()
	{
			Count++;
			client.SendBytes("Hi Sever");
	});
	client.Set_OnConnetFail([&]()
	{
			client.Connet();
	});
	client.Set_GetBytes([&](const UCode::BytesView bytes)
	{
			Count++;
	});


	client.Connet();


	while (Count != 4)
	{
		sever.Step();
		client.Step();
	}

	return Count == 4;
}
EditorTestEnd