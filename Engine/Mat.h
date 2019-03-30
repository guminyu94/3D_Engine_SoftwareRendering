
#pragma once

#include "Vec3.h"
#include "Vec4.h"

template <typename T, size_t S>
class _Mat
{
public:
	_Mat& operator=(const _Mat& rhs)
	{
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}
	_Mat& operator*=(T rhs)
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
	_Mat operator*(T rhs) const
	{
		_Mat result = *this;
		return result *= rhs;
	}
	_Mat& operator*=(const _Mat& rhs)
	{
		return *this = *this * rhs;
	}
	_Mat operator*(const _Mat& rhs) const
	{
		_Mat result;
		for (size_t j = 0; j < S; j++)
		{
			for (size_t k = 0; k < S; k++)
			{
				T sum = (T)0.0;
				for (size_t i = 0; i < S; i++)
				{
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}
	constexpr static _Mat Identity()
	{
		if  (S == 3)
		{
			return {
				(T)1.0,(T)0.0,(T)0.0,
				(T)0.0,(T)1.0,(T)0.0,
				(T)0.0,(T)0.0,(T)1.0,
			};
		}
		else if  (S == 4)
		{
			return {
				(T)1.0,(T)0.0,(T)0.0,(T)0.0,
				(T)0.0,(T)1.0,(T)0.0,(T)0.0,
				(T)0.0,(T)0.0,(T)1.0,(T)0.0,
				(T)0.0,(T)0.0,(T)0.0,(T)1.0,
			};
		}

	}
	static _Mat Scaling(T factor)
	{
		if  (S == 3)
		{
			return{
				factor,(T)0.0,(T)0.0,
				(T)0.0,factor,(T)0.0,
				(T)0.0,(T)0.0,factor,
			};
		}
		else if constexpr (S == 4)
		{
			return {
				factor,(T)0.0,(T)0.0,(T)0.0,
				(T)0.0,factor,(T)0.0,(T)0.0,
				(T)0.0,(T)0.0,factor,(T)0.0,
				(T)0.0,(T)0.0,(T)0.0,(T)1.0,
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}

	}
	static _Mat RotationZ(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		if  (S == 3)
		{
			return{
				 cosTheta, sinTheta, (T)0.0,
				-sinTheta, cosTheta, (T)0.0,
				(T)0.0,    (T)0.0,   (T)1.0,
			};
		}
		else if  (S == 4)
		{
			return {
				 cosTheta, sinTheta, (T)0.0,(T)0.0,
				-sinTheta, cosTheta, (T)0.0,(T)0.0,
				(T)0.0,    (T)0.0,   (T)1.0,(T)0.0,
				(T)0.0,	   (T)0.0,   (T)0.0,(T)1.0,
			};
		}
		
	}
	static _Mat RotationY(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		if  (S == 3)
		{
			return{
				 cosTheta, (T)0.0,-sinTheta,
				 (T)0.0,   (T)1.0, (T)0.0,
				 sinTheta, (T)0.0, cosTheta
			};
		}
		else if  (S == 4)
		{
			return {
				cosTheta, (T)0.0, -sinTheta,(T)0.0,
				(T)0.0,   (T)1.0, (T)0.0,   (T)0.0,
				sinTheta, (T)0.0, cosTheta, (T)0.0,
				(T)0.0,   (T)0.0, (T)0.0,   (T)1.0,
			};
		}
		
	}
	static _Mat RotationX(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		if  (S == 3)
		{
			return{
				(T)1.0, (T)0.0,   (T)0.0,
				(T)0.0, cosTheta, sinTheta,
				(T)0.0,-sinTheta, cosTheta,
			};
		}
		else if  (S == 4)
		{
			return {
				(T)1.0, (T)0.0,   (T)0.0,  (T)0.0,
				(T)0.0, cosTheta, sinTheta,(T)0.0,
				(T)0.0,-sinTheta, cosTheta,(T)0.0,
				(T)0.0, (T)0.0,   (T)0.0,  (T)1.0,
			};
		}
		
	}

	template<class V>
	constexpr static _Mat Translation(const V& tl)
	{
		return Translation(tl.x, tl.y, tl.z);
	}
	static _Mat Translation(T x, T y, T z)
	{
		if  (S == 4)
		{
			return {
				(T)1.0,(T)0.0,(T)0.0,(T)0.0,
				(T)0.0,(T)1.0,(T)0.0,(T)0.0,
				(T)0.0,(T)0.0,(T)1.0,(T)0.0,
				x,     y,      z,    (T)1.0,
			};
		}
	
	}
	constexpr static _Mat Projection(T w, T h, T n, T f)
	{
		if  (S == 4)
		{
			return {
				(T)2.0 * n / w,	(T)0.0,			(T)0.0,				(T)0.0,
				(T)0.0,			(T)2.0 * n / h,	(T)0.0,				(T)0.0,
				(T)0.0,			(T)0.0,			f / (f - n),		(T)1.0,
				(T)0.0,			(T)0.0,			-n * f / (f - n),	(T)0.0,
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}
	}
	constexpr static _Mat ProjectionHFOV(T fov, T ar, T n, T f)
	{
		if  (S == 4)
		{
			const auto fov_rad = fov * (T)PI / (T)180.0;
			const auto w = (T)1.0f / std::tan(fov_rad / (T)2.0);
			const auto h = w * ar;
			return {
				w,		(T)0.0,	(T)0.0,				(T)0.0,
				(T)0.0,	h,		(T)0.0,				(T)0.0,
				(T)0.0,	(T)0.0,	f / (f - n),		(T)1.0,
				(T)0.0,	(T)0.0,	-n * f / (f - n),	(T)0.0,
			};
		}

	}
public:
	// [ row ][ col ]
	T elements[S][S];
};

template<typename T>
_Vec3<T>& operator*=(_Vec3<T>& lhs, const _Mat<T, 3>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec3<T> operator*(const _Vec3<T>& lhs, const _Mat<T, 3>& rhs)
{
	return{
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
	};
}

template<typename T>
_Vec4<T>& operator*=(_Vec4<T>& lhs, const _Mat<T, 4>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec4<T> operator*(const _Vec4<T>& lhs, const _Mat<T, 4>& rhs)
{
	return{
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0] + lhs.w * rhs.elements[3][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1] + lhs.w * rhs.elements[3][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2] + lhs.w * rhs.elements[3][2],
		lhs.x * rhs.elements[0][3] + lhs.y * rhs.elements[1][3] + lhs.z * rhs.elements[2][3] + lhs.w * rhs.elements[3][3]
	};
}

typedef _Mat<float, 3> Mat3;
typedef _Mat<double, 3> Mad3;
typedef _Mat<float, 4> Mat4;
typedef _Mat<double, 4> Mad4;