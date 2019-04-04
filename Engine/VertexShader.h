#pragma once


#include "Vec4.h"
#include "Mat.h"
#include "Colors.h"
#include <cassert>
#include "Vertex.h"
#include <algorithm>

class VertexShader
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
		
		Output(const Vec4& src_pos, const Vec3& src_norm, const Vec3& src_worldView)
			:
			pos(src_pos),
			n(src_norm),
			worldView(src_worldView)
		{}

		Output(const Vec4& src_pos, const Vec3& src_norm, const Vec3& src_worldView, const Vec2& src_t)
			:
			pos(src_pos),
			n(src_norm),
			worldView(src_worldView),
			t(src_t)
		{}

		Output(const Vec4& src_pos, const Vec3& src_norm, const Vec3& src_worldView, const Vec2& src_t, const Vec3& src_world)
			:
			pos(src_pos),
			n(src_norm),
			worldView(src_worldView),
			t(src_t),
			world(src_world)
		{}

		// math operators
		Output& operator+=(const Output& rhs)
		{
			pos += rhs.pos;
			n += rhs.n;
			worldView += rhs.worldView;
			t += rhs.t;
			color += rhs.color;
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
			worldView -= rhs.worldView;
			t -= rhs.t;
			color -= rhs.color;
			return *this;
		}
		Output operator-(const Output& rhs) const
		{
			return Output(*this) -= rhs;
		}
		Output& operator*=(float rhs)
		{
			pos *= rhs;
			worldView *= rhs;
			n *= rhs;
			t *= rhs;
			color *= rhs;
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
			worldView /= rhs;
			t /= rhs;
			color /= rhs;
			return *this;
		}
		Output operator/(float rhs) const
		{
			return Output(*this) /= rhs;
		}
	public:
		Vec4 pos;
		Vec3 n;
		Vec3 worldView;
		Vec2 t;
		Vec3 color;
		Vec3 world;
	};
public:

	void BindWorld(const Mat4& transformation_in)
	{
		world = transformation_in;
		worldViewProj = world * view * proj;
	}
	void BindProjection(const Mat4& transformation_in)
	{
		proj = transformation_in;
		worldViewProj = world * view * proj;
	}
	void BindView(const Mat4& transformation_in)
	{
		view = transformation_in;
		worldViewProj = world * view * proj;
	}
	const Mat4& GetProj() const
	{
		return proj;
	}
	Output operator()(const Vertex& v) const
	{
		const auto p4 = Vec4(v.pos);
		const auto newpos = p4 * worldViewProj;
		return { newpos,Vec4{ v.n,0.0f } *world*view,p4 * world*view,v.t,p4*world};
	}


private:
	Mat4 world = Mat4::Identity();
	Mat4 view = Mat4::Identity();
	Mat4 proj = Mat4::Identity();
	Mat4 worldViewProj = Mat4::Identity();
	
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