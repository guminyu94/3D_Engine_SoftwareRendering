#pragma once

#include <vector>
#include "Vec3.h"
#include "VertexIndexList.h"
#include "Vertex.h"


class box
{
public:
	box(const float & _length) :
		length(_length)
	{
		verticesXY.emplace_back(-length / 2, -length / 2, length / 2);
		verticesXY.emplace_back(length / 2, -length / 2, length / 2);
		verticesXY.emplace_back(length / 2, -length / 2, -length / 2);
		verticesXY.emplace_back(-length / 2, -length / 2, -length / 2);
		verticesXY.emplace_back(length / 2, length / 2, length / 2);
		verticesXY.emplace_back(length / 2, length / 2, -length / 2);
		verticesXY.emplace_back(-length / 2, length / 2, -length / 2);
		verticesXY.emplace_back(-length / 2, length / 2, length / 2);

		//0,1,4,0,4,7
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(20, 499, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(250, 269, 0);
		//1, 2, 4, 2, 5, 4
		verticesUV.emplace_back(20, 499, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(20, 499, 0);
		verticesUV.emplace_back(250, 269, 0);
		verticesUV.emplace_back(20, 269, 0);
		//7,4,6,4,5,6
		verticesUV.emplace_back(20, 499, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(20, 499, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(250, 269, 0);
		//0,7,6,0,6,3
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(250, 269, 0);
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(20, 499, 0);
		//3,6,5,3,5,2
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(250, 269, 0);
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(20, 499, 0);
		//0,3,2,0,2,1
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(20, 499, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(20, 269, 0);
		verticesUV.emplace_back(250, 499, 0);
		verticesUV.emplace_back(250, 269, 0);

	}


	IndexedTriangleList<Vertex>* getIndexedTriangleList() const
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices{ 0,1,4,0,4,7,
			1, 2, 4, 2, 5, 4,
			7,4,6,4,5,6,
			0,7,6,0,6,3,
			3,6,5,3,5,2,
			0,3,2,0,2,1
		};
		for (unsigned int i = 0; i < indices.size(); i++)
		{
			Vertex v_temp(verticesXY[indices[i]], verticesUV[i]);
			vertices.emplace_back(v_temp);
		}
		std::string tex_img = "maptextured.jpg";
		return new IndexedTriangleList<Vertex>{ vertices,indices, tex_img };
	};

private:
	float length;
	std::vector<Vec3> verticesXY;
	std::vector<Vec3> verticesUV;

};
