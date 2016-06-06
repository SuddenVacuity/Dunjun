#ifndef DUNJUN_VERTEX_HPP
#define DUNJUN_VERTEX_HPP

#include <Dunjun/Color.hpp>

namespace Dunjun
{
	struct Vertex // must come before render
	{
		Vector3 position = {0, 0, 0};
		Vector2 texCoord = {0, 0};
		Color color = ColorLib::White;
		Vector3 normal = {0, 0, 0};

		Vertex() = default;

		Vertex(const Vector3& position, const Vector2& texCoord, const Color& color, const Vector3& normal = { 0, 0, 0 })
			: position(position)
			, texCoord(texCoord)
			, color(color)
			, normal(normal)
		{
		}
	};

	class VertexArray
	{
	public:
		inline VertexArray& append(const Vertex& vertex)
		{
			m_vertices.push_back(vertex);
			return *this;
		}

		inline VertexArray& append(const Vector3& position, 
								   const Vector2& texCoord, 
								   const Color& color, 
								   const Vector3& normal = { 0, 0, 0 })
		{
			return append(Vertex(position, texCoord, color, normal));
			
		}

		inline const Vertex& operator[](size_t index) const
		{
			return m_vertices[index];
		}

		inline Vertex& operator[](size_t index)
		{
			return m_vertices[index];
		}

		//const Vertex* getPtr() const { return &m_vertices[0]; }
		//inline const std::vector<Vertex>& getVector() const { return m_vertices; }
		inline const size_t size() const { return m_vertices.size(); }
	private:
		std::vector<Vertex> m_vertices;
	};


} // end Dunjun

#endif
