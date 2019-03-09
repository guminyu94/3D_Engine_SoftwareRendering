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
	std::vector<Vec3> vertex;
	const int nodesList[36];
};