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
	ScreenTransformer s_trans;
	Pipeline<Effect> pl;
	IndexedTriangleList<Vertex>* box_vertexlist_ptr;
	static constexpr float dTheta = PI;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float offset_z = -3.0f;
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