/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Mat2.h																				  *
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

#include "Vec3.h"

template <typename T>
class _Mat3
{
public:

	_Mat3& operator=(const _Mat3& rhs)
	{
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}

	_Mat3& operator*=(T rhs)
	{
		for (auto& row : elements)
		{
			for (T& e : row)
			{
				e *= rhs;
			}
		}
		return *this;
	}

	// create a new _Mat3 after times a constant
	_Mat3 operator*(T rhs) const
	{
		_Mat3 result = *this;
		return result *= rhs;
	}

	// matrix *
	_Mat3 operator*(const _Mat3& rhs) const
	{
		_Mat3 result;
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				T sum = (T)0.0;
				for (size_t i = 0; i < 3; i++)
				{
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}

	// Identity matrix
	static _Mat3 Identity()
	{
		_Mat3 i = { (T)1.0,(T)0.0,(T)0.0
			,(T)0.0,(T)1.0,(T)0.0,
			(T)0.0,(T)0.0,(T)1.0
		};
		return i;
	}

	// create the rot matrix
	static _Mat3 RotationZ(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return{
			 cosTheta, sinTheta, (T)0.0,
			-sinTheta, cosTheta, (T)0.0,
			(T)0.0,    (T)0.0,   (T)1.0
		};
	}
	static _Mat3 RotationY(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return{
			 cosTheta, (T)0.0,-sinTheta,
			 (T)0.0,   (T)1.0, (T)0.0,
			 sinTheta, (T)0.0, cosTheta
		};
	}
	static _Mat3 RotationX(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return{
			(T)1.0, (T)0.0,   (T)0.0,
			(T)0.0, cosTheta, sinTheta,
			(T)0.0,-sinTheta, cosTheta,
		};
	}

	static _Mat3 Scaling(T factor)
	{
		_Mat3 s = { factor,(T)0.0,(T)0.0,
			(T)0.0,factor,(T)0.0,
			(T)0.0,(T)0.0,factor
		};
		return s;
	}

public:
	// [ row ][ col ]
	T elements[3][3];
};
template<typename T>
_Vec3<T>& operator*=(_Vec3<T>& lhs, const _Mat3<T>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec3<T> operator*(const _Vec3<T>& lhs, const _Mat3<T>& rhs)
{
	return {
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]};
}

typedef _Mat3<float> Mat3;
typedef _Mat3<double> Mad3;