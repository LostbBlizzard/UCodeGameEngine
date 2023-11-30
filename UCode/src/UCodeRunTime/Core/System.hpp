#pragma once

#include "CoreNamespace.hpp"
CoreStart
class Gamelibrary;

class System 
{
	friend Gamelibrary;
public:
	System(Gamelibrary* lib) :_library(lib)
	{

	}
	virtual ~System() {};


	

	inline Gamelibrary* Getlibrary() { return _library; }
	inline const Gamelibrary* Getlibrary() const { return _library; }

	inline static void Destroy(System* system) { system->_IsDestroyed = true; }
	inline bool Get_IsDestroyed() const { return _IsDestroyed; }
private:
	Gamelibrary* _library =nullptr;
	bool _IsDestroyed =false;

	virtual void Update() {};
};
CoreEnd

