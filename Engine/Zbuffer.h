#pragma once
#include <limits>
#include <cassert>
class Zbuffer
{
public:
	Zbuffer(int width, int height)
		:width(width),
		height(height),
		pbuffer(new float[width*height])
	{};

	~Zbuffer()
	{
		delete[] pbuffer;
		pbuffer = nullptr;
	}

	Zbuffer(const Zbuffer&) = delete;

	Zbuffer & operator = (const Zbuffer &) = delete;

	void Clear()
	{
		const int nDepths = width * height;
		for (int i = 0; i < nDepths; i++)
		{
			pbuffer[i] = -1*std::numeric_limits<float>::infinity();
		}
	}

	float & At(int x, int y)
	{
		assert(x >= 0);
		assert(x< width);
		assert(y >= 0);
		assert(y < height);
		return pbuffer[y*width + x];
	}

	bool TestAndSet(int x, int y, float depth)
	{
		float & depthInBuffer = At(x, y);
		if (depth > depthInBuffer)
		{
			depthInBuffer = depth;
			return true;
		}
		return false;
	}

private:
	int width;
	int height;
	float* pbuffer = nullptr;
};

