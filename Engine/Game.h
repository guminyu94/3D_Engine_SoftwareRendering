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
#include "DefaultTextureEffect.h"
#include "MouseTracker.h"
#include "BoxScene.h"

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
	Pipeline<EffectLocalLight> pl;
	static constexpr float dTheta = PI;
	// fov
	static constexpr float aspect_ratio = 1.33333f;
	static constexpr float hfov = 95.0f;
	static constexpr float vfov = hfov / aspect_ratio;
	// model
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float offset_z = 3.0f;
	// light
	float lx = 0.0f;
	float ly = 0.0f;
	float lz = 0.5f;
	// camera
	static constexpr float htrack = to_rad(hfov) / (float)Graphics::ScreenWidth;
	static constexpr float vtrack = to_rad(vfov) / (float)Graphics::ScreenHeight;
	static constexpr float cam_speed = 1.0f;
	static constexpr float cam_roll_speed = PI;
	Vec3 cam_pos = { 0.0f,0.0f,0.0f };
	Mat4 cam_rot_inv = Mat4::Identity();
	MouseTracker mt;
	// scene
	BoxScene bScene;
	/********************************/
};