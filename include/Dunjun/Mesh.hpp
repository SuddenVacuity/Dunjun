#ifndef DUNJUN_MESH_HPP
#define DUNJUN_MESH_HPP

#include <Dunjun/Vertex.hpp>

namespace Dunjun
{
	enum class AttribLocation : u32
	{
		Position = 0,
		TexCoord = 1,
		Color = 2,
	};




	class Mesh
	{
	public:
		struct Data
		{
			GLenum drawType = GL_TRIANGLES;

			//std::vector<Vertex> vertices;
			VertexArray vertices;
			std::vector<u32> indices;

			// push a single triangle from indices
			Data& addFace(u32 a, u32 b, u32 c)
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);

				return *this;
			}

			// push a single triangle from indices into an existing mesh
			Data& addFace(u32 offset, u32 a, u32 b, u32 c)
			{
				indices.push_back(offset + a);
				indices.push_back(offset + b);
				indices.push_back(offset + c);

				return *this;
			}
		};

		Mesh();
		Mesh(const Data& data);

		virtual ~Mesh()
		{
			destroy();
		}

		void addData(const Data& data);

		void generate() const;

		void draw() const;

		inline void destroy() const
		{
			glDeleteBuffers(1, &m_vbo);
			glDeleteBuffers(1, &m_ibo);
		}


	private:
		Data m_data;

		mutable b32 m_generated;

		GLuint m_vbo;
		GLuint m_ibo;
		GLenum m_drawType;
		GLint m_drawCount;


	};
} // end Dunjun

#endif
