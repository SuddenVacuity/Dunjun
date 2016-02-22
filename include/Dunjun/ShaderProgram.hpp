#ifndef DUNJUN_SHADERPROGRAM_HPP
#define DUNJUN_SHADERPROGRAM_HPP

#include <Dunjun/Common.hpp>
#include <Dunjun/NonCopyable.hpp>

#include <Dunjun/OpenGL.hpp>

#include <string>

namespace Dunjun
{
	enum class ShaderType
	{
		Vertex,
		Fragment,
	};
	class ShaderProgram : public NonCopyable
	{
	public:
		ShaderProgram(); // Progarm for the shader
		virtual ~ShaderProgram();

		bool attachShaderFromFile(ShaderType type, const std::string& filename); // get shader info from file
		bool attachShaderFromMemory(ShaderType type, const std::string& filename);

		void use() const; // use
		bool isInUse() const; // check if in use
		void stopUsing() const;

		bool link();
		bool isLinked();


		void bindAttribLocation(GLuint location, const GLchar* name);

		GLint getAttribLocation( const GLchar* name);
		GLint getUniformLocation( const GLchar* name);

		void setUniform(const GLchar* name, float x);
		void setUniform(const GLchar* name, float x, float y);
		void setUniform(const GLchar* name, float x, float y, float z);
		void setUniform(const GLchar* name, float x, float y, float z, float w);
		void setUniform(const GLchar* name, unsigned int x);
		void setUniform(const GLchar* name, int x); // in case an interger is needed
		void setUniform(const GLchar* name, bool x); // incase a bool is needed

		inline GLuint getObject() const { return m_object; }
		inline const std::string& getErrorLog() const { return m_errorLog; }
		

	private:
		GLuint m_object; // the file
		bool m_linked;
		std::string m_errorLog;
	};
}



#endif