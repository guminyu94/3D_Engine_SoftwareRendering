#pragma once

#include <vector>
#include "Vec3.h"

template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned int> indices_in,std::string filename)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in)),
		tex_img(filename)
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	void getFaceNorm()
	{
		for (unsigned int i = 0; i < indices.size()/3; i++)
		{
			auto& v0 = vertices[i * 3];
			auto& v1 = vertices[i * 3 + 1];
			auto& v2 = vertices[i * 3 + 2];
			//cross product
			Vec3 norm = (v1.pos - v0.pos) % (v2.pos - v0.pos);
			norm.Normalize();
			v0.n = norm;
			v1.n = norm;
			v2.n = norm;
				
		}
	}
	std::vector<T> vertices;
	std::vector<unsigned int> indices;
	JPG2Vector tex_img;
};