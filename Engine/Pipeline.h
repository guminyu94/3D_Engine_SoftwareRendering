#pragma once

#include "ChiliWin.h"
#include "Graphics.h"
#include "Triangle.h"
#include "VertexIndexList.h"
#include "ScreenTransform.h"
#include "Mat3.h"
#include <algorithm>
#include "Vertex.h"


// fixed-function triangle drawing pipeline
// draws textured triangle lists with clamping
class Pipeline
{

public:
	Pipeline(Graphics& gfx)
		:
		gfx(gfx)
	{}
	void Draw(IndexedTriangleList<Vertex>& triList)
	{
		tex_img_ptr = &triList.tex_img;
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
	void BindTexture(const std::wstring& filename)
	{}
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
	void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// assemble triangles in the stream and process
		for (size_t i = 0, end = indices.size() / 3;
			i < end; i++)
		{
			// determine triangle vertices via indexing
			const auto& v0 = vertices[indices[i * 3]];
			const auto& v1 = vertices[indices[i * 3 + 1]];
			const auto& v2 = vertices[indices[i * 3 + 2]];
			// cull backfacing triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * v0.pos <= 0.0f)
			{
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
		pst.Transform(triangle.v0.pos);
		pst.Transform(triangle.v1.pos);
		pst.Transform(triangle.v2.pos);

		// draw the triangle
		DrawTriangle(triangle);
	}
	// === triangle rasterization functions ===
	void DrawTriangle(const Triangle<Vertex>& triangle)
	{
		DrawTexTriangle(triangle.v0.pos, triangle.v1.pos, triangle.v2.pos, triangle.v0.t, triangle.v1.t, triangle.v2.t);
	}
	void DrawTexTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3);
	void DrawTexUpperTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3);
	void DrawTexLowerTriangle(const Vec3 & v1, const Vec3 & v2, const Vec3 & v3, const Vec3 & tv1, const Vec3 & tv2, const Vec3 & tv3);

private:
	Graphics& gfx;
	ScreenTransformer pst;
	Mat3 rotation;
	Vec3 translation;
	JPG2Vector * tex_img_ptr;
};