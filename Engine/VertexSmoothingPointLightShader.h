#pragma once


#include "Vec4.h"
#include "Mat.h"
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
		Output(const Vec4& pos)
			:
			pos(pos)
		{}
		Output(const Vec4& src_pos, const Vec3& src_norm, const Vec3& src_color,const Vec3& src_worldPos)
			:
			color(src_color),
			pos(src_pos),
			n(src_norm),
			worldPos(src_worldPos)
		{}
		Output(const Vec4& src_pos, const Vec3& src_norm, const Vec3& src_worldPos)
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
		Vec4 pos;
		Vec4 n;
		Vec3 color;
		Vec3 worldPos;
	};
public:

	void BindWorld(const Mat4& transformation_in)
	{
		world = transformation_in;
		worldProj = world * proj;
	}
	void BindProjection(const Mat4& transformation_in)
	{
		proj = transformation_in;
		worldProj = world * proj;
	}
	const Mat4& GetProj() const
	{
		return proj;
	}
	Output operator()(const Vertex& v) const
	{
		const auto p4 = Vec4(v.pos);
		const auto newpos = p4 * worldProj;
		return { newpos,Vec4{ v.n,0.0f } *world,p4 * world };
	}


private:
	Mat4 world = Mat4::Identity();
	Mat4 proj = Mat4::Identity();
	Mat4 worldProj = Mat4::Identity();


public:
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
	void SetLightPosition(const Vec3& lp)
	{
		light_pos = lp;
	}

private:
	Mat4 transforamtion;
	Vec3 dir = { 1.0f,0.0f,0.0f };
	Vec3 diffuse = { 1.0f, 1.0f , 1.0f };
	Vec3 ambient = { 0.1f,0.1f,0.1f };
	Vec3 color = { 0.8f,0.85f,1.0f };
	Vec3 light_pos = {-1.0f,0.0f,-2.0f};
	float linear_attenuation = 1.0f;
	float quadradic_attenuation = 2.6f;
	float constant_attenuation = 0.3f;

};