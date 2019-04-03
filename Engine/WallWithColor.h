#pragma once

#include "Colors.h"
#include "Vec3.h"
#include <Vector>
#include "VertexIndexList.h"
#include "Vertex.h"

class WallWithColor
{
public:
	WallWithColor(const Color& wallColor_in, const Vec3& lt_corner, const Vec3& rt_corner, const Vec3& lb_corner, const Vec3& rb_corner) :
		wallColor(wallColor_in)
	{
		
		std::vector<Vec3> vertices;
		// vertices location
		vertices.emplace_back(lt_corner);
		vertices.emplace_back(rt_corner);
		vertices.emplace_back(lb_corner);
		vertices.emplace_back(rb_corner);
		// trianglar indices
		std::vector<unsigned int> indices{ 2,3,1,2,1,0 };
		// norm
		Vec3 norm = (vertices[3] - vertices[2]) % (vertices[1] - vertices[2]);
		norm.Normalize();
		// set up vertex and put them to the list
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			Vertex v{ vertices[i] };
			v.n = norm;
			list.vertices.emplace_back(v);

		}
		list.indices = indices;
		list.texColor = Vec3(wallColor_in);
	}


	IndexedTriangleList<Vertex> list;
private:
	Color wallColor;
};