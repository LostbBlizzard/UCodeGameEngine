#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "ActionClass.hpp"
CoreStart
template<typename R, typename... Pars> class BoxedAction
{
	typedef	delegate<R, Pars> ActionClass;
public:
	BoxedAction(ActionClass Call, Pars... parameters)
		:_Call(Call), _parameters(parameters)
	{};
	inline bool IsVald()
	{
		return _Call.IsVald();
	}
	R Invoke()
	{
		return _Call.Invoke(_parameters);
	}
	inline bool TryInvoke(R& Out)
	{
		bool r = IsVald();
		if (r)
		{
			Out = Invoke(?);
		}
		return r;
	}
private:
	ActionClass _Call;
	...Pars _parameters;
};

template<typename R, typename... Pars> class BoxedAction
{
	typedef	delegate<R, Pars> ActionClass;
public:
	BoxedAction(ActionClass Call, Pars... parameters)
		:_Call(Call), _parameters(parameters)
	{};
	inline bool IsVald()
	{
		return _Call.IsVald();
	}
	R Invoke()
	{
		return _Call.Invoke(_parameters);
	}
	inline bool TryInvoke(R& Out)
	{
		bool r = IsVald();
		if (r)
		{
			Out = Invoke();
		}
		return r;
	}
private:
	ActionClass _Call;
	...Pars _parameters;
};

template<typename... Pars> class BoxedAction<void,Pars...>
{
	typedef R void;
	typedef	delegate<R, Pars> ActionClass;
public:
	BoxedAction(ActionClass Call, Pars... parameters)
		:_Call(Call), _parameters(parameters)
	{};
	inline bool IsVald()
	{
		return _Call.IsVald();
	}
	R Invoke()
	{
		return _Call.Invoke(_parameters);
	}
	inline bool TryInvoke()
	{
		bool r = IsVald();
		if (r)
		{
			Invoke();
		}
		return r;
	}
private:
	ActionClass _Call;
	...Pars _parameters;
};
CoreEnd