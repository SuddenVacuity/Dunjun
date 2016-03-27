#include <Dunjun/ShaderProgram.hpp>


#define GLOBAL static
#define INTERNAL static
#define LOCAL_PERSIST static

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
			: m_object(0) // set default values
			, m_linked(false)
			, m_errorLog()
		{
			m_object = glCreateProgram(); // define m_object as shader the program
		}

		ShaderProgram::~ShaderProgram()
		{
			if (m_object)
				glDeleteProgram(m_object);
		}

		// add shaders here
		// get the shader type and file name for the shader
		bool ShaderProgram::attachShaderFromFile(ShaderType type, const std::string& filename)
		{
			std::string source = stringFromFile(filename); // define source and the filename
			return attachShaderFromMemory(type, source);
		}
		bool ShaderProgram::attachShaderFromMemory(ShaderType type, const std::string& source)
		{
			// check if m_object got compiled
			if (!m_object)
				m_object = glCreateProgram();

			const char* shaderSource = source.c_str();

			GLuint shader;
			if (type == ShaderType::Vertex) // check the shader type
				shader = glCreateShader(GL_VERTEX_SHADER); // create the shader
			else if (type == ShaderType::Fragment)
				shader = glCreateShader(GL_FRAGMENT_SHADER); // create the shader

			glShaderSource(shader, 1, &shaderSource, nullptr);
			glCompileShader(shader);

			// check for compile error
			GLint status; // declare var to check shader status
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // define status as the compile status
			if (status == GL_FALSE)
			{
			std::string msg("Compile failure in shader:\n");

				GLint infoLogLength;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength); // get shader information i for integer and v for vector
				char* strInfoLog = new char[infoLogLength + 1];
				glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog);

				msg.append(strInfoLog); // append msg by adding strInfoLog
				delete[] strInfoLog;

				msg.append("\n");
				m_errorLog.append(msg);

				glDeleteShader(shader);


				return false;
			}

			glAttachShader(m_object, shader);

			return true;
		}

		void ShaderProgram::use() const
		{
			if(!isInUse()) // check that it's not in use already
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
		void ShaderProgram::checkInUse() const
		{
			if(!isInUse())
				throw std::runtime_error("Shader not in use.");
		}

		bool ShaderProgram::link()
		{

			// check if m_object got compiled
			if (!m_object)
				m_object = glCreateProgram();


			if(!isLinked())
			{
				glLinkProgram(m_object);

				GLint status; // check for link error
				glGetProgramiv(m_object, GL_LINK_STATUS, &status);

				if (status == GL_FALSE)
				{
					std::string msg("Shader program linking failure:\n");

					GLint infoLogLength;
					glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &infoLogLength);
					char* strInfoLog = new char[infoLogLength +1];
					glGetProgramInfoLog(m_object, infoLogLength, NULL, strInfoLog);
					msg.append(strInfoLog);
					delete[] strInfoLog;

					msg.append("\n");
					m_errorLog.append(msg);

					glDeleteProgram(m_object);
					m_object = 0;

					m_linked = false;
					return m_linked;
				}

				m_linked = true;
			}

			return m_linked;
		}
		bool ShaderProgram::isLinked()
		{
			return m_linked;
		}


		void ShaderProgram::bindAttribLocation(GLuint location, const std::string& name)
		{
			glBindAttribLocation(m_object, location, name.c_str());
			m_attribLocations[name] = location;
		}
		GLint ShaderProgram::getAttribLocation(const std::string& name)
		{
			auto found = m_attribLocations.find(name); // find the name of attrib location
			if (found != m_attribLocations.end()) // check if location was found
			{
				return found->second; // if not return found second position
			}
			GLint loc = glGetAttribLocation(m_object, name.c_str());
			m_attribLocations[name] = loc;
			return loc;
			
		}
		GLint ShaderProgram::getUniformLocation(const std::string& name)
		{
			auto found = m_uniformLocations.find(name);
			if (found != m_uniformLocations.end())
			{
				return found->second;
			}
			GLint loc = glGetUniformLocation(m_object, name.c_str());
			m_uniformLocations[name] = loc;
			return loc;
		}

		void ShaderProgram::setUniform(const std::string& name, f32 x)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1) // check if getUniformLocation worked
				return;
			glUniform1f(loc, x);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform2f(loc, x, y);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y, f32 z)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform3f(loc, x, y, z);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y, f32 z, f32 w)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform4f(loc, x, y, z, w);
		}

		void ShaderProgram::setUniform(const std::string& name, u32 x)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, x);
		}
		void ShaderProgram::setUniform(const std::string& name, i32 x)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, (i32)x);
		}
		void ShaderProgram::setUniform(const std::string& name, bool x)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, x);
		}

		void ShaderProgram::setUniform(const std::string& name, const Vector2& v)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform2fv(loc, 1, v.data);

		}
		void ShaderProgram::setUniform(const std::string& name, const Vector3& v)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform3fv(loc, 1, v.data);
		}
		void ShaderProgram::setUniform(const std::string& name, const Vector4& v)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform4fv(loc, 1, v.data);
		}
		void ShaderProgram::setUniform(const std::string& name, const Matrix4& m)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniformMatrix4fv(loc, 1, GL_FALSE, m[0].data);
		}


		void ShaderProgram::setUniform(const std::string& name, const Quaternion& t)
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;

			glUniform4fv(loc, 1, t.data);

		}

		void ShaderProgram::setUniform(const std::string& name, const Transform& t)
		{
			checkInUse();

			setUniform(name + ".position", t.position);
			setUniform(name + ".orientation", t.orientation);
			setUniform(name + ".scale", t.scale);
		}
} // end Dunjun
