#pragma once

#include <vector>
#include "Vec3.h"


struct IndexedLineList
{
	std::vector<Vec3> vertex;
	const int nodesList[24];
};

struct IndexedTriangleList
{
	IndexedTriangleList(std::vector<Vec3> _vertex, std::vector<int> _nodesList)
	{
		verticesXY = std::move(_vertex);
		nodesList = std::move(_nodesList);
		for (int i = 0; i < 12; i++)
		{
			cull.emplace_back(false);
		}
	};

	void checkCullFace()
	{
		assert(verticesXY.size() > 2);
		assert(nodesList.size() % 3 == 0);
		// calculate whether the triangles face the camera
		for (int i = 0; i < 12; i++)
		{
			Vec3 tri_center = (verticesXY[nodesList[3 * i]] + verticesXY[nodesList[3 * i + 1]] + verticesXY[nodesList[3 * i + 2]]) / 3;
			Vec3 tri_norm = (verticesXY[nodesList[3 * i + 2]] - verticesXY[nodesList[3 * i]]) % (verticesXY[nodesList[3 * i + 1]] - verticesXY[nodesList[3 * i]]);
			if ((tri_center * tri_norm) >= 0)
			{
				cull[i]=true;
			}
			else
			{
				cull[i]=false;
			}
		};
	}

	std::vector<Vec3> verticesXY;
	std::vector<int> nodesList;
	std::vector<bool> cull;
	std::vector<Vec3> verticesUV;
};