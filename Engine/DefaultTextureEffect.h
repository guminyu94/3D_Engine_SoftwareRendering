#pragma once
#include "Vertex.h"
#include "Colors.h"
#include <string>
#include "JPG2Vector.h"
#include "Vertex.h"
#include "DefaultGeometryShader.h"
#include "VertexSmoothingPointLightShader.h"
#include "DefaultVertexShader.h"

class DefaultEffect
{
public:
	typedef DefaultVertexShader<Vertex> VertexShader;
	//typedef VertexSmoothingPointEffect VertexShader;
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
public:
	class PixelShader
	{

	public:

		template<class Input>
		Color operator()(const Input& in) const
		{
			//return tex_img_ptr->getPix(x, y);
			return {255,255,255};
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
	VertexShader vs;
	GeometryShader gs;
};


