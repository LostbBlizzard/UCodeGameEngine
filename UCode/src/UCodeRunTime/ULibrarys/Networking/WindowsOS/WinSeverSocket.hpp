#pragma once

#include <UCodeRunTime/RunTimeBasicTypes/Delegate.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/unordered_map.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
CoreStart
class WinSeverSocket
{
public:
	typedef unsigned char Byte;
	typedef Vector<Byte> ByteBuffer;
	template<typename R, typename ...pars> using CallBackType = Delegate<R, pars...>;
	typedef Byte CallBackKeyType;
	typedef Byte PlayerIDType;
	struct ClientSocket
	{
	public:
		ClientSocket() :_ID(0){}

		inline void Ser_ID(PlayerIDType id)
		{
			_ID = id;
		};
		void SendData(CallBackKeyType key, const Byte* _Bytes, size_t datasize);
		inline PlayerIDType Get_ID(){ return _ID; }
	private:
		PlayerIDType _ID;
	};
	typedef CallBackType<void, ClientSocket&> OnConnectType;
	typedef CallBackType<void, ClientSocket&> OnDisConnectType;
	typedef CallBackType<void, ClientSocket&, const Byte*, size_t> OnDataCallBackType;


	template<typename T> using AList = Vector<T>;

	struct CallBackInfo
	{
	public:
		CallBackKeyType Key = -1;
		OnDataCallBackType CallBack;
	};
	struct CallBacksType
	{
	public:
		typedef UCode::Unordered_map<CallBackKeyType, OnDataCallBackType>  CallList;



		CallBacksType()
		{

		}


		inline void SetCalls(CallList Calls) { _CallBacks = Calls; }
		inline void SetCalls(const CallBackInfo Values[], size_t Size)
		{
			_CallBacks.clear();

			for (size_t i = 0; i < Size; i++)
			{
				const auto& Item = Values[i];


				_CallBacks[Item.Key] = Item.CallBack;//Add Value.
			}
		}

		inline void Call(CallBackKeyType key, ClientSocket& fromClient, const Byte* b, size_t L)
		{
			bool key_exists = _CallBacks.count(key);
			if (key_exists)
			{

				_CallBacks[key](fromClient,b, L);
			}
		}
	private:
		CallList _CallBacks;
	};
	

	WinSeverSocket(size_t buffersize = 4096);
	~WinSeverSocket();

	inline void Set_MaxPlayers(PlayerIDType MaxPlayers) {
		_MaxPlayers = MaxPlayers;
	};
	inline void Set_OnConnect(OnConnectType  Value) { _OnConnect = Value; }
	inline void Set_OnDisConnect(OnDisConnectType Value) { _OnDisconnect = Value; }
	inline void Set_CallBacks(CallBacksType Value) { _CallBacks = Value; }
	void UpdateCalls();
	void MakeSever(String IP, SInt32 port);
private:
	bool _IsRuning;
	const size_t _buffersize;
	PlayerIDType _MaxPlayers;
	AList<ClientSocket> _Clients;

	OnConnectType _OnConnect;
	OnDisConnectType _OnDisconnect;
	CallBacksType _CallBacks;
};
CoreEnd
