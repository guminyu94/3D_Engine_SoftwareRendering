#pragma once
#include "Pipeline.h"
#include "FrameMath.h"

void Pipeline::DrawTexTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3)
{
	// get the pointer of vec3
	auto * v1_p = &v1;
	auto * v2_p = &v2;
	auto * v3_p = &v3;
	// get the pointer of texvec3
	auto * tv1_p = &tv1;
	auto * tv2_p = &tv2;
	auto * tv3_p = &tv3;
	// sort the nodes of 3 vectors based on the y
	if (v1_p->y > v2_p->y)
	{
		std::swap(v1_p, v2_p);
		std::swap(tv1_p, tv2_p);
	}
	if (v2_p->y > v3_p->y)
	{
		std::swap(v2_p, v3_p);
		std::swap(tv2_p, tv3_p);
	}
	if (v1_p->y > v2_p->y)
	{
		std::swap(v1_p, v2_p);
		std::swap(tv1_p, tv2_p);
	}

	// if the triangle is a upper case
	if (v2_p->y == v3_p->y)
	{
		// make sure the 2th is on left

		if (v2_p->x > v3_p->x)
		{
			std::swap(v2_p, v3_p);
			std::swap(tv2_p, tv3_p);
		}

		DrawTexUpperTriangle(*v1_p, *v2_p, *v3_p, *tv1_p, *tv2_p, *tv3_p);
	}

	// lower case
	else if (v1_p->y == v2_p->y)
	{
		// make sure the 1th is on left

		if (v1_p->x > v2_p->x)
		{
			std::swap(v1_p, v2_p);
			std::swap(tv1_p, tv2_p);
		}

		DrawTexLowerTriangle(*v1_p, *v2_p, *v3_p, *tv1_p, *tv2_p, *tv3_p);
	}

	// general case
	else
	{
		// find the x of the intercept point which divides the upper and lower triangle
		float Intercept_x = linearInterpolation(v1_p->y, v1_p->x, v3_p->y, v3_p->x, v2_p->y);
		const Vec3 Intercept = { Intercept_x,v2_p->y,0 };
		const Vec3 * Intercept_p = &Intercept;
		float ratio = (v2_p->y - v1_p->y) / (v3_p->y - v1_p->y);
		const Vec3 texIntercept = { ratio*(tv3_p->x - tv1_p->x) + tv1_p->x,ratio*(tv3_p->y - tv1_p->y) + tv1_p->y, 0 };
		const Vec3 * texIntercept_p = &texIntercept;
		// make sure the intercept point is on right

		if (v2_p->x > Intercept_p->x)
		{
			std::swap(v2_p, Intercept_p);
			std::swap(tv2_p, texIntercept_p);
		}



		DrawTexUpperTriangle(*v1_p, *v2_p, *Intercept_p, *tv1_p, *tv2_p, *texIntercept_p);
		assert(Intercept_p->x > v2_p->x);
		DrawTexLowerTriangle(*v2_p, *Intercept_p, *v3_p, *tv2_p, *texIntercept_p, *tv3_p);
	}

}

void Pipeline::DrawTexUpperTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3)
{
	assert(v1.y <= v2.y && v1.y <= v3.y);
	assert(v2.y == v3.y);
	assert(v2.x < v3.x);



	float slope_1 = (v2.x - v1.x) / (v2.y - v1.y);
	float slope_2 = (v3.x - v1.x) / (v3.y - v1.y);
	float ratio_1_y = (tv2.y - tv1.y) / (v2.y - v1.y);
	float ratio_2_y = (tv3.y - tv1.y) / (v3.y - v1.y);
	float ratio_1_x = (tv2.x - tv1.x) / (v2.y - v1.y);
	float ratio_2_x = (tv3.x - tv1.x) / (v3.y - v1.y);

	for (float i = ceil(v1.y); i <= floor(v2.y); i++)
	{
		float x_1 = slope_1 * (i - v1.y) + v1.x;
		float x_2 = slope_2 * (i - v1.y) + v1.x;

		float tex_point1_y = (i - v1.y) * ratio_1_y + tv1.y;
		float tex_point1_x = (i - v1.y) * ratio_1_x + tv1.x;
		float tex_point2_y = (i - v1.y) * ratio_2_y + tv1.y;
		float tex_point2_x = (i - v1.y) * ratio_2_x + tv1.x;

		float line_ratio = 0.0f;
		float tex_x = 0;
		float tex_y = 0;

		assert(x_2 >= x_1);
		for (float j = ceil(x_1); j <= floor(x_2); j++)
		{
			if (x_1 != x_2)
			{
				line_ratio = (j - x_1) / (x_2 - x_1);
				tex_x = (line_ratio*(tex_point2_x - tex_point1_x) + tex_point1_x);
				tex_y = (line_ratio*(tex_point2_y - tex_point1_y) + tex_point1_y);
				gfx.PutPixel((int)(j), (int)(i), (*tex_img_ptr).getPix((int)(tex_x), (int)(tex_y)));

			}

			else
			{
				tex_x = (tex_point2_x);
				tex_y = (tex_point2_y);
				gfx.PutPixel((int)(j), (int)(i), (*tex_img_ptr).getPix((int)(tex_x), (int)(tex_y)));
			}
		}

	}


}

void Pipeline::DrawTexLowerTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3)
{
	assert(v3.y >= v2.y && v3.y >= v1.y);
	assert(v1.y == v2.y);
	assert(v1.x < v2.x);
	if (abs(v3.y - v2.y) >= 1)
	{
		float slope_1 = (v1.x - v3.x) / (v1.y - v3.y);
		float slope_2 = (v2.x - v3.x) / (v2.y - v3.y);
		float ratio_1_y = (tv1.y - tv3.y) / (v1.y - v3.y);
		float ratio_2_y = (tv2.y - tv3.y) / (v2.y - v3.y);
		float ratio_1_x = (tv1.x - tv3.x) / (v1.y - v3.y);
		float ratio_2_x = (tv2.x - tv3.x) / (v2.y - v3.y);

		for (float i = floor(v3.y); i >= ceil(v1.y); i--)
		{
			float x_1 = slope_1 * (i - v3.y) + v3.x;
			float x_2 = slope_2 * (i - v3.y) + v3.x;

			float tex_point1_y = (i - v3.y) * ratio_1_y + tv3.y;
			float tex_point1_x = (i - v3.y) * ratio_1_x + tv3.x;
			float tex_point2_y = (i - v3.y) * ratio_2_y + tv3.y;
			float tex_point2_x = (i - v3.y) * ratio_2_x + tv3.x;

			float line_ratio = 0;
			float tex_x = 0;
			float tex_y = 0;

			assert(x_2 >= x_1);
			for (float j = ceil(x_1); j <= floor(x_2); j++)
			{
				if (x_1 != x_2)
				{
					line_ratio = (j - x_1) / (x_2 - x_1);
					tex_x = (line_ratio*(tex_point2_x - tex_point1_x) + tex_point1_x);
					tex_y = (line_ratio*(tex_point2_y - tex_point1_y) + tex_point1_y);
					gfx.PutPixel((int)(j), (int)(i), (*tex_img_ptr).getPix((int)(tex_x), (int)(tex_y)));
				}
				else
				{
					tex_x = (tex_point2_x);
					tex_y = (tex_point2_y);
					gfx.PutPixel((int)(j), (int)(i), (*tex_img_ptr).getPix((int)(tex_x), (int)(tex_y)));
				}

			}

		}
	}

}