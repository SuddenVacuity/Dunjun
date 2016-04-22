
#include <Dunjun/Mesh.hpp>

namespace Dunjun
{
	Mesh::Mesh()
		: m_data()
		, m_generated(false)
		, m_vbo(0)
		, m_ibo(0)
		, m_drawType(GL_TRIANGLES)
		, m_drawCount(0)
	{
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);
	}

	Mesh::Mesh(const Data& data)
		: m_data(data)
		, m_generated(false)
		, m_vbo(0)
		, m_ibo(0)
		, m_drawType(data.drawType)
		, m_drawCount(data.indices.size())
	{
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		generate();
	}

	void Mesh::addData(const Data& data)
	{
		m_data = data;
		m_drawType = m_data.drawType;
		m_drawCount = m_data.indices.size();
		m_generated = false;
	}

	void Mesh::generate() const
	{
		if(m_generated)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // bind the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_data.vertices.size(),
						&m_data.vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo); // bind the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * m_data.indices.size(),
						&m_data.indices[0], GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the buffer

		m_generated = true;
	}

	void Mesh::draw() const
	{
		if(!m_generated)
			generate();

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // bind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo); // bind the buffer

		// set attrib to data from m_data
		glEnableVertexAttribArray((u32)AttribLocation::Position); // enables attribute array[0] a_position
		glEnableVertexAttribArray((u32)AttribLocation::TexCoord); // enable attribute [1] a_texCoord
		glEnableVertexAttribArray((u32)AttribLocation::Color); // enables attribute array[2] a_color ''

		// pointer for attribute position (att position[], size of vertices x/y/z, int type, normalized?, stride, pointer)
		glVertexAttribPointer((u32)AttribLocation::Position, 3, 
			GL_FLOAT, GL_FALSE, sizeof(Vertex),  
			(const GLvoid*)0);
		glVertexAttribPointer((u32)AttribLocation::TexCoord, 2, 
			GL_FLOAT, GL_FALSE, sizeof(Vertex), 
			(const GLvoid*)(sizeof(Dunjun::Vector3)));
		glVertexAttribPointer((u32)AttribLocation::Color, 4, 
			GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), 
			(const GLvoid*)(sizeof(Dunjun::Vector3) + sizeof(Dunjun::Vector2)));
		// stride says how many floats there are per vertex
		// const void *pointer says how far offset the information starts in the vertex

		// get the draw info from ModelAsset asset
		//glDrawArrays(asset->drawType, asset->drawStart, asset->drawCount); // (mode to draw in, first vertex, total vertices)
		glDrawElements(m_drawType, m_drawCount, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray((u32)AttribLocation::Position); // enables attribute array[0] a_position
		glDisableVertexAttribArray((u32)AttribLocation::TexCoord); // enable attribute [1] a_texCoord
		glDisableVertexAttribArray((u32)AttribLocation::Color); // enables attribute array[2] a_color ''


		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the buffer

	}

}
