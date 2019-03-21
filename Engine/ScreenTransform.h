#pragma once
#include "Vec3.h"
#include "Graphics.h"
#include "Vertex.h"

class ScreenTransformer
{
public:

	ScreenTransformer()
		:
		xFactor(float(Graphics::ScreenWidth) / 2.0f),
		yFactor(float(Graphics::ScreenHeight) / 2.0f)
	{}

	Vertex& Transform(Vertex& v) const
	{
		const float zInv = 1.0f / v.pos.z;
		// to do the perspective correction, first bring all the pos & tex vec to 1/z space
		v *= zInv;
		v.pos.x = (v.pos.x  + 1.0f) * xFactor;
		v.pos.y = (-v.pos.y  + 1.0f) * yFactor;
		// save 1/z in vertex.pos.z for now
		v.pos.z = zInv;

		return v;
	}

	Vertex GetTransformed(const Vertex& v) const
	{
		return Transform(Vertex(v));
	}
private:
	float xFactor;
	float yFactor;
};