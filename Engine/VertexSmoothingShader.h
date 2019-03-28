#pragma once

#include "Vec3.h"
#include "Mat3.h"
#include "Colors.h"
#include <cassert>
#include "Vertex.h"
#include <algorithm>

class VertexSmoothingEffect
{
public:
	class Output
	{
	public:
		Output() = default;
		Output(const Vec3& pos)
			:
			pos(pos)
		{}
		Output(const Vec3& pos, const Output& src)
			:
			color(src.color),
			pos(pos)
		{}
		Output(const Vec3& pos, const Vec3& color)
			:
			color(color),
			pos(pos)
		{}
		Output& operator+=(const Output& rhs)
		{
			pos += rhs.pos;
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
			color /= rhs;
			return *this;
		}
		Output operator/(float rhs) const
		{
			return Output(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec3 color;
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
		// calculate intensity based on angle of incidence
		const auto d = diffuse * std::max(0.0f, -(v.n * rotation) * dir.GetNormalized());
		// add diffuse+ambient, filter by material color, saturate and scale
		const auto c = color.GetHadamard(d + ambient).Saturate() * 255.0f;
		return{ v.pos * rotation + translation,c };
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
	void SetLightDirection(const float& theta)
	{
		dir = { cos(theta),sin(theta),0 };
	}
	void SetMaterialColor(Color c)
	{
		color = Vec3(c);
	}
private:
	Mat3 rotation;
	Vec3 translation;
	Vec3 dir = { 1.0f,0.0f,0.0f };
	Vec3 diffuse = { 0.5f, 0.5f , 0.5f };
	Vec3 ambient = { 0.1f,0.1f,0.1f };
	Vec3 color = { 0.8f,0.85f,1.0f };
};

