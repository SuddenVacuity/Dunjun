#ifndef DUNJUN_SHADERPROGRAM_HPP
#define DUNJUN_SHADERPROGRAM_HPP

#include <Dunjun/Common.hpp>
#include <Dunjun/NonCopyable.hpp>
#include <Dunjun/Math.hpp>
//#include <Dunjun/Math/Matrix4.hpp>

#include <Dunjun/OpenGL.hpp>

#include <map>
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

		void setUniform(const GLchar* name, f32 x);
		void setUniform(const GLchar* name, f32 x, f32 y);
		void setUniform(const GLchar* name, f32 x, f32 y, f32 z);
		void setUniform(const GLchar* name, f32 x, f32 y, f32 z, f32 w);
		void setUniform(const GLchar* name, u32 x);
		void setUniform(const GLchar* name, i32 x); // in case an interger is needed

		void setUniform(const GLchar* name, bool x); // incase a bool is needed

		void setUniform(const GLchar* name, const Vector2& v); // set u_model in vertex shader
		void setUniform(const GLchar* name, const Vector3& v);
		void setUniform(const GLchar* name, const Vector4& v);
		void setUniform(const GLchar* name, const Matrix4& m);

		inline GLuint getObject() const { return m_object; }
		inline const std::string& getErrorLog() const { return m_errorLog; }
		

	private:
		GLuint m_object; // the file
		bool m_linked;
		std::string m_errorLog;

		std::map<const GLchar*, GLint> m_attribLocations;
		std::map<const GLchar*, GLint> m_uniformLocations;


	};
}



#endif
