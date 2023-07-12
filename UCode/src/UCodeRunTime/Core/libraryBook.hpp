#pragma once

#include "CoreNamespace.hpp"
CoreStart
class Gamelibrary;

class libraryBook 
{
	friend Gamelibrary;
public:
	libraryBook(Gamelibrary* lib);
	virtual ~libraryBook() {};


	

	inline Gamelibrary* Getlibrary() { return _library; }
	inline const Gamelibrary* Getlibrary() const { return _library; }

	inline static void Destroy(libraryBook* compoent) { compoent->_IsDestroyed = true; }
	inline bool Get_IsDestroyed() const { return _IsDestroyed; }
private:
	Gamelibrary* _library;
	bool _IsDestroyed;

	virtual void Update() {};
};
CoreEnd

