#include "../Core/CoreNamespace.hpp"
#include <functional>
CoreStart
class Defer
{

public:
  Defer(const std::function<void()>& Func)
	{
		_ToCall = Func;
	}
	Defer(std::function<void()>&& Func)
	{
		_ToCall = Func;
	}
	~Defer()
	{
		_ToCall();
	}

private:
	std::function<void()> _ToCall;
};

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define UCodeGEDefer(Func) UCode::Defer TOKENPASTE2(DeferredCall,__COUNTER__) ={ [&](){Func;} };

CoreEnd
