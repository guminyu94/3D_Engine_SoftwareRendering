// vertex type used for geometry and throughout pipeline
#pragma once
#include "Vec3.h"
#include "Vec2.h"

class Vertex
{
public:
	Vertex() = default;

	Vertex(const Vec3& pos)
		:
		pos(pos)
	{}
	// this enables template functions clone a vertex
	// while changing the pos only
	Vertex(const Vec3& pos, const Vertex& src)
		:
		t(src.t),
		pos(pos)
	{}
	Vertex(const Vec3& pos, const Vec2& t)
		:
		t(t),
		pos(pos)
	{}
	Vertex(const Vec3& pos, const Vec2& t,const Vec3& n)
		:
		t(t),
		pos(pos),
		n(n)
	{}


	Vertex& operator+=(const Vertex& rhs)
	{
		pos += rhs.pos;
		t += rhs.t;
		c += rhs.c;
		n += rhs.n;
		return *this;
	}
	Vertex operator+(const Vertex& rhs) const
	{
		return Vertex(*this) += rhs;
	}
	Vertex& operator-=(const Vertex& rhs)
	{
		pos -= rhs.pos;
		t -= rhs.t;
		c -= rhs.c;
		n -= rhs.n;
		return *this;
	}
	Vertex operator-(const Vertex& rhs) const
	{
		return Vertex(*this) -= rhs;
	}
	Vertex& operator*=(float rhs)
	{
		pos *= rhs;
		t *= rhs;
		c *= rhs;
		n *= rhs;
		return *this;
	}
	Vertex operator*(float rhs) const
	{
		return Vertex(*this) *= rhs;
	}
	Vertex& operator/=(float rhs)
	{
		pos /= rhs;
		t /= rhs;
		c /= rhs;
		n /= rhs;
		return *this;
	}
	Vertex operator/(float rhs) const
	{
		return Vertex(*this) /= rhs;
	}
public:
	Vec3 pos;
	Vec3 n;
	Vec2 t;
	Vec3 c;
};