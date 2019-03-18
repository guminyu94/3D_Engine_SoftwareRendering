/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Mat3.h"
#include <windows.h>
#include <string>
#include "JPG2Vector.h"


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	box_1(1.0f),
	box_tex("aa322ca2a52f3192b09a650e8eb4c8e0_2.jpg")
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 1.0f / 60.0f;
	if (wnd.kbd.KeyIsPressed('Q'))
	{
		theta_x += dTheta * dt/2;
	}
	if (wnd.kbd.KeyIsPressed('W'))
	{
		theta_y += dTheta * dt/2;
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		theta_z += dTheta * dt/2;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		theta_x -= dTheta * dt/2;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		theta_y -= dTheta * dt/2;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		theta_z -= dTheta * dt/2;
	}
	if (wnd.kbd.KeyIsPressed('F'))
	{
		offset_z +=  dt;
	}
	if (wnd.kbd.KeyIsPressed('G'))
	{
		offset_z -= dt;
	}
}

void Game::ComposeFrame()
{
	
	// set up the rot matrix
	auto rotxMat = _Mat3<float>::RotationX(theta_x);
	auto rotyMat = _Mat3<float>::RotationY(theta_y);
	auto rotzMat = _Mat3<float>::RotationZ(theta_z);
	auto rotMat = rotxMat * rotyMat * rotzMat;
	

	/*
	// get the line and vertices list
	IndexedLineList box_1_vl_list = box_1.getLineVertexIndexList();
	// rot the vertices, z offset, and screen transform
	for (auto & vec : box_1_vl_list.vertex)
	{
		 vec *= rotMat;
		 vec += {0.0f, 0.0f, offset_z};
		 s_trans.Transform(vec);
	}


	// draw the lines
	for (int i = 0; i < (std::end(box_1_vl_list.nodesList) - std::begin(box_1_vl_list.nodesList)) - 1; i = i + 2)
	{
		// debugger output
		std::wstring w_string_debug;
		std::string string_debug = "Line Index: " + std::to_string(i/2) + " \n" + "\0";
		w_string_debug.assign(string_debug.begin(), string_debug.end());
		OutputDebugString(w_string_debug.c_str());
		// draw the lines
		gfx.DrawLine(box_1_vl_list.vertex[box_1_vl_list.nodesList[i]], box_1_vl_list.vertex[box_1_vl_list.nodesList[i + 1]], Colors::White);
	}
	*/

	/*
	IndexedTriangleList box_1_vt_list = box_1.getTriangleVertexIndexList();
	for (auto & vec : box_1_vt_list.verticesXY)
	{
		vec *= rotMat;
		vec += {0.0f, 0.0f, offset_z};
	}
	box_1_vt_list.checkCullFace();
	for (auto & vec : box_1_vt_list.verticesXY)
	{
		s_trans.Transform(vec);
	}
	

	for (int i = 0; i < (std::end(box_1_vt_list.nodesList) - std::begin(box_1_vt_list.nodesList)) - 2; i = i + 3)
	{
		// debugger output
		//std::wstring w_string_debug;
		//std::string string_debug = "Line Index(Triangle): " + std::to_string(i / 3) + " \n" + "\0";
		//w_string_debug.assign(string_debug.begin(), string_debug.end());
		//OutputDebugString(w_string_debug.c_str());
		// draw the triangle
		if (box_1_vt_list.cull[i / 3])
		{
			gfx.DrawTriangle(box_1_vt_list.verticesXY[box_1_vt_list.nodesList[i]], box_1_vt_list.verticesXY[box_1_vt_list.nodesList[i + 1]], box_1_vt_list.verticesXY[box_1_vt_list.nodesList[i + 2]], colors_array[i / 3]);
		}
	}
	*/

	/*
	Vec3 v1 = {0,0,0};
	Vec3 v2 = { 0,1,0 };
	Vec3 v3 = { 1,0,0 };
	v1 *= rotMat;
	v2 *= rotMat;
	v3 *= rotMat;
	v1 += {0.0f, 0.0f, offset_z};
	v2 += {0.0f, 0.0f, offset_z};
	v3 += {0.0f, 0.0f, offset_z};
	s_trans.Transform(v1);
	s_trans.Transform(v2);
	s_trans.Transform(v3);
	Vec3 tv1 = { 20, 499, 0 };
	Vec3 tv2 = { 20, 269, 0 };
	Vec3 tv3 = { 250, 499, 0 };
	auto c1 = box_tex.getPix(363, 398);
	auto c2 = box_tex.getPix(266, 138);
	auto c3 = Colors::White;
	gfx.DrawTexTriangle(v1, v2, v3, tv1, tv2, tv3, box_tex);
	*/

	
	// get the vec list and text list
	IndexedTriangleList box_1_vt_list = box_1.getTriangleVertexIndexList();
	IndexedTriangleList box_1_tex_vt_list = box_1.getTexTriangleVertexIndexList();
	// rot the vec
	for (auto & vec : box_1_vt_list.verticesXY)
	{
		vec *= rotMat;
		vec += {0.0f, 0.0f, offset_z};
	}
	// check the face
	box_1_vt_list.checkCullFace();
	// transfer model to sceen space
	for (auto & vec : box_1_vt_list.verticesXY)
	{
		s_trans.Transform(vec);
	}

	// draw the tex
	for (int i = 0; i < (std::end(box_1_vt_list.nodesList) - std::begin(box_1_vt_list.nodesList)) - 2; i = i + 3)
	{
		if ( true)
		{
			if (box_1_vt_list.cull[i / 3])
			{
				gfx.DrawTexTriangle(box_1_vt_list.verticesXY[box_1_vt_list.nodesList[i]], box_1_vt_list.verticesXY[box_1_vt_list.nodesList[i + 1]], box_1_vt_list.verticesXY[box_1_vt_list.nodesList[i + 2]],
					box_1_tex_vt_list.verticesXY[i], box_1_tex_vt_list.verticesXY[i + 1], box_1_tex_vt_list.verticesXY[i + 2], box_tex);
			}
		}
		

	}
	

}