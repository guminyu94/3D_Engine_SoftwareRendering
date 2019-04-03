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
	pl(gfx)
{
	cam_pos = Vec4{ 0.0f,0.5f,5.0f,0.0f };
	cam_rot_inv = Mat4::RotationY(PI);

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

	// camera control
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

	// light control
	if (wnd.kbd.KeyIsPressed('U'))
	{
		ly += dt;
	}
	if (wnd.kbd.KeyIsPressed('J'))
	{
		ly -= dt;
	}
	if (wnd.kbd.KeyIsPressed('H'))
	{
		lx += dt;
	}
	if (wnd.kbd.KeyIsPressed('K'))
	{
		lx -= dt;
	}
	if (wnd.kbd.KeyIsPressed('Y'))
	{
		lz += dt;
	}
	if (wnd.kbd.KeyIsPressed('I'))
	{
		lz -= dt;
	}
	
}

void Game::ComposeFrame()
{
	pl.BeginFrame();
	const auto proj = Mat4::ProjectionHFOV(hfov, aspect_ratio, 0.5f, 20.0f);
	const auto view = Mat4::Translation(-cam_pos) * cam_rot_inv;
	// set up the rot matrix
	pl.effect.vs.BindWorld(
		Mat4::RotationX(theta_x) *
		Mat4::RotationY(theta_y) *
		Mat4::RotationZ(theta_z) *
		Mat4::Translation(0.0f, 0.0f, offset_z)
	);

	// update light
	pl.effect.vs.BindView(view);
	pl.effect.vs.BindProjection(proj);
	Vec4 lp{ lx,ly,lz,0 };
	pl.effect.ps.SetLightPosition(lp * view);

	// draw the obj list
	for (unsigned int i = 0; i < bScene.objList.size(); i++)
	{
		pl.texOrMat = bScene.isTexOrMat[i];
		pl.Draw(bScene.objList[i]);
	}
	
}