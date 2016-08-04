#ifndef DUNJUN_GRAPHICS_VERTEX_HPP
#define DUNJUN_GRAPHICS_VERTEX_HPP

#include <Dunjun/Graphics/Color.hpp>

namespace Dunjun
{
	struct Vertex // must come before render
	{
		Vector3 position;
		Vector2 texCoord;
		Color color;
		Vector3 normal;
		Vector3 tangent;


		GLOBAL const Vertex Identity;

		//Vertex() = default;
		
		Vertex(const Vector3& position = Vector3::Zero,
			   const Vector2& texCoord = Vector2::Zero,
			   const Color& color = ColorLib::White,
			   const Vector3& normal = Vector3::Zero,
			   const Vector3& tangent = Vector3::Zero)
			: position(position)
			, texCoord(texCoord)
			, color(color)
			, normal(normal)
			, tangent(tangent)
		{
		}
	};
	//class VertexArray
	//{
	//public:
	//	inline VertexArray& append(const Vertex& vertex)
	//	{
	//		m_vertices.emplace_back(vertex);
	//		return *this;
	//	}
	//
	//	inline VertexArray& append(const Vector3& position, 
	//							   const Vector2& texCoord, 
	//							   const Color& color, 
	//							   const Vector3& normal = { 0, 0, 0 })
	//	{
	//		return append(Vertex(position, texCoord, color, normal));
	//		
	//	}
	//
	//	inline const Vertex& operator[](size_t index) const
	//	{
	//		return m_vertices[index];
	//	}
	//
	//	inline Vertex& operator[](size_t index)
	//	{
	//		return m_vertices[index];
	//	}
	//
	//	//const Vertex* getPtr() const { return &m_vertices[0]; }
	//	//inline const std::vector<Vertex>& getVector() const { return m_vertices; }
	//	inline const size_t size() const { return m_vertices.size(); }
	//
	//	inline void reserve(size_t size) { return m_vertices.reserve(size); }
	//private:
	//	std::vector<Vertex> m_vertices;
	//};
	//
	//
} // end Dunjun

#endif
