#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
#include <functional>
CoreStart

template<typename R, typename... Pars>
struct Delegate
{
	enum class State :UInt8
	{
		Null,
		Cfunction,
		Ufunction,
	};
	struct Ufuction//To be Added
	{

	};
	Delegate()
	{
		_State = State::Null;
	}
	Delegate(const std::function<R(Pars...)>& Value)
	{
		_State = State::Cfunction;
		_Cfunc = Value;
	}
	Delegate(const Ufuction& Value)
	{
		_State = State::Ufunction;
		_Ufunc = Value;
	}
	Delegate& operator=(const std::function<R(Pars...)>& Value)
	{
		_State = State::Cfunction;
		_Cfunc = Value;
		return *this;
	}
	Delegate& operator=(const Ufuction& Value)
	{
		_State = State::Ufunction;
		_Ufunc = Value;
		return *this;
	}
	void SetAsNull()
	{
		_State = State::Null;
	}

	R operator()(Pars... pars)
	{
		return Invoke(pars...);
	}
	R Invoke(Pars... pars)
	{
		switch (_State)
		{
		case State::Cfunction:return _Cfunc(pars...);
		case State::Ufunction:
		case State::Null:

		default:return{};
		}
	}
private:
	State _State = State::Null;
	std::function<R(Pars...)> _Cfunc;
	Ufuction _Ufunc;
};

template<typename... Pars>
struct Delegate<void, Pars...>
{
	using R = void;
	enum class State :UInt8
	{
		Null,
		Cfunction,
		Ufunction,
	};
	struct Ufuction//To be Added
	{

	};
	Delegate()
	{
		_State = State::Null;
	}
	Delegate(const std::function<R(Pars...)>& Value)
	{
		_State = State::Cfunction;
		_Cfunc = Value;
	}
	Delegate(const Ufuction& Value)
	{
		_State = State::Ufunction;
		_Ufunc = Value;
	}
	Delegate& operator=(const std::function<R(Pars...)>& Value)
	{
		_State = State::Cfunction;
		_Cfunc = Value;
		return *this;
	}
	Delegate& operator=(const Ufuction& Value)
	{
		_State = State::Ufunction;
		_Ufunc = Value;
		return *this;
	}
	void SetAsNull()
	{
		_State = State::Null;
	}

	R operator()(Pars... pars)
	{
		return Invoke(pars...);
	}
	R Invoke(Pars... pars)
	{
		switch (_State)
		{
		case State::Cfunction:_Cfunc(pars...);
		case State::Ufunction:
		case State::Null:

		default:return;
		}
	}
private:
	State _State = State::Null;
	std::function<R(Pars...)> _Cfunc;
	Ufuction _Ufunc;
};


CoreEnd