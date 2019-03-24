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
		img_width = img.width();
		img_height = img.height();

		// place all the pixels' color into a vector
		for (int i = 1; i <= img_height; i++)
		{
			for (int j = 1; j <= img_width; j++)
			{
				if (i == 600)
				{
					int inexp = 1;
				}
				gil::rgb8_pixel_t px = *const_view(img).at(j, i);
				Color pix_color{ (unsigned int)px[0] ,(unsigned int)px[1],(unsigned int)px[2] };
				img_pix.emplace_back(pix_color);
			}
		}

	}
	
	JPG2Vector()
	{};

	// get the pixel
	Color getPix(int x, int y)
	{
		return img_pix[(y-1)*img_width+x-1];
	}

	Color getPix(const Vec3 & vec)
	{
		return getPix((int)vec.x, (int)vec.y);
	}

private:
	gil::rgb8_image_t img;
	int img_width;
	int img_height;
	std::vector<Color> img_pix;
	
};





