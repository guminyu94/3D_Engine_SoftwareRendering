#pragma once

#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"
#include "Miniball.h"
#include <fstream>
#include <cctype>

template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList(const std::vector<T>& verts_in, const std::vector<unsigned int> & indices_in,const std::string & filename)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in)),
		tex_img(filename)
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	IndexedTriangleList(const std::vector<T> & verts_in, const std::vector<unsigned int>& indices_in)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	IndexedTriangleList()
	{};

	void getFaceNorm()
	{
		for (unsigned int i = 0; i < indices.size()/3; i++)
		{
			auto& v0 = vertices[indices[i * 3]];
			auto& v1 = vertices[indices[i * 3+1]];
			auto& v2 = vertices[indices[i * 3+2]];
			//cross product
			Vec3 norm = (v1.pos - v0.pos) % (v2.pos - v0.pos);
			norm.Normalize();
			v0.n = norm;
			v1.n = norm;
			v2.n = norm;
				
		}
	}

	void RevNorm()
	{
		for (unsigned int i = 0; i < vertices.size() ; i++)
		{
			vertices[i].n = - ( vertices[i].n);
		}
	}

	static IndexedTriangleList<T> Loader(const std::string& filename)

	{

		IndexedTriangleList<T> t1;

		// check first line of file to see if CCW winding comment exists
		bool isCCW = false;
		{
			std::ifstream file(filename);
			std::string firstline;
			std::getline(file, firstline);
			std::transform(firstline.begin(), firstline.end(), firstline.begin(), std::tolower);
			if (firstline.find("ccw") != std::string::npos)
			{
				isCCW = true;
			}
		}

		// these will be filled by obj loading function
		using namespace tinyobj;
		attrib_t attrib;
		std::vector<shape_t> shapes;
		std::string err;

		// load/parse the obj file
		const bool ret = LoadObj(&attrib, &shapes, nullptr, &err, filename.c_str());

		// check for errors
		if (!err.empty() && err.substr(0, 4) != "WARN")
		{
			throw std::runtime_error(("LoadObj returned error:" + err + " File:" + filename).c_str());
		}
		if (!ret)
		{
			throw std::runtime_error(("LoadObj returned false  File:" + filename).c_str());
		}
		if (shapes.size() == 0u)
		{
			throw std::runtime_error(("LoadObj object file had no shapes  File:" + filename).c_str());
		}

		// extract vertex data
		// attrib.vertices is a flat std::vector of floats corresponding
		// to vertex positions, laid out as xyzxyzxyz... etc.
		// first preallocate required space in OUR std::vector
		t1.vertices.reserve(attrib.vertices.size() / 3u);
		// iterate over individual vertices, construct Vec3s in OUR vector
		for (int i = 0; i < attrib.vertices.size(); i += 3)
		{
			t1.vertices.emplace_back(Vec3{
				attrib.vertices[i + 0],
				attrib.vertices[i + 1],
				attrib.vertices[i + 2]
				});
		}

		// extract index data
		// obj file can contain multiple meshes, we assume just 1
		const auto& mesh = shapes[0].mesh;
		// mesh contains a std::vector of num_face_vertices (uchar)
		// and a flat std::vector of indices. If all faces are triangles
		// then for any face f, the first index of that faces is [f * 3n]
		t1.indices.reserve(mesh.indices.size());
		for (unsigned int f = 0; f < mesh.num_face_vertices.size(); f++)
		{
			// make sure there are no non-triangle faces
			if (mesh.num_face_vertices[f] != 3u)
			{
				std::stringstream ss;
				ss << "LoadObj error face #" << f << " has "
					<< mesh.num_face_vertices[f] << " vertices";
				throw std::runtime_error(ss.str().c_str());
			}

			// load set of 3 indices for each face into OUR index std::vector
			for (unsigned int vn = 0; vn < 3u; vn++)
			{
				const auto idx = mesh.indices[f * 3u + vn];
				t1.indices.push_back(unsigned int(idx.vertex_index));
			}

			// reverse winding if file marked as CCW
			if (isCCW)
			{
				// swapping any two indices reverse the winding dir of triangle
				std::swap(t1.indices.back(), *std::prev(t1.indices.end(), 2));
			}
		}
		return t1;
	}

	void AdjustToTrueCenter()
	{
		// used to enable miniball to access vertex pos info
		struct VertexAccessor
		{
			// iterator type for iterating over vertices
			typedef std::vector<T>::const_iterator Pit;
			// it type for iterating over components of vertex
			// (pointer is used to iterate over members of class here)
			typedef const float* Cit;
			// functor that miniball uses to get element iter based on vertex iter
			Cit operator()(Pit it) const
			{
				return &it->pos.x;
			}
		};

		// solve the minimum bounding sphere
		Miniball::Miniball<VertexAccessor> mb(3, vertices.cbegin(), vertices.cend());
		// get center of min sphere
		// result is a pointer to float[3] (what a shitty fuckin interface)
		const auto pc = mb.center();
		const Vec3 center = { *pc,*std::next(pc),*std::next(pc,2) };
		// adjust all vertices so that center of minimal sphere is at 0,0
		for (auto& v : vertices)
		{
			v.pos -= center;
		}
	}
	float GetRadius() const
	{
		// find element with max distance from 0,0; that is our radius
		return std::max_element(vertices.begin(), vertices.end(),
			[](const T& v0, const T& v1)
		{
			return v0.pos.LenSq() < v1.pos.LenSq();
		}
		)->pos.Len();
	}

	static IndexedTriangleList<T> LoadNormals(const std::string& filename)
	{
		IndexedTriangleList<T> tl;

		// check first line of file to see if CCW winding comment exists
		bool isCCW = false;
		{
			std::ifstream file(filename);
			std::string firstline;
			std::getline(file, firstline);
			std::transform(firstline.begin(), firstline.end(), firstline.begin(), std::tolower);
			if (firstline.find("ccw") != std::string::npos)
			{
				isCCW = true;
			}
		}

		// these will be filled by obj loading function
		using namespace tinyobj;
		attrib_t attrib;
		std::vector<shape_t> shapes;
		std::string err;

		// load/parse the obj file
		const bool ret = LoadObj(&attrib, &shapes, nullptr, &err, filename.c_str());

		// check for errors
		if (!err.empty() && err.substr(0, 4) != "WARN")
		{
			throw std::runtime_error(("LoadObj returned error:" + err + " File:" + filename).c_str());
		}
		if (!ret)
		{
			throw std::runtime_error(("LoadObj returned false  File:" + filename).c_str());
		}
		if (shapes.size() == 0u)
		{
			throw std::runtime_error(("LoadObj object file had no shapes  File:" + filename).c_str());
		}

		// extract vertex data
		// attrib.vertices is a flat std::vector of floats corresponding
		// to vertex positions, laid out as xyzxyzxyz... etc.
		// first preallocate required space in OUR std::vector
		tl.vertices.reserve(attrib.vertices.size() / 3u);
		// iterate over individual vertices, construct Vec3s in OUR vector
		for (int i = 0; i < attrib.vertices.size(); i += 3)
		{
			tl.vertices.emplace_back(Vec3{
				attrib.vertices[i + 0],
				attrib.vertices[i + 1],
				attrib.vertices[i + 2]
				});
		}

		// extract index data
		// obj file can contain multiple meshes, we assume just 1
		const auto& mesh = shapes[0].mesh;
		// mesh contains a std::vector of num_face_vertices (uchar)
		// and a flat std::vector of indices. If all faces are triangles
		// then for any face f, the first index of that faces is [f * 3n]
		tl.indices.reserve(mesh.indices.size());
		for (size_t f = 0; f < mesh.num_face_vertices.size(); f++)
		{
			// make sure there are no non-triangle faces
			if (mesh.num_face_vertices[f] != 3u)
			{
				std::stringstream ss;
				ss << "LoadObj error face #" << f << " has "
					<< mesh.num_face_vertices[f] << " vertices";
				throw std::runtime_error(ss.str().c_str());
			}

			// load set of 3 indices for each face into OUR index std::vector
			for (size_t vn = 0; vn < 3u; vn++)
			{
				const auto idx = mesh.indices[f * 3u + vn];
				tl.indices.push_back(size_t(idx.vertex_index));
				// write normals into the vertices
				tl.vertices[(size_t)idx.vertex_index].n = Vec3{
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]
				};
			}

			// reverse winding if file marked as CCW
			if (isCCW)
			{
				// swapping any two indices reverse the winding dir of triangle
				std::swap(tl.indices.back(), *std::prev(tl.indices.end(), 2));
			}
		}

		return tl;
	}

	std::vector<T> vertices;
	std::vector<unsigned int> indices;
	JPG2Vector tex_img;
	Vec3 texColor;
};