#include "WinClientSocket.hpp"
CoreStart
WinCilentSocket::WinCilentSocket(size_t buffersize):
	_buffersize(buffersize), 
	_OutputBuffer(buffersize),
	_InputBuffer(buffersize),
	_OnConnect(),
	_OnDisconnect(),
	_CallBacks()
{
	
}
WinCilentSocket::~WinCilentSocket()
{
}
void WinCilentSocket::UpdateCalls()
{
}
void WinCilentSocket::ConnectToSever(std::string IP, SInt32 port)
{
}
void WinCilentSocket::Disconnect()
{
}
void WinCilentSocket::SendData(CallBackKeyType key,const Byte* _Bytes, size_t BytesSize)
{
}
CoreEnd