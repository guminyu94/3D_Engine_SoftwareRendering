#pragma once


#include "Vec3.h"
#include "Mat3.h"
#include "Colors.h"
#include <cassert>
#include "Vertex.h"
#include <algorithm>

class VertexSmoothingPointEffect
{
public:
	class Output
	{
	public:

		// initilzation
		Output() = default;
		Output(const Vec3& pos)
			:
			pos(pos)
		{}
		Output(const Vec3& src_pos, const Vec3& src_norm, const Vec3& src_color,const Vec3& src_worldPos)
			:
			color(src_color),
			pos(src_pos),
			n(src_norm),
			worldPos(src_worldPos)
		{}
		Output(const Vec3& src_pos, const Vec3& src_norm, const Vec3& src_worldPos)
			:
			pos(src_pos),
			n(src_norm),
			worldPos(src_worldPos)
		{}

		// math operators
		Output& operator+=(const Output& rhs)
		{
			pos += rhs.pos;
			n += rhs.n;
			worldPos += rhs.worldPos;
			return *this;
		}
		Output operator+(const Output& rhs) const
		{
			return Output(*this) += rhs;
		}
		Output& operator-=(const Output& rhs)
		{
			pos -= rhs.pos;
			n -= rhs.n;
			worldPos -= rhs.worldPos;
			return *this;
		}
		Output operator-(const Output& rhs) const
		{
			return Output(*this) -= rhs;
		}
		Output& operator*=(float rhs)
		{
			pos *= rhs;
			worldPos *= rhs;
			n *= rhs;
			return *this;
		}
		Output operator*(float rhs) const
		{
			return Output(*this) *= rhs;
		}
		Output& operator/=(float rhs)
		{
			pos /= rhs;
			n /= rhs;
			worldPos /= rhs;
			return *this;
		}
		Output operator/(float rhs) const
		{
			return Output(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec3 n;
		Vec3 color;
		Vec3 worldPos;
	};
public:
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	Output operator()(const Vertex& v) const
	{
		// transform vertex pos first
		const auto pos = v.pos * rotation + translation;
		
		return{ pos,v.n* rotation,pos};
		// return{ v.pos * rotation + translation,Colors::Blue };
	}
	void SetDiffuseLight(const float& dlv)
	{
		diffuse = { dlv,dlv,dlv };
	}
	void SetAmbientLight(const float& alv)
	{
		ambient = { alv,alv,alv };
	}
	void SetMaterialColor(Color c)
	{
		color = Vec3(c);
	}
	void SetPX(float x)
	{
		light_pos.x = x;
	}
	void SetPY(float y)
	{
		light_pos.y = y;
	}
	void SetPZ(float z)
	{
		light_pos.z = z;
	}
	
private:
	Mat3 rotation;
	Vec3 translation;
	Vec3 dir = { 1.0f,0.0f,0.0f };
	Vec3 diffuse = { 1.0f, 1.0f , 1.0f };
	Vec3 ambient = { 0.1f,0.1f,0.1f };
	Vec3 color = { 0.8f,0.85f,1.0f };
	Vec3 light_pos = {-1.0f,0.0f,-2.0f};
	float linear_attenuation = 1.0f;
	float quadradic_attenuation = 2.6f;
	float constant_attenuation = 0.3f;
};