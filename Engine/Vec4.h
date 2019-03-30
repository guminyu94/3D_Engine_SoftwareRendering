#pragma once

#include <algorithm>
#include "Math.h"
#include "Vec3.h"

template <typename T>
class _Vec4 : public _Vec3<T>
{
public:
	_Vec4() = default;
	_Vec4(T x, T y, T z, T w)
		:
		_Vec3(x, y, z),
		w(w)
	{}
	_Vec4(const _Vec3& v3, float w = 1.0f)
		:
		_Vec3(v3),
		w(w)
	{}
	template <typename T2>
	explicit operator _Vec4<T2>() const
	{
		return{ (T2)x,(T2)y,(T2)z,(T2)w };
	}
	//T		LenSq() const
	//{
	//	return sq( *this );
	//}
	//T		Len() const
	//{
	//	return sqrt( LenSq() );
	//}
	//_Vec3&	Normalize()
	//{
	//	const T length = Len();
	//	x /= length;
	//	y /= length;
	//	z /= length;
	//	return *this;
	//}
	//_Vec3	GetNormalized() const
	//{
	//	_Vec3 norm = *this;
	//	norm.Normalize();
	//	return norm;
	//}
	_Vec4	operator-() const
	{
		return _Vec4(-x, -y, -z, -w);
	}
	_Vec4&	operator=(const _Vec4 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}
	_Vec4&	operator+=(const _Vec4 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	_Vec4&	operator-=(const _Vec4 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	//T		operator*( const _Vec4 &rhs ) const
	//{
	//	return x * rhs.x + y * rhs.y + z * rhs.z;
	//}
	_Vec4	operator+(const _Vec4 &rhs) const
	{
		return _Vec4(*this) += rhs;
	}
	_Vec4	operator-(const _Vec4 &rhs) const
	{
		return _Vec4(*this) -= rhs;
	}
	_Vec4&	operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}
	_Vec4	operator*(const T &rhs) const
	{
		return _Vec4(*this) *= rhs;
	}
	//_Vec4	operator%( const _Vec4& rhs ) const
	//{
	//	return _Vec4(
	//		y * rhs.z - z * rhs.y,
	//		z * rhs.x - x * rhs.z,
	//		x * rhs.y - y * rhs.x );
	//}
	_Vec4&	operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}
	_Vec4	operator/(const T &rhs) const
	{
		return _Vec4(*this) /= rhs;
	}
	bool	operator==(const _Vec4 &rhs) const
	{
		return x == rhs.x && y == rhs.y && rhs.z == z && rhs.w == w;
	}
	bool	operator!=(const _Vec4 &rhs) const
	{
		return !(*this == rhs);
	}
	// clamp to between 0.0 ~ 1.0
	_Vec4&	Saturate()
	{
		x = std::min(1.0f, std::max(0.0f, x));
		y = std::min(1.0f, std::max(0.0f, y));
		z = std::min(1.0f, std::max(0.0f, z));
		w = std::min(1.0f, std::max(0.0f, w));
		return *this;
	}
	// clamp to between 0.0 ~ 1.0
	_Vec4	GetSaturated() const
	{
		_Vec4 temp(*this);
		temp.Saturate();
		return temp;
	}
	// x3 = x1 * x2 etc.
	_Vec4&  Hadamard(const _Vec4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}
	// x3 = x1 * x2 etc.
	_Vec4	GetHadamard(const _Vec4& rhs) const
	{
		_Vec4 temp(*this);
		temp.Hadamard(rhs);
		return temp;
	}
public:
	T w;
};

typedef _Vec4<float> Vec4;
typedef _Vec4<double> Ved4;
typedef _Vec4<int> Vei4;