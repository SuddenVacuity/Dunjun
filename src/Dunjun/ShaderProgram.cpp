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
		else
		{
			while (file.good()) // while file is open
			{
				getline(file, line); // get a line from the file and define line as the string
				output.append(line + "\n"); // add a new line to the string line
			}
		}

		file.close();
		return output;
	}


		ShaderProgram::ShaderProgram()
			: object(0) // set default values
			, isLinked(false)
			, errorLog()
		{
			object = glCreateProgram(); // define m_object as shader the program
		}

		ShaderProgram::~ShaderProgram()
		{
			if (object)
				glDeleteProgram(object);
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
			if (!object)
				object = glCreateProgram();

			const char* shaderSource = source.c_str();

			GLuint shader;
			if (type == ShaderType::Vertex) // check the shader type
				shader = glCreateShader(GL_VERTEX_SHADER); // create the shader
			else if (type == ShaderType::Fragment)
				shader = glCreateShader(GL_FRAGMENT_SHADER); // create the shader
			else
				throw std::runtime_error("Shader Type Unknown - ShaderProgram::attachShaderFromMemory()");

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
				errorLog.data.append(msg);

				glDeleteShader(shader);


				return false;
			}

			glAttachShader(object, shader);

			return true;
		}

		void ShaderProgram::use() const
		{
			if(!isInUse()) // check that it's not in use already
				glUseProgram(object);
		}
		bool ShaderProgram::isInUse() const
		{
			GLint currentProgram = 0;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

			return (currentProgram == (GLint)object);
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
			if (!object)
				object = glCreateProgram();


			if(!isLinked)
			{
				glLinkProgram(object);

				GLint status; // check for link error
				glGetProgramiv(object, GL_LINK_STATUS, &status);

				if (status == GL_FALSE)
				{
					std::string msg("Shader program linking failure:\n");

					GLint infoLogLength;
					glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
					char* strInfoLog = new char[infoLogLength +1];
					glGetProgramInfoLog(object, infoLogLength, nullptr, strInfoLog);
					msg.append(strInfoLog);
					delete[] strInfoLog;

					msg.append("\n");
					errorLog.data.append(msg);

					glDeleteProgram(object);
					object = 0;

					isLinked = false;
					return isLinked;
				}

				isLinked = true;
			}

			return isLinked;
		}

		void ShaderProgram::bindAttribLocation(GLuint location, const std::string& name)
		{
			glBindAttribLocation(object, location, name.c_str());
			m_attribLocations[name] = location;
		}
		GLint ShaderProgram::getAttribLocation(const std::string& name) const
		{
			auto found = m_attribLocations.find(name); // find the name of attrib location
			if (found != m_attribLocations.end()) // check if location was found
			{
				return found->second; // if not return found second position
			}
			GLint loc = glGetAttribLocation(object, name.c_str());
			m_attribLocations[name] = loc;
			return loc;
			
		}
		GLint ShaderProgram::getUniformLocation(const std::string& name) const
		{
			auto found = m_uniformLocations.find(name);
			if (found != m_uniformLocations.end())
			{
				return found->second;
			}
			GLint loc = glGetUniformLocation(object, name.c_str());
			m_uniformLocations[name] = loc;
			return loc;
		}

		void ShaderProgram::setUniform(const std::string& name, f32 x) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1) // check if getUniformLocation worked
				return;
			glUniform1f(loc, x);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform2f(loc, x, y);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y, f32 z) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform3f(loc, x, y, z);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y, f32 z, f32 w) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform4f(loc, x, y, z, w);
		}

		void ShaderProgram::setUniform(const std::string& name, u32 x) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, x);
		}
		void ShaderProgram::setUniform(const std::string& name, s32 x) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, (s32)x);
		}
		void ShaderProgram::setUniform(const std::string& name, bool x) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, static_cast<int>(x));
		}

		void ShaderProgram::setUniform(const std::string& name, const Vector2& v) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform2fv(loc, 1, &v[0]);

		}
		void ShaderProgram::setUniform(const std::string& name, const Vector3& v) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform3fv(loc, 1, &v[0]);
		}
		void ShaderProgram::setUniform(const std::string& name, const Vector4& v) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform4fv(loc, 1, &v[0]);
		}
		void ShaderProgram::setUniform(const std::string& name, const Matrix4& m) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
		}


		void ShaderProgram::setUniform(const std::string& name, const Quaternion& t) const
		{
			checkInUse();
			GLint loc = getUniformLocation(name);
			if (loc == -1)
				return;

			glUniform4fv(loc, 1, &t.data[0]);

		}

		void ShaderProgram::setUniform(const std::string& name, const Transform& t) const
		{
			checkInUse();

			setUniform(name + ".position", t.position);
			setUniform(name + ".orientation", t.orientation);
			setUniform(name + ".scale", t.scale);
		}
} // end Dunjun
