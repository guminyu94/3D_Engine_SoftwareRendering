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
#include "Zbuffer.h"

// fixed-function triangle drawing pipeline
// draws textured triangle lists with clamping

template<class Effect>
class Pipeline
{

public:

	//typedef typename Pipeline<class Effect> Pipeline;
	typedef typename Effect::VertexShader::Output VSout;
	typedef typename Effect::GeometryShader::Output GSout;

	Pipeline(Graphics& gfx)
		:
		gfx(gfx),
		zb(gfx.ScreenWidth, gfx.ScreenHeight)
	{}

	//reset the z buffer
	void BeginFrame()
	{
		zb.Clear();
		triangle_index = 0u;
	}

	void Draw(IndexedTriangleList<Vertex>& triList)
	{
		effect.ps.loadTex(&(triList.tex_img));
		ProcessVertices(triList.vertices, triList.indices);
	}


private:
	// vertex processing function
	// transforms vertices and then passes vtx & idx lists to triangle assembler
	void ProcessVertices(const std::vector<class Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		// create vertex vector for vs output
		std::vector<VSout> verticesOut(vertices.size());
		std::transform(vertices.begin(), vertices.end(), verticesOut.begin(), effect.vs);

		// assemble triangles from stream of indices and vertices
		AssembleTriangles(verticesOut, indices);
	}

	// triangle assembly function
	// assembles indexed vertex stream into triangles and passes them to post process
	// culls (does not send) back facing triangles
	void AssembleTriangles(const std::vector<VSout>& vertices, const std::vector<unsigned int>& indices)
	{
		// assemble triangles in the stream and process
		for (unsigned int i = 0, end = indices.size() / 3;
			i < end; i++, triangle_index++)
		{

			// determine triangle vertices via indexing
			auto& v0 = vertices[i * 3];
			auto& v1 = vertices[i * 3 + 1];
			auto& v2 = vertices[i * 3 + 2];
			// cull backfacing triangles with cross product (%)
			if ((((v1.pos - v0.pos) % (v2.pos - v0.pos)) * (v0.pos + v1.pos + v2.pos) / 3) < 0.0f)
			{
				unsigned int faceIndex = i;
				// debugger output
				std::wstring w_string_debug;
				std::string string_debug = "Face Index(Triangle): " + std::to_string(faceIndex) + " \n" + "\0";
				w_string_debug.assign(string_debug.begin(), string_debug.end());
				OutputDebugString(w_string_debug.c_str());

				// process 3 vertices into a triangle
				ProcessTriangle(v0, v1, v2, i);
			}
		}
	}

	// triangle processing function
	// takes 3 vertices to generate triangle
	// sends generated triangle to post-processing
	void ProcessTriangle(const VSout& v0, const VSout& v1, const VSout& v2, unsigned int triangle_index)
	{
		// generate triangle from 3 vertices using gs
		// and send to post-processing
		PostProcessTriangleVertices(effect.gs(v0, v1, v2, triangle_index));
	}

	// vertex post-processing function
	// perform perspective and viewport transformations
	void PostProcessTriangleVertices(Triangle<GSout>& triangle)
	{
		// perspective divide and screen transform for all 3 vertices
		pst.Transform(triangle.v0);
		pst.Transform(triangle.v1);
		pst.Transform(triangle.v2);
		// put the light color on
		Light_c = triangle.v0.color;
		// draw the triangle
		DrawTriangle(triangle);
		
	}

	// === triangle rasterization functions ===

