#include "Bounds.hpp"

CoreStart
Vec2 Bounds2d::ClosestPoint(Vec2 v)
{
	return Vec2();
}

bool Bounds2d::Contains(Vec2 v)
{
	
	return false;
}

void Bounds2d::Encapsulate(Vec2 v)
{
}

void Bounds2d::Expand(float32 amount)
{
}
#define valueinRange(value,min,max) ( (value >= min) && (value <= max) )
bool Bounds2d::Intersects(const Bounds2d& other) const
{

	const Vec2 tl1 = {center.X + extents.X/2.0f,center.Y + extents.Y/2.0f };// Top Left 
	const Vec2 br1 = {center.X - extents.X/2.0f,center.Y - extents.Y/2.0f };// Bottom Right 

	const Vec2 tl2 = { other.center.X + other.extents.X/2.0f,other.center.Y + other.extents.Y/2.0f };// Top Left 
	const Vec2 br2 = { other.center.X - other.extents.X/2.0f,other.center.Y - other.extents.Y/2.0f };// Bottom Right 
	
	return (tl1.X > br2.X) && (br1.X < tl2.X) && (tl1.Y > br2.Y) && (br1.Y < tl2.Y);
}

CoreEnd


