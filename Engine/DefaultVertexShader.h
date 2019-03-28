#pragma once
#include "Mat3.h"
#include "Vec3.h"

template<class T>
class DefaultVertexShader
{
public:
	typedef T Output;
public:
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}

	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}

	Output operator() (const T& in) const
	{
		return { in.pos * rotation + translation };
	}

private:
	Mat3 rotation;
	Vec3 translation;
};

