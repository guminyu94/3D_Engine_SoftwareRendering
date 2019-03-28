#pragma once

#include <vector>
#include "Vec3.h"
#include "VertexIndexList.h"
#include "JPG2Vector.h"
#include <string>
#include "Vertex.h"

class box
{
public:
	box(const float & _length,const std::string & filename,const float & _z_offset):
		length(_length),
		tex_img_filename(filename)
	{
		verticesXY.emplace_back(-length / 2,-length / 2,length / 2+ _z_offset);
		verticesXY.emplace_back(length / 2, -length / 2, length / 2+ _z_offset);
		verticesXY.emplace_back(length / 2, -length / 2, -length / 2+ _z_offset);
		verticesXY.emplace_back(-length / 2, -length / 2, -length / 2+ _z_offset);
		verticesXY.emplace_back(length / 2, length / 2, length / 2+ _z_offset);
		verticesXY.emplace_back(length / 2, length / 2, -length / 2+ _z_offset);
		verticesXY.emplace_back(-length / 2, length / 2, -length / 2+ _z_offset);
		verticesXY.emplace_back(-length / 2, length / 2, length / 2+ _z_offset);
		
		//0,1,4,0,4,7
		verticesUV.emplace_back(20, 500, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(250, 270, 0);
		verticesUV.emplace_back(20, 500, 0);
		verticesUV.emplace_back(250, 270, 0);
		verticesUV.emplace_back(20, 270, 0);

		//1, 2, 4, 2, 5, 4
		verticesUV.emplace_back(20, 500, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(20, 270, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(250, 270, 0);
		verticesUV.emplace_back(20, 270, 0);

		//7,4,6,4,5,6
		verticesUV.emplace_back(20, 500, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(20, 270, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(250, 270, 0);
		verticesUV.emplace_back(20, 270, 0);

		//0,7,6,0,6,3
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(250, 270, 0);
		verticesUV.emplace_back(20, 270, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(20, 270, 0);
		verticesUV.emplace_back(20, 500, 0);

		//3,6,5,3,5,2
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(250, 270, 0);
		verticesUV.emplace_back(20, 270, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(20, 270, 0);
		verticesUV.emplace_back(20, 500, 0);

		//0,3,2,0,2,1
		verticesUV.emplace_back(20, 270, 0);
		verticesUV.emplace_back(20, 500, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(20, 270, 0);
		verticesUV.emplace_back(250, 500, 0);
		verticesUV.emplace_back(250, 270, 0);
	}
	box(const float & _length) :
		length(_length)
	{
		verticesXY.emplace_back(-length / 2, -length / 2, length / 2 );
		verticesXY.emplace_back(length / 2, -length / 2, length / 2 );
		verticesXY.emplace_back(length / 2, -length / 2, -length / 2 );
		verticesXY.emplace_back(-length / 2, -length / 2, -length / 2 );
		verticesXY.emplace_back(length / 2, length / 2, length / 2);
		verticesXY.emplace_back(length / 2, length / 2, -length / 2 );
		verticesXY.emplace_back(-length / 2, length / 2, -length / 2 );
		verticesXY.emplace_back(-length / 2, length / 2, length / 2 );
	}
	std::unique_ptr<IndexedTriangleList<Vertex>> getIndexedTriangleList() const
	{

		std::vector<unsigned int> indices{ 0, 1, 4, 0, 4, 7,
			1, 2, 4, 2, 5, 4,
			7, 4, 6, 4, 5, 6,
			0, 7, 6, 0, 6, 3,
			3, 6, 5, 3, 5, 2,
			0, 3, 2, 0, 2, 1 };

		//std::vector<unsigned int> indices{ 0,1,4,0,4,7 };

		std::vector<Vertex> vertices;

		for (unsigned int i = 0; i < verticesXY.size(); i++)
		{
			Vertex vert_temp = { verticesXY[i] };
			vertices.emplace_back(vert_temp);
		}

		std::unique_ptr<IndexedTriangleList<Vertex>> boxList_ptr (new IndexedTriangleList<Vertex>{ vertices,indices });

		return boxList_ptr;

	};


private:
	float length;
	std::vector<Vec3> verticesXY;
	std::vector<Vec3> verticesUV;
	std::string tex_img_filename;
};
