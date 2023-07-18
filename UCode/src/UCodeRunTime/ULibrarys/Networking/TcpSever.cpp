#include "TcpSever.hpp"
CoreStart
TcpSever::~TcpSever()
{
	if (IsSeverRuning()) {
		CloseSever();
	}
}
TcpSever::TcpSever()
{
	_ReviedBytes.resize(MaxReadBufferSize);
}
void TcpSever::StartSever(const Ip_t& IP, Port_t Port)
{
#ifdef DEBUG
	if (IsSeverRuning()) {
		throw std::exception("Sever Is Runing");
	}
#endif // DEBUG


	MinimalSocket::Port port = Port; // port the server needs to bind
	_Base = MinimalSocket::tcp::TcpServer(port, MinimalSocket::AddressFamily::IP_V4);

	_Clients.resize(MaxClients);
}
void TcpSever::Step()
{
#ifdef DEBUG
	if (!IsSeverRuning()) {
		throw std::exception("Sever Is not Runing");
	}
#endif // DEBUG

	auto OpClient = _Base.acceptNewClient(MinimalSocket::Timeout(1));
	if (OpClient.has_value())
	{
		optional<size_t> NewIndex;
		for (size_t i = 0; i < _Clients.size(); i++)
		{
			auto& Item = _Clients[i];
			if (Item._Base.get())
			{
				NewIndex = i;
				break;
			}
		}

		if (NewIndex.has_value())
		{
			_Clients[NewIndex.value()] = Client(NewIndex.value(), std::move(OpClient.value()));
		}
		else
		{
			//Max Clients
		}
	}

	
	MinimalSocket::Buffer V{ (char*)_ReviedBytes.data(),_ReviedBytes.size() };
	for (size_t i = 0; i < _Clients.size(); i++)
	{
		auto& Item = _Clients[i];
		if (Item._Base)
		{
			
			size_t BufSize = Item._Base->receive(V, MinimalSocket::Timeout(1));

			if (BufSize) {
				_ReceiveBytes(Item, BytesView::Make(_ReviedBytes.data(), BufSize));
			}
		}
	}
}
void TcpSever::CloseSever()
{
#ifdef DEBUG
	if (!IsSeverRuning()) {
		throw std::exception("sever Is not runing");
	}
#endif // DEBUG
	_Base = MinimalSocket::tcp::TcpServer();
}
CoreEnd