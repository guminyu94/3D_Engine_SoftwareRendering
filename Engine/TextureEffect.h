#pragma once
#include "Vertex.h"
#include "Colors.h"
#include <string>
#include "JPG2Vector.h"
#include "DefaultVertexShader.h"
#include "Vertex.h"
#include "DefaultGeometryShader.h"

class Effect
{
public:
	typedef DefaultVertexShader<Vertex> VertexShader;
	typedef DefaultGeometryShader<Vertex> GeometryShader;
public:
	class PixelShader
	{
		
	public:
		
		Color operator()(int x, int y) const
		{
			return tex_img_ptr->getPix(x, y);
		}

		void loadTex(JPG2Vector *  _tex_img_ptr)
		{
			tex_img_ptr = _tex_img_ptr;
		}

	private:
		JPG2Vector * tex_img_ptr;
	};
public:
	PixelShader ps;
	VertexShader dvs;
	GeometryShader gs;
};

