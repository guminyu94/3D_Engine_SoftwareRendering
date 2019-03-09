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


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	box_1(1.0f)
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
		theta_x += dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('W'))
	{
		theta_y += dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		theta_z += dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		theta_x -= dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		theta_y -= dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		theta_z -= dTheta * dt;
	}


}

void Game::ComposeFrame()
{
	float offset_z = 5;
	// set up the rot matrix
	auto rotxMat = _Mat3<float>::RotationX(theta_x);
	auto rotyMat = _Mat3<float>::RotationY(theta_y);
	auto rotzMat = _Mat3<float>::RotationZ(theta_z);
	auto rotMat = rotxMat * rotyMat * rotzMat;
	// get the line and vertices list
	IndexedLineList box_1_vl_list = box_1.getLineVertexIndexList();
	// rot the vertices, z offset, and screen transform
	for (auto & vec : box_1_vl_list.vertex)
	{
		 vec *= rotMat;
		 vec += {0.0f, 0.0f, offset_z};
		 s_trans.Transform(vec);
	}

	// test the draw line
	gfx.DrawLine(100.0f, 0.0f, 110.0f, 0.0f, Colors::Green);

	// get the line and vertices list
	for (int i = 0; i < (std::end(box_1_vl_list.nodesList) - std::begin(box_1_vl_list.nodesList)) - 1; i = i + 2)
	{
		// debug window, print the line's index  
		//std::string string_debug = "First Index: "+ std::to_string(box_1_vl_list.nodesList[i]) + " \n"+ "Second Index: " + std::to_string(box_1_vl_list.nodesList[i+1]) 
		//	+ " \n"+"\0";
		std::wstring w_string_debug;
		std::string string_debug = "Line Index: " + std::to_string(i/2) + " \n" + "\0";
		w_string_debug.assign(string_debug.begin(), string_debug.end());
		OutputDebugString(w_string_debug.c_str());
		// draw the lines
		auto x1 = box_1_vl_list.vertex[box_1_vl_list.nodesList[i]].x;
		auto x2 = box_1_vl_list.vertex[box_1_vl_list.nodesList[i+1]].x;
		gfx.DrawLine(box_1_vl_list.vertex[box_1_vl_list.nodesList[i]], box_1_vl_list.vertex[box_1_vl_list.nodesList[i + 1]], Colors::White);
	}

}