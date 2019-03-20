#pragma once

#include <vector>
#include "Vec3.h"

template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList(std::vector<T> verts_in, std::vector<size_t> indices_in,std::string filename)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in)),
		tex_img(filename)
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}
	std::vector<T> vertices;
	std::vector<size_t> indices;
	JPG2Vector tex_img;
};