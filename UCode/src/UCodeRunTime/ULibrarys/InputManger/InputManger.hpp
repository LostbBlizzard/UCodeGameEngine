#pragma once
#include <UCodeRunTime/includeULibrayCompoent.hpp>
#include "InputNamespace.hpp"
#include <UCodeRunTime/BasicTypes.hpp>

#include "UCodeLang/LangCore/LangDef.hpp"
#include "bitset"
InputStart

UCodeLangExportSymbol("UCodeGameEngine") enum class InputKey : Byte
{
	Null,

	//Mouse
	LeftMouseButtion,
	RightMouseButtion,
	MiddleMouseButtion,
	MouseScrollUp,
	MouseScrollDown,

	//Other
	Space,Tab,Escape,Enter,BackSpace,Delete,
	ForwardSlash,BackSlash,Dot,Comma,
	//Other
	RightArrow,LeftArrow,DownArrow,UpArrow,
	//Numbers
	Key_0,Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, Key_7, Key_8,Key_9,
	//Leters
	A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
	
	MaxSize,
};
enum class CallBackType : Byte
{
	Press,
	Release,
	Repeat,
};
using InputCallBackKey = Byte;
constexpr InputCallBackKey NullCallBackKey = 0;
//Should be a System.
class InputManger final :private Compoent
{
private:
	InputManger(Entity* p);
	~InputManger();
public:
	
	using InputCallBack = Delegate<void>;

	
	

	static InputManger* GetInput(GameRunTime* e);
	static InputManger* FindInput(const GameRunTime* e);
	static InputManger* Current();
	 
	inline bool GetInputKey(InputKey input) const
	{
		return _InputBuff[(size_t)input];
	}
	f32 GetInputKey_Float(InputKey input) const;
	inline Vec2 Get_CursorPosition() { return _CursorPosition; }

	UCodeGameEngineNoDiscard InputCallBackKey SetCallBack(const InputCallBack& callback,InputKey Inputkey, CallBackType type);
	void ReMoveCallBack(InputCallBackKey id);

	void Input_API_CallInputCallBack(InputKey Inputkey,CallBackType type);
	inline void Input_API_SetInput(bool Value, InputKey input)
	{
		_InputBuff[(size_t)input] = Value;
	}
	inline void Input_API_Set_CursorPosition(f32 x, f32 y)
	{
		_CursorPosition = { x,y };
	}
	void Input_API_Set_Scroll(f32 x, f32 y);


private:
	struct CallBackData
	{
	public:
		CallBackData(InputKey Inputkey,CallBackType calltype, InputCallBackKey ID) 
			:_call(), _Inputkey(Inputkey), _calltype(calltype), _ID(ID)
		{
			
		}
		inline void SetUpCall(InputCallBack call)
		{
			_call = call;
		}
		inline void Call()
		{
			_call();
		}
		inline auto Get_Key() const { return _Inputkey; }
		inline auto Get_ID() const { return _ID; }
		inline auto Get_Type() const { return _calltype; }
	private:
		InputCallBack _call;
		InputKey _Inputkey;
		CallBackType _calltype;
		InputCallBackKey _ID;
	};


	Vec2 _CursorPosition;
	Vec2 _ScrollPosition;
	InputCallBackKey _NextCallBackID;
	Vector<CallBackData> _CallBacks;
	std::bitset<(size_t)InputKey::MaxSize> _InputBuff;
	static UComponentData type_Data;
};


UCodeLangExportSymbol("UCodeGameEngine") struct Input
{
	UCodeLangExport static bool IsKeyDown(InputKey Key);
	UCodeLangExport static Vec2 GetMousePosition();
};

InputEnd