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
#if UCodeGEDebug
	if (IsConneted())
	{
		UCodeGEThrow("Client Is Runing");
	}
#endif // DEBUG
	
	_Base = Unique_ptr<MinimalSocket::tcp::TcpClient>(new MinimalSocket::tcp::TcpClient(MinimalSocket::Address(IP,Port)));
	bool V = _Base->open(MinimalSocket::Timeout(1));
	if (V)
	{
		_isConneted = true;


		if (_OnConnet.has_value()) 
		{
			(*_OnConnet)();
		}
	}
	else
	{
		_Base = {};
		if (_OnConnetFail.has_value()) {
			(*_OnConnetFail)();
		}
	}
	
}

void TcpClient::Disconnet()
{
#if UCodeGEDebug
	if (!IsConneted())
	{
		UCodeGEThrow("Client Is not Runing");
	}
#endif // DEBUG
	_Base.reset();

	_isConneted = false;
}

void TcpClient::Step()
{
#if UCodeGEDebug
	if (!IsConneted())
	{
		
		//UCodeGEThrow("Client Is Runing");
	}
#endif // DEBUG
	if (_Base) {
		size_t BufSize = _Base->receive(MinimalSocket::Buffer{ (char*)_RevecivedBytes.data(),_RevecivedBytes.size() }, MinimalSocket::Timeout(1));
		if (BufSize)
		{
			if (_isConneted == false) {
				_isConneted = true;
			}
			if (_GetBytes.has_value()) {
				(*_GetBytes)(BytesView::Make(_RevecivedBytes.data(), BufSize));
			}
		}
	}
}

bool TcpClient::IsConneted()
{
	if (_Base)
	{
		return _isConneted;
	}
	else
	{
		return false;
	}
}

void TcpClient::SendBytes(const BytesView Bytes)
{
#if UCodeGEDebug
	if (!IsConneted())
	{
		UCodeGEThrow("Client Is not Runing");
	}
#endif // DEBUG
	_Base->send(MinimalSocket::ConstBuffer{ (const char*)Bytes.Data(),Bytes.Size() });
}

CoreEnd