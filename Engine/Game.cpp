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
#pragma once
#include "MainWindow.h"
#include "Game.h"
#include "Mat.h"
#include <windows.h>
#include <string>
#include "VertexIndexList.h"
#include "Sphere.h"


Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	pl(gfx),
	pl_2(gfx),
	box_1(0.1f),
	//sphere(std::move(Sphere::GetPlain<Vertex>(0.1f,12,24))),
	Rabbit(std::move(IndexedTriangleList<Vertex>::LoadNormals("suzanne.obj")))
{
	/*
	Vec3 v1 = { 0,0,0 };
	Vec3 v2 = { 1,0,0 };
	Vec3 v3 = { 0,1,0 };
	Vec3 tv1 = { 20, 499, 0 };
	Vec3 tv2 = { 250, 499, 0 };
	Vec3 tv3 = { 20, 269, 0 };
	
	std::vector<Vertex> tri_vert_list;
	tri_vert_list.push_back({ v1,tv1 });
	tri_vert_list.push_back({ v2,tv2 });
	tri_vert_list.push_back({ v3,tv3 });
	tri_vertexlist_ptr = new IndexedTriangleList<Vertex> (tri_vert_list, {0,1,2}, "aa322ca2a52f3192b09a650e8eb4c8e0_2.jpg");
	*/
	box_vertexlist_ptr_1 = box_1.getIndexedTriangleList();
	//box_vertexlist_ptr_2 = box_2.getIndexedTriangleList();
	box_vertexlist_ptr_1->getFaceNorm();
	//box_vertexlist_ptr_2->getFaceNorm();
	Rabbit.AdjustToTrueCenter();
	//Rabbit.getFaceNorm();
	offset_z = Rabbit.GetRadius()*2;
	lz = Rabbit.GetRadius();
	//theta_z = PI;
	//sphere.getFaceNorm();
	//Rabbit.RevNorm();
};

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 1.0f/4 ;
	if (wnd.kbd.KeyIsPressed('Q'))
	{
		theta_x += 0.2*dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('W'))
	{
		theta_y += 0.2*dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		theta_z += 0.2*dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		theta_x -= 0.2*dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		theta_y -= 0.2*dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		theta_z -= 0.2*dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('F'))
	{
		offset_z +=  0.6*dt;
	}
	if (wnd.kbd.KeyIsPressed('G'))
	{
		offset_z -= 0.6*dt;
	}
	/*if (wnd.kbd.KeyIsPressed('Y'))
	{
		lt += dTheta*dt;
		pl.effect.vs.SetLightDirection(lt);
	}
	if (wnd.kbd.KeyIsPressed('H'))
	{
		lt -= dTheta * dt;
		pl.effect.vs.SetLightDirection(lt);
	}
	if (wnd.kbd.KeyIsPressed('U'))
	{
		alv += dt;
		pl.effect.vs.SetAmbientLight(alv);
	}
	if (wnd.kbd.KeyIsPressed('J'))
	{
		alv -= dt;
		pl.effect.vs.SetAmbientLight(alv);
	}
	if (wnd.kbd.KeyIsPressed('I'))
	{
		dlv += dt;
		pl.effect.vs.SetAmbientLight(dlv);
	}
	if (wnd.kbd.KeyIsPressed('K'))
	{
		dlv -= dt;
		pl.effect.vs.SetAmbientLight(dlv);
	}
	*/
	
	if (wnd.kbd.KeyIsPressed('J'))
	{
		lx += 5*dt;

	}
	if (wnd.kbd.KeyIsPressed('L'))
	{
		lx -= 5*dt;
	}
	if (wnd.kbd.KeyIsPressed('I'))
	{
		ly +=5* dt;
	}
	if (wnd.kbd.KeyIsPressed('K'))
	{
		ly -= 5*dt;
	}
	if (wnd.kbd.KeyIsPressed('U'))
	{
		lz += 2* dt;
	}
	if (wnd.kbd.KeyIsPressed('O'))
	{
		lz -= 2*dt;
	}
	// debugger output
	//std::wstring w_string_debug;
	//std::string string_debug = "Light_Z: " + std::to_string(lz) + " \n" + "\0";
	//w_string_debug.assign(string_debug.begin(), string_debug.end());
	//OutputDebugString(w_string_debug.c_str());

}

void Game::ComposeFrame()
{
	pl.BeginFrame();
	const auto proj = Mat4::ProjectionHFOV(100.0f, 1.33333f, 0.5f, 4.0f);
	Vec4 test({ 0.4,0.1,0.2,1 });
	auto result = test * proj;
	// set up the rot matrix
	
	pl.effect.vs.BindWorld(
		Mat4::RotationX(theta_x) *
		Mat4::RotationY(theta_y) *
		Mat4::RotationZ(theta_z) *
		Mat4::Translation(0.0f, 0.0f, offset_z)
	);
	
	pl.effect.vs.BindProjection(proj);
	pl.effect.ps.SetLightPosition({ lx,ly,lz });
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

	//pl.effect.vs.BindRotation(rotMat);
	//pl.effect.vs.BindTranslation(tran_vec);
	//pl.Draw(*box_vertexlist_ptr_1);
	//pl.Draw(*box_vertexlist_ptr_2);
	//pl.Draw(Rabbit);


	//pl_2.effect.vs.BindTransformation(Mat4::Translation(lx,ly,lz ));

	//pl.Draw(sphere);
	pl.Draw(Rabbit);
	//pl_2.Draw(*box_vertexlist_ptr_1);
}