#ifndef DUNJUN_MESH_HPP
#define DUNJUN_MESH_HPP

#include <Dunjun/Vertex.hpp>

namespace Dunjun
{
	enum class AttribLocation : u32
	{
		Position = 0,
		Color = 1,
		TexCoord = 2,
	};




	class Mesh
	{
	public:
		struct Data
		{
			GLenum drawType = GL_TRIANGLES;

			std::vector<Vertex> vertices;
			std::vector<u32> indices;
		};

		Mesh();
		Mesh(const Data& data);

		virtual ~Mesh()
		{
			destroy();
		}

		void generate();

		void draw();

		inline void destroy() const
		{
			glDeleteBuffers(1, &m_vbo);
			glDeleteBuffers(1, &m_ibo);
		}


	private:
		b32 m_generated;

		Data m_data;

		GLuint m_vbo;
		GLuint m_ibo;
		GLenum m_drawType;
		GLint m_drawCount;


	};
} // end Dunjun

#endif
