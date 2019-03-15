#pragma once
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <string>
#include <vector>
#include <jpeglib.h>
#include "Colors.h"

namespace gil = boost::gil;

class JPG2Vector
{
public:

	// constructor loads the image
	JPG2Vector(const std::string &filename)
	{
		gil::jpeg_read_image(filename, img);
	}

	// get the pixel
	Color getPix(int x, int y)
	{
		//gil::rgb8_pixel_t px = *const_view(img).at(x, y);
		//return { (unsigned int)px[0] ,(unsigned int)px[0],(unsigned int)px[2] };
		return { 0,0,0 };
	}

	Color getPix(const Vec3 & vec)
	{
		return getPix((int)vec.x, (int)vec.y);
	}

private:
	gil::rgb8_image_t img;
};





