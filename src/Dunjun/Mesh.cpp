
#include <Dunjun/Mesh.hpp>

namespace Dunjun
{
	void Mesh::Data::generateNormals()
	{
		for(u32 i = 0; i < indices.size(); i += 3)
		{
			Vertex& v0 = vertices[indices[i + 0]];
			Vertex& v1 = vertices[indices[i + 1]];
			Vertex& v2 = vertices[indices[i + 2]];
	
			Vector3 a = v1.position - v0.position;
			Vector3 b = v2.position - v1.position;
			
			Vector3 normal = normalize(cross(a, b));

			// smoothes out shared vertices/indices
			v0.normal = normal;
			v1.normal = normal;
			v2.normal = normal;
		}
	
		for (size_t i = 0; i < vertices.size(); i++)
			vertices[i].normal = normalize(vertices[i].normal);
	}

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

		// set attrib to data from m_data
		glEnableVertexAttribArray((u32)AttribLocation::Position); // enables attribute array[0] a_position
		glEnableVertexAttribArray((u32)AttribLocation::TexCoord); // enable attribute [1] a_texCoord
		glEnableVertexAttribArray((u32)AttribLocation::Color); // enables attribute array[2] a_color
		glEnableVertexAttribArray((u32)AttribLocation::Normal); // enables attribute array[3] a_normal

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // bind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo); // bind the buffer

		// pointer for attribute position (att position[], size of vertices x/y/z, int type, normalized?, stride, pointer)
		glVertexAttribPointer((u32)AttribLocation::Position, 3, 					// 
			GL_FLOAT, GL_FALSE, sizeof(Vertex),  									// GL_FLOAT specifies they are floats
			(const GLvoid*)0);														// GL_FALSE specifies they do not need to be normalized
		glVertexAttribPointer((u32)AttribLocation::TexCoord, 2, 					// 
			GL_FLOAT, GL_FALSE, sizeof(Vertex), 									// so first value is data type
			(const GLvoid*)(sizeof(Dunjun::Vector3)));								// and second is if need normalize
		glVertexAttribPointer((u32)AttribLocation::Color, 4,						// 
			GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),								// 
			(const GLvoid*)(sizeof(Dunjun::Vector3) + sizeof(Dunjun::Vector2)));	// 
		glVertexAttribPointer((u32)AttribLocation::Normal, 3,						// 
			GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(const GLvoid*)(sizeof(Dunjun::Vector3) + sizeof(Dunjun::Vector2) + sizeof(Dunjun::Color)));
		// stride says how many floats there are per vertex
		// const void *pointer says how far offset the information starts in the vertex

		// get the draw info from ModelAsset asset
		//glDrawArrays(asset->drawType, asset->drawStart, asset->drawCount); // (mode to draw in, first vertex, total vertices)
		glDrawElements(m_drawType, m_drawCount, GL_UNSIGNED_INT, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the buffer

		glDisableVertexAttribArray((u32)AttribLocation::Position); // disables attribute array[0] a_position
		glDisableVertexAttribArray((u32)AttribLocation::TexCoord); // disables attribute [1] a_texCoord
		glDisableVertexAttribArray((u32)AttribLocation::Color); // disables attribute array[2] a_color
		glDisableVertexAttribArray((u32)AttribLocation::Normal); // disables attribute array[3] a_normal

	}

}
