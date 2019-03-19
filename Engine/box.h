#pragma once

#include <vector>
#include "Vec3.h"
#include "VertexIndexList.h"
#include "JPG2Vector.h"
#include <string>

class box
{
public:
	box(const float & _length):
		length(_length)
	{
		verticesXY.emplace_back(-length / 2,-length / 2,length / 2);
		verticesXY.emplace_back(length / 2, -length / 2, length / 2);
		verticesXY.emplace_back(length / 2, -length / 2, -length / 2);
		verticesXY.emplace_back(-length / 2, -length / 2, -length / 2);
		verticesXY.emplace_back(length / 2, length / 2, length / 2);
		verticesXY.emplace_back(length / 2, length / 2, -length / 2);
		verticesXY.emplace_back(-length / 2, length / 2, -length / 2);
		verticesXY.emplace_back(-length / 2, length / 2, length / 2);
		
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

	IndexedLineList getLineVertexIndexList() const
	{
		return 
		{ verticesXY,{ 0,1,1,2,2,3,3,0,0,7,1,4,2,5,3,6,7,4,4,5,5,6,6,7 }};
	}

	IndexedTriangleList getTriangleVertexIndexList() const
	{
		return 
		{ verticesXY,{ 0,1,4,0,4,7,
			1,2,4,2,5,4,
			7,4,6,4,5,6,
			0,7,6,0,6,3,
			3,6,5,3,5,2,
			0,3,2,0,2,1} 
		};
	};

	IndexedTriangleList getTexTriangleVertexIndexList() const
	{
		return { verticesUV,{ 0,1,4,0,4,7,
			1,2,4,2,5,4,
			7,4,6,4,5,6,
			0,7,6,0,6,3,
			3,6,5,3,5,2,
			0,3,2,0,2,1}
		};
	}

private:
	float length;
	std::vector<Vec3> verticesXY;
	std::vector<Vec3> verticesUV;
};
