#pragma once

#include <UCodeRunTime/RunTimeBasicTypes/Delegate.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/unordered_map.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
CoreStart
class WinCilentSocket
{
public:
	typedef unsigned char Byte;
	typedef Vector<Byte> ByteBuffer;
	template<typename R, typename ...pars> using CallBackType = Delegate<R,pars...>;

	typedef CallBackType<void> OnConnectType;
	typedef CallBackType<void> OnDisConnectType;
	typedef CallBackType<void,const ByteBuffer&,size_t> OnDataCallBackType;
	typedef Byte CallBackKeyType;

	
	
	
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

		inline void Call(CallBackKeyType key,const ByteBuffer& b,size_t L)
		{
			bool key_exists = _CallBacks.count(key);
			if (key_exists)
			{
				
				_CallBacks[key](b,L);
			}
		}
	private:
		CallList _CallBacks;
	};
	

	static_assert(sizeof(Byte) == 1, " 'Byte' is not 1 bytes");

	WinCilentSocket(size_t buffersize = 4096);
	~WinCilentSocket();

	void UpdateCalls();

	inline void Set_OnConnect(OnConnectType value) { _OnConnect = value; }
	inline void Set_OnDisconnect(OnDisConnectType value) { _OnDisconnect = value; }
	inline void Set_CallBacks(CallBacksType value) { _CallBacks = value; };
	
	

	void ConnectToSever(std::string IP, SInt32 port);
	void Disconnect();
	void SendData(CallBackKeyType key, const Byte* _Bytes, size_t BytesSize);
private:
	const size_t _buffersize;
	ByteBuffer _OutputBuffer;
	ByteBuffer _InputBuffer;

	OnConnectType _OnConnect;
	OnDisConnectType _OnDisconnect;
	CallBacksType _CallBacks;
};
CoreEnd
