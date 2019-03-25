/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.h																				  *
*   Minyu Gu 2019/3/17
******************************************************************************************/
#pragma once

#include "Graphics.h"
#include "box.h"
#include "ScreenTransform.h"
#include "FrameMath.h"
#include "Pipeline.h"
#include "ObjFileLoader.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	box box_1;
	box box_2;
	Pipeline<Effect> pl;
	std::unique_ptr<IndexedTriangleList<Vertex>> box_vertexlist_ptr_1;
	std::unique_ptr<IndexedTriangleList<Vertex>> box_vertexlist_ptr_2;
	IndexedTriangleList<Vertex> Rabbit;
	static constexpr float dTheta = PI;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float offset_z = -0.5f;
	const Color colors_array[12] = {
		Colors::White,
		Colors::Blue,
		Colors::Cyan,
		Colors::Gray,
		Colors::Green,
		Colors::Magenta,
		Colors::LightGray,
		Colors::Red,
		Colors::Yellow,
		Colors::White,
		Colors::Blue,
		Colors::Cyan
	};
	/********************************/
};