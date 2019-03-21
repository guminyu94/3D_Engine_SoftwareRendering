#pragma once

#include "ChiliWin.h"
#include "Graphics.h"
#include "Triangle.h"
#include "VertexIndexList.h"
#include "ScreenTransform.h"
#include "Mat3.h"
#include <algorithm>
#include "TextureEffect.h"
#include "Vertex.h"


// fixed-function triangle drawing pipeline
// draws textured triangle lists with clamping

template<class Effect>
class Pipeline
{

public:
	//typedef typename Pipeline<class Effect> Pipeline;
	Pipeline(Graphics& gfx)
		:
		gfx(gfx)
	{}
	void Draw(IndexedTriangleList<Vertex>& triList)
	{
		effect.ps.loadTex(&(triList.tex_img));
		ProcessVertices(triList.vertices, triList.indices);
	}
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
private:
	// vertex processing function
	// transforms vertices and then passes vtx & idx lists to triangle assembler
	void ProcessVertices(const std::vector<class Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		// create vertex vector for vs output
		std::vector<Vertex> verticesOut;

		// transform vertices using matrix + vector
		for (const auto& v : vertices)
		{
			verticesOut.emplace_back(v.pos * rotation + translation, v.t);
		}

		// assemble triangles from stream of indices and vertices
		AssembleTriangles(verticesOut, indices);
	}
	// triangle assembly function
	// assembles indexed vertex stream into triangles and passes them to post process
	// culls (does not send) back facing triangles
	void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		// assemble triangles in the stream and process
		for (unsigned int i = 0, end = indices.size() / 3;
			i < end; i++)
		{

			// determine triangle vertices via indexing
			auto& v0 = vertices[i * 3];
			auto& v1 = vertices[i * 3 + 1];
			auto& v2 = vertices[i * 3 + 2];
			// cull backfacing triangles with cross product (%)
			if ((((v1.pos - v0.pos) % (v2.pos - v0.pos)) * (v0.pos + v1.pos + v2.pos) / 3) < 0.0f)
			{
				int faceIndex = i;
				// debugger output
				std::wstring w_string_debug;
				std::string string_debug = "Face Index(Triangle): " + std::to_string(faceIndex) + " \n" + "\0";
				w_string_debug.assign(string_debug.begin(), string_debug.end());
				OutputDebugString(w_string_debug.c_str());
				// process 3 vertices into a triangle
				ProcessTriangle(v0, v1, v2);
			}
		}
	}
	// triangle processing function
	// takes 3 vertices to generate triangle
	// sends generated triangle to post-processing
	void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		// generate triangle from 3 vertices using gs
		// and send to post-processing
		PostProcessTriangleVertices(Triangle<Vertex>{ v0, v1, v2 });
	}
	// vertex post-processing function
	// perform perspective and viewport transformations
	void PostProcessTriangleVertices(Triangle<Vertex>& triangle)
	{
		// perspective divide and screen transform for all 3 vertices
		pst.Transform(triangle.v0);
		pst.Transform(triangle.v1);
		pst.Transform(triangle.v2);

		// draw the triangle
		DrawTriangle(triangle);
	}
	// === triangle rasterization functions ===
	void DrawTriangle(const Triangle<Vertex>& triangle)
	{
		DrawTexTriangle(triangle.v0.pos, triangle.v1.pos, triangle.v2.pos, triangle.v0.t, triangle.v1.t, triangle.v2.t);
	}

	void DrawTexTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3)
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
			// find the x,z of the intercept point which divides the upper and lower triangle
			float Intercept_x = linearInterpolation(v1_p->y, v1_p->x, v3_p->y, v3_p->x, v2_p->y);
			float Intercept_z = linearInterpolation(v1_p->y, v1_p->z, v3_p->y, v3_p->z, v2_p->y);
			const Vec3 Intercept = { Intercept_x,v2_p->y,Intercept_z };
			const Vec3 * Intercept_p = &Intercept;
			// find the intercept point of tex
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
			assert(Intercept_p->x >= v2_p->x);
			DrawTexLowerTriangle(*v2_p, *Intercept_p, *v3_p, *tv2_p, *texIntercept_p, *tv3_p);
		}

	}

	void DrawTexUpperTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3)
	{
		assert(v1.y <= v2.y && v1.y <= v3.y);
		assert(v2.y == v3.y);
		assert(v2.x <= v3.x);


		if (v2.y != v1.y && v2.x != v3.x)
		{
			float slope_1_x = (v2.x - v1.x) / (v2.y - v1.y);
			float slope_2_x = (v3.x - v1.x) / (v3.y - v1.y);
			float slope_1_z = (v2.z - v1.z) / (v2.y - v1.y);
			float slope_2_z = (v3.z - v1.z) / (v3.y - v1.y);


			float ratio_1_x = (tv2.x - tv1.x) / (v2.y - v1.y);
			float ratio_2_x = (tv3.x - tv1.x) / (v3.y - v1.y);

			float ratio_1_y = (tv2.y - tv1.y) / (v2.y - v1.y);
			float ratio_2_y = (tv3.y - tv1.y) / (v3.y - v1.y);


			float ratio_1_z = (tv2.z - tv1.z) / (v2.y - v1.y);
			float ratio_2_z = (tv3.z - tv1.z) / (v3.y - v1.y);

			for (float i = ceil(v1.y); i <= floor(v2.y); i++)
			{
				float x_1 = slope_1_x * (i - v1.y) + v1.x;
				float x_2 = slope_2_x * (i - v1.y) + v1.x;
				float z_1 = slope_1_z * (i - v1.y) + v1.z;
				float z_2 = slope_2_z * (i - v1.y) + v1.z;


				float tex_point1_x = (i - v1.y) * ratio_1_x + tv1.x;
				float tex_point1_y = (i - v1.y) * ratio_1_y + tv1.y;


				float tex_point2_x = (i - v1.y) * ratio_2_x + tv1.x;
				float tex_point2_y = (i - v1.y) * ratio_2_y + tv1.y;


				float line_ratio = 0.0f;
				float tex_x = 0;
				float tex_y = 0;

				assert(x_2 >= x_1);

				for (float j = ceil(x_1); j <= floor(x_2); j++)
				{


					line_ratio = (j - x_1) / (x_2 - x_1);
					tex_x = (line_ratio*(tex_point2_x - tex_point1_x) + tex_point1_x);
					tex_y = (line_ratio*(tex_point2_y - tex_point1_y) + tex_point1_y);

					float z = 1.0f / (line_ratio*(z_2 - z_1) + z_1);

					tex_x *= z;
					tex_y *= z;

					gfx.PutPixel((int)(j), (int)(i), effect.ps((int)(tex_x), (int)(tex_y)));

				}



			}
		}
	}

	void DrawTexLowerTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3)
	{
		assert(v3.y >= v2.y && v3.y >= v1.y);
		assert(v1.y == v2.y);
		assert(v1.x <= v2.x);
		if (v3.y != v2.y && v1.x != v2.x)
		{

			float ratio_1_y = (tv1.y - tv3.y) / (v1.y - v3.y);
				float ratio_2_y = (tv2.y - tv3.y) / (v2.y - v3.y);

				float ratio_1_x = (tv1.x - tv3.x) / (v1.y - v3.y);
				float ratio_2_x = (tv2.x - tv3.x) / (v2.y - v3.y);

				float ratio_1_z = (tv1.z - tv3.z) / (v1.y - v3.y);
				float ratio_2_z = (tv2.z - tv3.z) / (v2.y - v3.y);

				float slope_1_x = (v1.x - v3.x) / (v1.y - v3.y);
			float slope_2_x = (v2.x - v3.x) / (v2.y - v3.y);
			float slope_1_z = (v1.z - v3.z) / (v1.y - v3.y);
			float slope_2_z = (v2.z - v3.z) / (v2.y - v3.y);

			for (float i = floor(v3.y); i >= ceil(v1.y); i--)
			{
				float x_1 = slope_1_x * (i - v3.y) + v3.x;
				float x_2 = slope_2_x * (i - v3.y) + v3.x;
				float z_1 = slope_1_z * (i - v3.y) + v3.z;
				float z_2 = slope_2_z * (i - v3.y) + v3.z;

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

					line_ratio = (j - x_1) / (x_2 - x_1);
					tex_x = (line_ratio*(tex_point2_x - tex_point1_x) + tex_point1_x);
					tex_y = (line_ratio*(tex_point2_y - tex_point1_y) + tex_point1_y);

					float z = 1.0f / (line_ratio*(z_2 - z_1) + z_1);

					tex_x *= z;
					tex_y *= z;

					gfx.PutPixel((int)(j), (int)(i), effect.ps((int)(tex_x), (int)(tex_y)));



				}



			}
		}

	}

public:
	Effect effect;
private:
	Graphics& gfx;
	ScreenTransformer pst;
	Mat3 rotation;
	Vec3 translation;
	JPG2Vector * tex_img_ptr;
};