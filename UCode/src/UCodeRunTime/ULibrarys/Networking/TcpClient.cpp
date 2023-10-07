#include "TcpClient.hpp"
CoreStart

TcpClient::TcpClient()
{
	_RevecivedBytes.resize(MaxReadBufferSize);
}

TcpClient::~TcpClient()
{

}

void TcpClient::Connet(const Ip_t& IP, Port_t Port)
{
#if UCodeGameEngineDEBUG
	if (IsConneted())
	{
		throw std::exception("Client Is Runing");
	}
#endif // DEBUG
	
	_Base = Unique_ptr<MinimalSocket::tcp::TcpClient>(new MinimalSocket::tcp::TcpClient(MinimalSocket::Address(IP,Port)));
	bool V = _Base->open(MinimalSocket::Timeout(1));
	if (V)
	{
		_OnConnet();
	}
	else
	{
		_OnConnetFail();
	}
}

void TcpClient::Disconnet()
{
#if UCodeGameEngineDEBUG
	if (!IsConneted())
	{
		throw std::exception("Client Is not Runing");
	}
#endif // DEBUG
	_Base.reset();
}

void TcpClient::Step()
{
#if UCodeGameEngineDEBUG
	if (IsConneted())
	{
		throw std::exception("Client Is Runing");
	}
#endif // DEBUG

	size_t BufSize = _Base->receive(MinimalSocket::Buffer{ (char*)_RevecivedBytes.data(),_RevecivedBytes.size() }, MinimalSocket::Timeout(1));
	if (BufSize)
	{
		_GetBytes(BytesView::Make(_RevecivedBytes.data(), BufSize));
	}
}

bool TcpClient::IsConneted()
{
	if (_Base)
	{
		return _Base->wasOpened();
	}
	else
	{
		return false;
	}
}

void TcpClient::SendBytes(const BytesView Bytes)
{
#if UCodeGameEngineDEBUG
	if (!IsConneted())
	{
		throw std::exception("Client Is not Runing");
	}
#endif // DEBUG
	_Base->send(MinimalSocket::ConstBuffer{ (const char*)Bytes.Data(),Bytes.Size() });
}

CoreEnd