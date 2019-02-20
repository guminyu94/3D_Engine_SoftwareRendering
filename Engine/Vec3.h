/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include "Math.h"

template <typename T>
class _Vec3
{
public:
	// empty param
	_Vec3() {}

	// initialize the vector with given value
	_Vec3(T x, T y, T z)
		:
		x(x),
		y(y),
		z(z)
	{}

	// initialize the vector with a existing vector
	_Vec3(const _Vec3& vect)
		:
		_Vec3(vect.x, vect.y, vect.z)
	{}

	/*
	template <typename T2>

	explicit operator _Vec3<T2>() const
	{
		return { (T2)x,(T2)y,(T2)z };
	}
	*/

	// square of the length
	T		LenSq() const
	{
		return sq(*this);
	}

	// length
	T		Len() const
	{
		return sqrt(LenSq());
	}

	// normalize the vector
	_Vec3&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}

	// get a new normalized vector3
	_Vec3	GetNormalized() const
	{
		_Vec3 norm = *this;
		norm.Normalize();
		return norm;
	}

	// return a new - vector3
	_Vec3	operator-() const
	{
		return _Vec3(-x, -y,-z);
	}

	// overwrite = 
	_Vec3&	operator=(const _Vec3 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	// overwrute +=
	_Vec3&	operator+=(const _Vec3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	// overwrite -=
	_Vec3&	operator-=(const _Vec3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	// overlaod the *
	T		operator*(const _Vec3 &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	// overload the + , return a new vector3
	_Vec3	operator+(const _Vec3 &rhs) const
	{
		return _Vec3(*this) += rhs;
	}

	// overload the - , return a new vector3
	_Vec3	operator-(const _Vec3 &rhs) const
	{
		return _Vec3(*this) -= rhs;
	}

	// overload the *=
	_Vec3&	operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	// overload the *
	_Vec3	operator*(const T &rhs) const
	{
		return _Vec3(*this) *= rhs;
	}

	// overload the operator /=
	_Vec3&	operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}


	// overload the operator /, this returns a new vec3
	_Vec3	operator/(const T &rhs) const
	{
		return _Vec3(*this) /= rhs;
	}

	// overload the operator ==
	bool	operator==(const _Vec3 &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	   
	// overwrite !=
	bool	operator!=(const _Vec3 &rhs) const
	{
		return !(*this == rhs);
	}
public:
	T x;
	T y;
	T z;
};

typedef _Vec3<float> Vec2;
typedef _Vec3<double> Ved2;
typedef _Vec3<int> Vei2;