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

			int B = 0xffff;
			client.SendBytes(UCode::BytesView::Make((UCode::Byte*)B, sizeof(B)));
	});
	client.Set_GetBytes([&](const UCode::BytesView bytes)
	{
			Count++;
	});


	client.Connet();


	while (Count =! 4)
	{
		client.Step();
		sever.Step();
	}

	return Count == 4;
}
EditorTestEnd