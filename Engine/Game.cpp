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
	
	float dt = 1.0f / 20.0f;
	if (wnd.kbd.KeyIsPressed('W'))
	{
		cam_pos += Vec4{ 0.0f,0.0f,1.0f,0.0f } *!cam_rot_inv * cam_speed * dt;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		cam_pos += Vec4{ -1.0f,0.0f,0.0f,0.0f } *!cam_rot_inv * cam_speed * dt;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		cam_pos += Vec4{ 0.0f,0.0f,-1.0f,0.0f } *!cam_rot_inv * cam_speed * dt;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		cam_pos += Vec4{ 1.0f,0.0f,0.0f,0.0f } *!cam_rot_inv * cam_speed * dt;
	}
	if (wnd.kbd.KeyIsPressed('C'))
	{
		cam_pos += Vec4{ 0.0f,1.0f,0.0f,0.0f } *!cam_rot_inv * cam_speed * dt;
	}
	if (wnd.kbd.KeyIsPressed('Z'))
	{
		cam_pos += Vec4{ 0.0f,-1.0f,0.0f,0.0f } *!cam_rot_inv * cam_speed * dt;
	}
	if (wnd.kbd.KeyIsPressed('Q'))
	{
		cam_rot_inv = cam_rot_inv * Mat4::RotationZ(cam_roll_speed * dt);
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		cam_rot_inv = cam_rot_inv * Mat4::RotationZ(-cam_roll_speed * dt);
	}

	while (! wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		switch (e.GetType())
		{
		case Mouse::Event::Type::LPress:
			mt.Engage(e.GetPos());
			break;
		case Mouse::Event::Type::LRelease:
			mt.Release();
			break;
		case Mouse::Event::Type::Move:
			if (mt.Engaged())
			{
				const auto delta = mt.Move(e.GetPos());
				cam_rot_inv = cam_rot_inv
					* Mat4::RotationY((float)-delta.x * htrack)
					* Mat4::RotationX((float)-delta.y * vtrack);
			}
			break;
		}
	}

}

void Game::ComposeFrame()
{
	pl.BeginFrame();
	const auto proj = Mat4::ProjectionHFOV(hfov, aspect_ratio, 0.5f, 4.0f);
	const auto view = Mat4::Translation(-cam_pos) * cam_rot_inv;
	// set up the rot matrix
	pl.effect.vs.BindWorld(
		Mat4::RotationX(theta_x) *
		Mat4::RotationY(theta_y) *
		Mat4::RotationZ(theta_z) *
		Mat4::Translation(0.0f, 0.0f, offset_z)
	);
	pl.effect.vs.BindView(view);
	pl.effect.vs.BindProjection(proj);
	pl.effect.ps.SetLightPosition({ lx,ly,lz });
	
	pl.Draw(Rabbit);
}