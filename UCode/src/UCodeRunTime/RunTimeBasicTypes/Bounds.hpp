#pragma once

#include <UCodeRunTime/Core/CoreNamespace.hpp>

#include "Vectors.hpp"
CoreStart
class Bounds2d
{
public:
	Bounds2d() :center(), extents() {};
	Bounds2d(Vec2 c, Vec2 e) :center(c), extents(e) {};
	inline Vec2 max() { return center + extents; }
	inline Vec2 min() { return center - extents; }
	inline Vec2 size() { return extents * 2; }//https://docs.unity3d.com/ScriptReference/Bounds-size.html

	//
	Vec2 ClosestPoint(Vec2 v);
	bool Contains(Vec2 v);
	void Encapsulate(Vec2 v);
	void Expand(float32 amount);
	bool Intersects(const Bounds2d& other) const;

	

	Vec2 center;
	Vec2 extents;
};
class Bounds3d
{
	Bounds3d() {};
	Bounds3d(Vec3 c, Vec3 e) :center(c), extents(e){};
	inline Vec3 max() { return center + extents; }
	inline Vec3 min() { return center - extents; }
	inline Vec3 size() { return extents * 2; }

	//
	Vec3 ClosestPoint(Vec3 v);
	bool Contains(Vec3 v);
	void Encapsulate(Vec3 v);
	void Expand(float32 amount);
	bool Intersects(const Bounds3d& B);

	Vec3 center;
	Vec3 extents;
};

class Bounds2dInt
{
	Bounds2dInt() {};
	Bounds2dInt(Vec2i c, Vec2i e) :center(c), extents(e) {};
	inline Vec2i max() { return center + extents; }
	inline Vec2i min() { return center - extents; }
	inline Vec2i size() { return extents * 2; }

	//
	Vec2i ClosestPoint(Vec2i v);
	bool Contains(Vec2i v);
	void Encapsulate(Vec2i v);
	void Expand(SInt32 amount);
	bool Intersects(const Bounds2dInt& B);

	Vec2i center;
	Vec2i extents;
};
class Bounds3dInt
{
	Bounds3dInt() {};
	Bounds3dInt(Vec3i c, Vec3i e) :center(c), extents(e) {};
	inline Vec3i max() { return center + extents; }
	inline Vec3i min() { return center - extents; }
	inline Vec3i size() { return extents * 2; }

	//
	Vec3i ClosestPoint(Vec3i v);
	bool Contains(Vec3i v);
	void Encapsulate(Vec3i v);
	void Expand(SInt32 amount);
	bool Intersects(const Bounds3dInt& B);

	Vec3i center;
	Vec3i extents;
};


CoreEnd

