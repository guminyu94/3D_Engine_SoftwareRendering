#pragma once
#include "Vertex.h"
#include "Colors.h"
#include <string>
#include "JPG2Vector.h"
#include "Vertex.h"
#include "DefaultGeometryShader.h"
#include "VertexShader.h"

class EffectLocalLight
{
public:
	// typedef DefaultVertexShader<Vertex> VertexShader;
	typedef VertexShader VertexShader;
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
public:
	class PixelShader
	{

	public:
		template<class Input>
		Color operator()(const Input& in) const
		{
			// re-normalize interpolated surface normal
			const auto surf_norm = in.n.GetNormalized();
			// vertex to light data
			const auto v_to_l = light_pos - in.worldView;
			const auto dist = v_to_l.Len();
			const auto dir = v_to_l / dist;
			// calculate attenuation
			const auto attenuation = 1.0f /
				(constant_attenuation + linear_attenuation * dist + quadradic_attenuation * sq(dist));
			// calculate intensity based on angle of incidence and attenuation
			const auto d = light_diffuse * attenuation * std::max(0.0f, surf_norm * dir);
			// reflected light vector
			const auto w = surf_norm * (v_to_l * surf_norm);
			const auto r = w * 2.0f - v_to_l;
			// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
			const auto s = light_diffuse * specular_intensity * std::pow(std::max(0.0f, -r.GetNormalized() * in.worldView.GetNormalized()), specular_power);
			if (textureOrMaterial == false)
			{
				auto pc = Vec3(tex_img_ptr->getPix((int)(in.t.x), (int)(in.t.y)))/255.0f;
				//return Color(pc);
				return Color(pc.GetHadamard(d + light_ambient ).Saturate() * 255.0f);
			}

			else
			{
				// add diffuse+ambient, filter by material color, saturate and scale
				return Color(material_color.GetHadamard(d + light_ambient + s).Saturate() * 255.0f);
			}

		}
		void SetDiffuseLight(const Vec3& c)
		{
			light_diffuse = c;
		}
		void SetAmbientLight(const Vec3& c)
		{
			light_ambient = c;
		}
		void SetLightPosition(const Vec4& pos_in)
		{
			light_pos = pos_in;
		}
		void loadTex(JPG2Vector *  _tex_img_ptr)
		{
			tex_img_ptr = _tex_img_ptr;
		}
		void SetMaterial(Vec3 c)
		{
			material_color = c;
		}
	private:
		Vec3 light_pos = { 0.0f,0.0f,0.5f };
		Vec3 light_diffuse = { 1.0f,1.0f,1.0f };
		Vec3 light_ambient = { 0.1f,0.1f,0.1f };
		Vec3 material_color = { 0.8f,0.85f,1.0f };
		// diffuse
		float linear_attenuation = 0.2f;
		float quadradic_attenuation = 0.619f;
		float constant_attenuation = 0.082f;
		// specular
		float specular_power = 40.0f;
		float specular_intensity = 1.5f;
	public:
		JPG2Vector * tex_img_ptr;
		bool textureOrMaterial = false;
	};
public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;

};

