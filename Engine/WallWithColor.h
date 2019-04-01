#pragma once

#include "Colors.h"
#include "Vec3.h"
#include <Vector>
#include "VertexIndexList.h"
#include "Vertex.h"

class WallWithColor
{
public:
	WallWithColor(Color wallColor_in, Vec3 lt_corner, Vec3 rb_corner) :
		wallColor(wallColor_in)
	{
		// calculate the corner
		Vec3 rt_corner = { rb_corner.x,lt_corner.y,rb_corner.z };
		Vec3 lb_corner = { lt_corner.x,rb_corner.y,lt_corner.z };
		std::vector<Vec3> vertices;
		// vertices location
		vertices.emplace_back(lt_corner);
		vertices.emplace_back(rt_corner);
		vertices.emplace_back(lb_corner);
		vertices.emplace_back(rb_corner);
		// trianglar indices
		std::vector<unsigned int> indices{ 3,4,2,3,2,1 };
		// norm
		Vec3 norm = (vertices[4] - vertices[3]) % (vertices[2] - vertices[3]);
		norm.Normalize();
		// set up vertex and put them to the list
		for (unsigned int i = 0; i < indices.size(); i++)
		{
			Vertex v{ vertices[indices[i]] };
			v.n = norm;
			list.vertices.emplace_back(v);
			list.indices.emplace_back(indices[i]);
		}
		list.texColor = Vec3(wallColor_in);
	}


	IndexedTriangleList<Vertex> list;
private:
	Color wallColor;
};