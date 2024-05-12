#pragma once
#include "UCodeRunTime/BasicTypes.hpp"

CoreStart

struct ImageData
{
	i32 Width = 0;
	i32 Height = 0;
	Vector<Color32> _ColorData;
};

CoreEnd
