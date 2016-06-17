#ifndef DUNJUN_GRAPHICS_MESH_HPP
#define DUNJUN_GRAPHICS_MESH_HPP

#include <Dunjun/Graphics/Vertex.hpp>

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

	enum class DrawType : GLenum
	{
		Points = GL_POINTS,
		Lines = GL_LINES,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP,
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		// Quads = GL_QUADS,
	};

	class Mesh
	{
	public:
		struct Data
		{
			DrawType drawType = DrawType::Triangles;

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

	private:
		friend class SceneRenderer;

		inline void destroy() const
		{
			if (m_vbo)
				glDeleteBuffers(1, &m_vbo);
			if (m_ibo)
				glDeleteBuffers(1, &m_ibo);
		}

		void draw() const;

		Data m_data;

		mutable b32 m_generated = false;

		mutable u32 m_vbo = 0;
		mutable u32 m_ibo = 0;
		DrawType m_drawType = DrawType::Triangles;
		s32 m_drawCount = 0;


	};
} // end Dunjun

#endif