	void DrawTriangle(const Triangle<GSout>& triangle)
	{
		
		// get the pointer of vec3, the index start at 1
		auto * v1_p = &triangle.v0;
		auto * v2_p = &triangle.v1;
		auto * v3_p = &triangle.v2;

		// sort the nodes of 3 vectors based on the y
		if (v1_p->pos.y > v2_p->pos.y)
		{
			std::swap(v1_p, v2_p);
		}
		if (v2_p->pos.y > v3_p->pos.y)
		{
			std::swap(v2_p, v3_p);
		}
		if (v1_p->pos.y > v2_p->pos.y)
		{
			std::swap(v1_p, v2_p);
		}

		// if the triangle is a upper case
		if (v2_p->pos.y == v3_p->pos.y)
		{
			// make sure the 2th is on left

			if (v2_p->pos.x > v3_p->pos.x)
			{
				std::swap(v2_p, v3_p);
			}

			DrawTexUpperTriangle(*v1_p, *v2_p, *v3_p);
		}

		// lower case
		else if (v1_p->pos.y == v2_p->pos.y)
		{
			// make sure the 1th is on left

			if (v1_p->pos.x > v2_p->pos.x)
			{
				std::swap(v1_p, v2_p);
			}

			DrawTexLowerTriangle(*v1_p, *v2_p, *v3_p);
		}

		// general case
		else
		{

			//  find the intercept Vertex
			float ratio = (v2_p->pos.y - v1_p->pos.y) / (v3_p->pos.y - v1_p->pos.y);
			const GSout Intercept = (*v3_p - *v1_p) * ratio + (*v1_p);
			const GSout * Intercept_p = &Intercept;

			// make sure the intercept point is on right

			if (v2_p->pos.x > Intercept_p->pos.x)
			{
				std::swap(v2_p, Intercept_p);
			}

			DrawTexUpperTriangle(*v1_p, *v2_p, *Intercept_p);

			assert(Intercept_p->pos.x >= v2_p->pos.x);

			DrawTexLowerTriangle(*v2_p, *Intercept_p, *v3_p);
		}

	}

	void DrawTexUpperTriangle(const GSout & v1, const GSout & v2, const GSout & v3)
	{
		assert(v1.pos.y <= v2.pos.y && v1.pos.y <= v3.pos.y);
		//assert(v2.pos.y == v3.pos.y);
		assert(v2.pos.x <= v3.pos.x);

		
		if (v2.pos.y != v1.pos.y && v2.pos.x != v3.pos.x)
		{

			for (float i = ceil(v1.pos.y); i <= floor(v2.pos.y); i++)
			{

				GSout point1 = (v2 - v1)*((i - v1.pos.y) / (v2.pos.y - v1.pos.y)) + v1;
				GSout point2 = (v3 - v1)*((i - v1.pos.y) / (v3.pos.y - v1.pos.y)) + v1;

				assert(point2.pos.x >= point1.pos.x);

				for (float j = ceil(point1.pos.x); j <= floor(point2.pos.x); j++)
				{

					GSout cur_point = (point2 - point1)*((j - point1.pos.x) / (point2.pos.x - point1.pos.x)) + point1;

					float cur_point_z = 1.0f / (cur_point.pos.z);

					if (zb.TestAndSet((int)(j), (int)(i), cur_point_z))
					{
						//cur_point.t.x *= cur_point_z;
						//cur_point.t.y *= cur_point_z;
						//gfx.PutPixel((int)(j), (int)(i), effect.ps((int)(cur_point.t.x), (int)(cur_point.t.y)));
						gfx.PutPixel((int)(j), (int)(i), (Light_c));

					}

				}

			}
		}
	}

	void DrawTexLowerTriangle(const GSout & v1, const GSout & v2, const GSout & v3)
	{
		
		assert(v3.pos.y >= v2.pos.y && v3.pos.y >= v1.pos.y);
		//assert(v1.pos.y == v2.pos.y);
		assert(v1.pos.x <= v2.pos.x);
		if (v3.pos.y != v2.pos.y && v1.pos.x != v2.pos.x)
		{

			for (float i = floor(v3.pos.y); i >= ceil(v1.pos.y); i--)
			{

				GSout point1 = (v1 - v3)* ((i - v3.pos.y) / (v1.pos.y - v3.pos.y)) + v3;
				GSout point2 = (v2 - v3)*((i - v3.pos.y) / (v2.pos.y - v3.pos.y)) + v3;
				assert(point2.pos.x >= point1.pos.x);


				for (float j = ceil(point1.pos.x); j <= floor(point2.pos.x); j++)
				{

					GSout cur_point = (point2 - point1)*((j - point1.pos.x) / (point2.pos.x - point1.pos.x)) + point1;

					float cur_point_z = 1.0f / cur_point.pos.z;

					if (zb.TestAndSet((int)(j), (int)(i), cur_point_z))
					{
						//cur_point.t.x *= cur_point_z;
						//cur_point.t.y *= cur_point_z;
						//gfx.PutPixel((int)(j), (int)(i), effect.ps((int)(cur_point.t.x), (int)(cur_point.t.y)));
						gfx.PutPixel((int)(j), (int)(i), (Light_c));

					}


				}

			}
		}

	}

public:
	Effect effect;
private:
	Graphics& gfx;
	ScreenTransformer<GSout> pst;
	JPG2Vector * tex_img_ptr;
	Zbuffer zb;
	unsigned int triangle_index;
	Color Light_c ;
};