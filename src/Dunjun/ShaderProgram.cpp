#include <Dunjun/ShaderProgram.hpp>

#include <fstream>

namespace Dunjun
{
	INTERNAL std::string stringFromFile(const std::string& filename) // function used to import strings from external files into code at compile
	{
		std::ifstream file; // declare a file stream
		file.open(filename.c_str(), std::ios::in | std::ios::binary); // tell to open file.c_str()(to be defined later)
																	  // and to take input fromm the file

		std::string output; // declares string output
		std::string line; // declares string line

		if (!file.is_open()) // check if there was an error finding/opening the file
		{
			std::runtime_error(std::string("Failed to open file: ") + filename);
		}
		while (file.good()) // while file is open
		{
			getline(file, line); // get a line from the file and define line as the string
			output.append(line + "\n"); // add a new line to the string line
		}
		file.close();
		return output;
	}


		ShaderProgram::ShaderProgram()
			: m_object(0)
			, m_linked(false)
			, m_errorLog()
		{
			m_object = glCreateProgram();
		}

		ShaderProgram::~ShaderProgram()
		{
			glDeleteProgram(m_object);
		}

		// add shaders here
		bool ShaderProgram::attachShaderFromFile(ShaderType type, const std::string& filename)
		{
			std::string source = stringFromFile(filename);
			return attachShaderFromMemory(type, source);
		}
		bool ShaderProgram::attachShaderFromMemory(ShaderType type, const std::string& source)
		{
			const char* shaderSource = source.c_str();

			GLuint shader;
			if (type == ShaderType::Vertex)
				shader = glCreateShader(GL_VERTEX_SHADER);
			else if (type == ShaderType::Fragment)
				shader = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(shader, 1, &shaderSource, nullptr);
			glCompileShader(shader);

			glAttachShader(m_object, shader);

			return true;
		}

		void ShaderProgram::use() const
		{
			if(!isInUse())
				glUseProgram(m_object);
		}
		bool ShaderProgram::isInUse() const
		{
			GLint currentProgram = 0;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

			return (currentProgram == (GLint)m_object);
		}
		void ShaderProgram::stopUsing() const
		{
			if (isInUse())
				glUseProgram(0);
		}

		bool ShaderProgram::link()
		{
			if(!isLinked())
			{
				glLinkProgram(m_object);

				m_linked = true;
			}

			return m_linked;
		}
		bool ShaderProgram::isLinked()
		{
			return m_linked;
		}


		void ShaderProgram::bindAttribLocation(GLuint location, const char* name)
		{
			glBindAttribLocation(m_object, location, name);
		}
		GLint ShaderProgram::getAttribLocation(const GLchar* name)
		{
			return glGetAttribLocation(m_object, name);
		}
		GLint ShaderProgram::getUniformLocation(const GLchar* name)
		{
			return glGetUniformLocation(m_object, name);
		}

		void ShaderProgram::setUniform(const GLchar* name, float x)
		{
			if (!isInUse())
				use();
			glUniform1f(glGetUniformLocation(m_object, name), x);
		}
		void ShaderProgram::setUniform(const GLchar* name, float x, float y)
		{
			if (!isInUse())
				use();
			glUniform2f(glGetUniformLocation(m_object, name), x, y);
		}
		void ShaderProgram::setUniform(const GLchar* name, float x, float y, float z)
		{
			if (!isInUse())
				use();
			glUniform3f(glGetUniformLocation(m_object, name), x, y, z);
		}
		void ShaderProgram::setUniform(const GLchar* name, float x, float y, float z, float w)
		{
			if (!isInUse())
				use();
			glUniform4f(glGetUniformLocation(m_object, name), x, y, z, w);
		}

		void ShaderProgram::setUniform(const GLchar* name, unsigned int x)
		{
			glUniform1i(getUniformLocation(name), x);
		}
		void ShaderProgram::setUniform(const GLchar* name, int x)
		{
			glUniform1i(getUniformLocation(name), (int)x);
		}
		void ShaderProgram::setUniform(const GLchar* name, bool x)
		{
			glUniform1i(getUniformLocation(name), x);
		}

}