#pragma once
#include "Triangle.h"

template<class T>
class DefaultGeometryShader
{
public:
	typedef T Output;
public:
	Triangle<T> operator()(const T & in0, const T & in1, const T& in2, unsigned int tri_index)
	{
		return { in0 ,in1 ,in2 };
	}
};

