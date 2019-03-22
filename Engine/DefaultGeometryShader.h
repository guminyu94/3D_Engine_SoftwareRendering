#pragma once
#include "Triangle.h"

template<class Vertex>
class DefaultGeometryShader
{
public:
	typedef Vertex Output;
public:
	Triangle<Output> operator()(const Vertex & in0, const Vertex & in1, const Vertex& in2, unsigned int tri_index)
	{
		return { in0 ,in1 ,in2 };
	}
};

