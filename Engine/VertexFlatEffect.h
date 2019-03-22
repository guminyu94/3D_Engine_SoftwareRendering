#pragma once
#include "Vec3.h"
#include "Mat3.h"

class VertexFlatEffect
{
public:
	
private:
	Mat3 rotation;
	Vec3 translation;
	Vec3 dir = { 0.0f,0.0f,1.0f };
	Vec3 diffuse = { 1.0f, 1.0f , 1.0f };
	Vec3 ambient = {0.1f,0.1f,0.1f};
	Vec3 color = { 0.8f,0.85f,1.0f };
};

