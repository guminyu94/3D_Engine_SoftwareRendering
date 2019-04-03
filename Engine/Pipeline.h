#pragma once

#include "ChiliWin.h"
#include "Graphics.h"
#include "Triangle.h"
#include "VertexIndexList.h"
#include "NDCScreenTransformer.h"
#include "Mat.h"
#include <algorithm>
#include "TextureEffectPixelShader.h"
#include "Vertex.h"
#include "Zbuffer.h"
#include <algorithm>

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
		if (texOrMat == false)
		{
			effect.ps.textureOrMaterial = false;
			effect.ps.loadTex(&(triList.tex_img));
			ProcessVertices(triList.vertices, triList.indices);
		}
		else
		{
			effect.ps.textureOrMaterial = true;
			effect.ps.SetMaterial(triList.texColor);
			ProcessVertices(triList.vertices, triList.indices);
		}
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
		const auto eyepos = Vec4{ 0.0f,0.0f,0.0f,1.0f } *effect.vs.GetProj();
		// assemble triangles in the stream and process
		for (unsigned int i = 0, end = indices.size() / 3;
			i < end; i++, triangle_index++)
		{
			// determine triangle vertices via indexing
			auto& v0 = vertices[indices[i * 3]];
			auto& v1 = vertices[indices[i * 3+1]];
			auto& v2 = vertices[indices[i * 3+2]];
			// cull backfacing triangles with cross product (%)
			if (((v1.pos - v0.pos) % (v2.pos - v0.pos)) * v0.pos <= 0.0f)
			{
				//unsigned int faceIndex = i;
				// debugger output
				//std::wstring w_string_debug;
				//std::string string_debug = "Face Index(Triangle): " + std::to_string(faceIndex) + " \n" + "\0";
				//w_string_debug.assign(string_debug.begin(), string_debug.end());
				//OutputDebugString(w_string_debug.c_str());

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
		// and send to clipper
		ClipCullTriangle(effect.gs(v0, v1, v2, triangle_index));
	}

	void ClipCullTriangle(Triangle<GSout>& t)
	{
	
		// cull tests
		if (abs(t.v0.pos.x / t.v0.pos.w) > 1.0f  &&
			abs(t.v1.pos.x / t.v1.pos.w) > 1.0f &&
			abs(t.v2.pos.x / t.v2.pos.w) > 1.0f)
		{
			return;
		}
		if (abs(t.v0.pos.y / t.v0.pos.w) > 1.0f  &&
			abs(t.v1.pos.y / t.v1.pos.w) > 1.0f &&
			abs(t.v2.pos.y / t.v2.pos.w) > 1.0f)
		{
			return;
		}
		
		if (abs(t.v0.pos.z / t.v0.pos.w) > 1.0f  &&
			abs(t.v1.pos.z / t.v1.pos.w) > 1.0f &&
			abs(t.v2.pos.z / t.v2.pos.w) > 1.0f)
		{
			return;
		}
		
		if ((t.v0.pos.z) < 0.0f  &&
			(t.v1.pos.z) < 0.0f &&
			(t.v2.pos.z) < 0.0f)
		{
			return;
		}
		
		
		const auto Clip1 = [this](GSout& v0, GSout& v1, GSout& v2)
		{
			// calculate alpha values for getting adjusted vertices
			const float alphaA = (-v0.pos.z) / (v1.pos.z - v0.pos.z);
			const float alphaB = (-v0.pos.z) / (v2.pos.z - v0.pos.z);
			// inteerpolate to get v0a and v0b
			const auto v0a = v0 + (v1 - v0) * alphaA;;
			const auto v0b = v0 + (v2 - v0) * alphaB;;
			// draw triangles
			PostProcessTriangleVertices(Triangle<GSout>{v0a, v1, v2});
			PostProcessTriangleVertices(Triangle<GSout>{v0b, v0a, v2});
		};

		const auto Clip2 = [this](GSout& v0, GSout& v1, GSout& v2)
		{
			// calculate alpha values for getting adjusted vertices
			const float alpha0 = (-v0.pos.z) / (v2.pos.z - v0.pos.z);
			const float alpha1 = (-v1.pos.z) / (v2.pos.z - v1.pos.z);

			// interpolate to get v0a and v0b
			v0 = v0 + (v2 - v0) * alpha0;;
			v1 = v1 + (v2 - v1) * alpha1;;

			// draw triangles
			PostProcessTriangleVertices(Triangle<GSout>{ v0, v1, v2 });
		};

		// near clipping tests
		if (t.v0.pos.z < 0.0f)
		{
			if (t.v1.pos.z < 0.0f)
			{
				Clip2(t.v0, t.v1, t.v2);
			}
			else if (t.v2.pos.z < 0.0f)
			{
				Clip2(t.v0, t.v2, t.v1);
			}
			else
			{
				Clip1(t.v0, t.v1, t.v2);
			}
		}
		else if (t.v1.pos.z < 0.0f)
		{
			if (t.v2.pos.z < 0.0f)
			{
				Clip2(t.v1, t.v2, t.v0);
			}
			else
			{
				Clip1(t.v1, t.v0, t.v2);
			}
		}
		else if (t.v2.pos.z < 0.0f)
		{
			Clip1(t.v2, t.v0, t.v1);
		}
		else // no near clipping necessary
		{
			PostProcessTriangleVertices(t);
		}
		
		
		//PostProcessTriangleVertices(t);
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
		//Light_c = triangle.v0.color;
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
		//assert(v1.pos.y <= v2.pos.y && v1.pos.y <= v3.pos.y);
		//assert(v2.pos.y == v3.pos.y);
		//assert(v2.pos.x <= v3.pos.x);

		
		if (v2.pos.y != v1.pos.y && v2.pos.x != v3.pos.x)
		{
			int start = ceil(v1.pos.y);
			int end = floor(v2.pos.y);
			for (int i = std::max(start,0); i <= std::min(end, (int)(Graphics::ScreenHeight - 1)); i++)
			{

				GSout point1 = (v2 - v1)*((i - v1.pos.y) / (v2.pos.y - v1.pos.y)) + v1;
				GSout point2 = (v3 - v1)*((i - v1.pos.y) / (v3.pos.y - v1.pos.y)) + v1;

				//assert(point2.pos.x >= point1.pos.x);
				int start = ceil(point1.pos.x);
				int end = floor(point2.pos.x);
				for (int j = std::max(0, start); j <= std::min(end, (int)(Graphics::ScreenWidth - 1)); j++)
				{

					GSout cur_point = (point2 - point1)*((j - point1.pos.x) / (point2.pos.x - point1.pos.x)) + point1;

					//float cur_point_z = 1.0f / (cur_point.pos.z);

					if (zb.TestAndSet((int)(j), (int)(i), cur_point.pos.z))
					{
						float cur_point_w = 1.0f / cur_point.pos.w;
						auto point =   cur_point*cur_point_w;
						//cur_point *= cur_point_z;
						//cur_point.t.x *= cur_point_z;
						//cur_point.t.y *= cur_point_z;

					
						gfx.PutPixel((int)(j), (int)(i), effect.ps(point));
		
					}

				}

			}
		}
	}

	void DrawTexLowerTriangle(const GSout & v1, const GSout & v2, const GSout & v3)
	{
		
		//assert(v3.pos.y >= v2.pos.y && v3.pos.y >= v1.pos.y);
		//assert(v1.pos.y == v2.pos.y);
		//assert(v1.pos.x <= v2.pos.x);
		if (v3.pos.y != v2.pos.y && v1.pos.x != v2.pos.x)
		{
			int start = floor(v3.pos.y);
			int end = ceil(v1.pos.y);
			for (int i = std::min((int)(Graphics::ScreenHeight - 1), start); i >= std::max(0, end); i--)
			{

				GSout point1 = (v1 - v3)* ((i - v3.pos.y) / (v1.pos.y - v3.pos.y)) + v3;
				GSout point2 = (v2 - v3)*((i - v3.pos.y) / (v2.pos.y - v3.pos.y)) + v3;
				//assert(point2.pos.x >= point1.pos.x);

				int start = ceil(point1.pos.x);
				int end = floor(point2.pos.x);
				for (int j = std::max(0, start); j <= std::min(end, (int)(Graphics::ScreenWidth - 1)); j++)
				{

					GSout cur_point = (point2 - point1)*((j - point1.pos.x) / (point2.pos.x - point1.pos.x)) + point1;

					//float cur_point_z = 1.0f / cur_point.pos.z;

					if (zb.TestAndSet((int)(j), (int)(i), cur_point.pos.z))
					{
						float cur_point_w = 1.0f / cur_point.pos.w;
						auto point = cur_point * cur_point_w;
						//cur_point *= cur_point_z;
						//cur_point.t.x *= cur_point_z;
						//cur_point.t.y *= cur_point_z;
						//gfx.PutPixel((int)(j), (int)(i), effect.ps((int)(cur_point.t.x), (int)(cur_point.t.y)));
						
						gfx.PutPixel((int)(j), (int)(i), effect.ps(point));
						//gfx.PutPixel((int)(j), (int)(i),Colors::White);

					}


				}

			}
		}

	}

public:
	Effect effect;
	bool texOrMat;
private:
	Graphics& gfx;
	NDCScreenTransformer<GSout> pst;
	JPG2Vector * tex_img_ptr;
	Zbuffer zb;
	unsigned int triangle_index;
};