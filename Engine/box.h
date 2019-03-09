#pragma once

#include <vector>
#include "Vec3.h"
#include "VertexIndexList.h"

class box
{
public:
	box(float _length):
		length(_length)
	{
		vertex.emplace_back(length / 2,-length / 2,-length / 2);
		vertex.emplace_back(length / 2, length / 2, -length / 2);
		vertex.emplace_back(-length / 2, length / 2, -length / 2);
		vertex.emplace_back(-length / 2, -length / 2, -length / 2);
		vertex.emplace_back(length / 2, length / 2, length / 2);
		vertex.emplace_back(-length / 2, length / 2, length / 2);
		vertex.emplace_back(-length / 2, -length / 2, length / 2);
		vertex.emplace_back(length / 2, -length / 2, length / 2);
		
	}

	IndexedLineList getLineVertexIndexList() const
	{
		return {vertex,{ 0,1,1,2,2,3,3,0,0,7,1,4,2,5,3,6,7,4,4,5,5,6,6,7 }};
	}

	IndexedTriangleList getTriangleVertexIndexList() const
	{
		return { vertex,{ 0,1,4,0,4,7,
			1,2,4,2,5,4,
			2,4,5,4,6,5,
			7,6,4,0,7,4,
			4,5,6,4,6,7,
			0,3,1,1,3,2 } 
		};
	}

private:
	float length;
	std::vector<Vec3> vertex;
};
