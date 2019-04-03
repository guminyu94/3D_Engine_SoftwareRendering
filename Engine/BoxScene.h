#pragma once
#include "box.h"
#include "Sphere.h"
#include "ObjFileLoader.h"
#include <vector>
#include "WallWithColor.h"
#include "Colors.h"

class BoxScene
{
public:

	BoxScene() 
	{
		
		// create box
		box box_1(1, "aa322ca2a52f3192b09a650e8eb4c8e0_2.jpg", 2, 0, 0);
		objList.emplace_back(*box_1.getIndexedTriangleList());
		isTexOrMat.emplace_back(false);
		objList.back().getFaceNorm();
		
		// create obj model monkey
		objList.emplace_back(IndexedTriangleList<Vertex>::LoadNormals("suzanne.obj"));
		isTexOrMat.emplace_back(true);
		objList.back().texColor = { 0.8f,0.85f,1.0f };
		objList.back().AdjustToTrueCenter();
		
		/*
		// create wall left
		WallWithColor wall_left(Colors::Red, { -3,4,3 }, {-3,4,-2}, {-3,-2,3}, { -3,-2,-2 });
		objList.emplace_back(wall_left.list);
		isTexOrMat.emplace_back(true);
		//objList.back().RevNorm();

		
		// wall right
		WallWithColor wall_right(Colors::Green, { 3,4,-2 }, {3,4,3}, { 3,-2,-2 }, { 3,-2,3 });
		objList.emplace_back(wall_right.list);
		isTexOrMat.emplace_back(true);
		//objList.back().RevNorm();


		
		//wall ceil
		WallWithColor wall_ceil(Colors::White, { -3,4,3 }, { 3,4,3 }, {-3,4,-2}, { 3,4,-2 });
		objList.emplace_back(wall_ceil.list);
		isTexOrMat.emplace_back(true);
		//objList.back().RevNorm();
		
		

		// wall hehind
		WallWithColor wall_hehind(Colors::White, { -3,4,-2 }, { 3,4,-2 }, { -3,-2,-2 }, { 3,-2,-2 });
		objList.emplace_back(wall_hehind.list);
		isTexOrMat.emplace_back(true);
		
		*/
		// wall floor
		WallWithColor wall_floor(Colors::White, { -3,-1,-2 }, { 3,-1,-2 }, { -3,-1,3 }, { 3,-1,3 });
		objList.emplace_back(wall_floor.list);
		isTexOrMat.emplace_back(true);
		//objList.back().RevNorm();
		
	};


public:
	std::vector<IndexedTriangleList<Vertex>> objList;
	std::vector<bool> isTexOrMat;
};