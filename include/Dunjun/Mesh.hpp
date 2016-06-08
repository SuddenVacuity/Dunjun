#ifndef DUNJUN_MESH_HPP
#define DUNJUN_MESH_HPP

#include <Dunjun/Vertex.hpp>

namespace Dunjun
{
	class SceneRenderer;

	enum class AttribLocation : u32
	{
		Position = 0,
		TexCoord = 1,
		Color = 2,
		Normal = 3,
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
				indices.emplace_back(a);
				indices.emplace_back(b);
				indices.emplace_back(c);

				return *this;
			}

			// push a single triangle from indices into an existing mesh
			Data& addFace(u32 offset, u32 a, u32 b, u32 c)
			{
				indices.emplace_back(offset + a);
				indices.emplace_back(offset + b);
				indices.emplace_back(offset + c);

				return *this;
			}

			void generateNormals();
		};

		Mesh();
		Mesh(const Data& data);

		virtual ~Mesh()
		{
			destroy();
		}

		void addData(const Data& data);

		void generate() const;

		inline void destroy() const
		{
			glDeleteBuffers(1, &m_vbo);
			glDeleteBuffers(1, &m_ibo);
		}


	private:
		friend class SceneRenderer;

		void draw() const;

		Data m_data;

		mutable b32 m_generated;

		mutable GLuint m_vbo;
		mutable GLuint m_ibo;
		GLenum m_drawType;
		GLint m_drawCount;


	};
} // end Dunjun

#endif
