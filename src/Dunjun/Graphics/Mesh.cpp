
#include <Dunjun/Graphics/Mesh.hpp>

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
			v0.normal += normal;
			v1.normal += normal;
			v2.normal += normal;
		}
	
		for (size_t i = 0; i < vertices.size(); i++)
			vertices[i].normal = normalize(vertices[i].normal);
	}

	Mesh::Mesh()
		: data()
		, generated(false)
		, vbo(0)
		, ibo(0)
		, drawType(DrawType::Triangles)
		, drawCount(0)
	{
	}

	Mesh::Mesh(const Data& data)
		: data(data)
		, generated(false)
		, vbo(0)
		, ibo(0)
		, drawType(data.drawType)
		, drawCount((s32)data.indices.size())
	{
		generate();
	}

	void Mesh::addData(const Data& d)
	{
		data = d;
		drawType = d.drawType;
		drawCount = d.indices.size();
		generated = false;
	}

	void Mesh::generate() const
	{
		if(generated)
			return;

		if(!vbo)
			glGenBuffers(1, &vbo);
		if(!ibo)
			glGenBuffers(1, &ibo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.vertices.size(),
						&data.vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // bind the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * data.indices.size(),
						&data.indices[0], GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the buffer

		generated = true;
	}

	void Mesh::draw() const
	{
		if(!generated)
			generate();

		// set attrib to data from data
		glEnableVertexAttribArray((u32)AttribLocation::Position); // enables attribute array[0] a_position
		glEnableVertexAttribArray((u32)AttribLocation::TexCoord); // enable attribute [1] a_texCoord
		glEnableVertexAttribArray((u32)AttribLocation::Color); // enables attribute array[2] a_color
		glEnableVertexAttribArray((u32)AttribLocation::Normal); // enables attribute array[3] a_normal

		glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // bind the buffer

		// pointer for attribute position (att position[], size of vertices x/y/z, int type, normalized?, stride, pointer)
		glVertexAttribPointer((u32)AttribLocation::Position, 3, 				
			GL_FLOAT, false, 
			sizeof(Vertex), 
			(const GLvoid*)0);													
		glVertexAttribPointer((u32)AttribLocation::TexCoord, 2, 				
			GL_FLOAT, false, 
			sizeof(Vertex), 								
			(const GLvoid*)(sizeof(Dunjun::Vector3)));		
		glVertexAttribPointer((u32)AttribLocation::Color, 4,
			GL_UNSIGNED_BYTE, true, 
			sizeof(Vertex),							
			(const GLvoid*)(sizeof(Dunjun::Vector3) + sizeof(Dunjun::Vector2)));
		glVertexAttribPointer((u32)AttribLocation::Normal, 3,					
			GL_FLOAT, false, 
			sizeof(Vertex),
			(const GLvoid*)(sizeof(Dunjun::Vector3) + sizeof(Dunjun::Vector2) + sizeof(Dunjun::Color)));
		// stride says how many floats there are per vertex
		// const void *pointer says how far offset the information starts in the vertex

		// get the draw info from ModelAsset asset
		//glDrawArrays(asset->drawType, asset->drawStart, asset->drawCount); // (mode to draw in, first vertex, total vertices)
		glDrawElements((GLenum)drawType, (s32)drawCount, GL_UNSIGNED_INT, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the buffer

		glDisableVertexAttribArray((u32)AttribLocation::Position); // disables attribute array[0] a_position
		glDisableVertexAttribArray((u32)AttribLocation::TexCoord); // disables attribute [1] a_texCoord
		glDisableVertexAttribArray((u32)AttribLocation::Color); // disables attribute array[2] a_color
		glDisableVertexAttribArray((u32)AttribLocation::Normal); // disables attribute array[3] a_normal

	}

}
