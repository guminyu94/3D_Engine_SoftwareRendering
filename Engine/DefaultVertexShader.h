#pragma once
#include "Mat.h"
#include "Vec3.h"

template<class T>
class DefaultVertexShader
{
public:
	class Output
	{
	public:

		// initilzation
		Output() = default;
	
		Output(const Vec4& src_pos)
			:
			pos(src_pos)
		{}

		// math operators
		Output& operator+=(const Output& rhs)
		{
			pos += rhs.pos;
			
			return *this;
		}
		Output operator+(const Output& rhs) const
		{
			return Output(*this) += rhs;
		}
		Output& operator-=(const Output& rhs)
		{
			pos -= rhs.pos;
			
			return *this;
		}
		Output operator-(const Output& rhs) const
		{
			return Output(*this) -= rhs;
		}
		Output& operator*=(float rhs)
		{
			pos *= rhs;
			
			return *this;
		}
		Output operator*(float rhs) const
		{
			return Output(*this) *= rhs;
		}
		Output& operator/=(float rhs)
		{
			pos /= rhs;
			
			return *this;
		}
		Output operator/(float rhs) const
		{
			return Output(*this) /= rhs;
		}
	public:
		Vec4 pos;
	};
public:
	void BindTransformation(const Mat4& trans_in)
	{
		Transformation = trans_in;
	}


	Output operator() (const T& in) const
	{
		auto pt = Vec4(in.pos) * Transformation;
		return { pt };
	}

private:
	Mat4 Transformation;
};

