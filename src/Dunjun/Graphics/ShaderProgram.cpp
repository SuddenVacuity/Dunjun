#include <Dunjun/Graphics/ShaderProgram.hpp>


#define GLOBAL static
#define INTERNAL static
#define LOCAL_PERSIST static

namespace Dunjun
{
	std::vector<std::string> split(const std::string& s, char delim)
	{
		std::vector<std::string> elems;

		const char* cstr = s.c_str();
		size_t strLength = s.length();
		size_t start = 0;
		size_t end = 0;

		while (end <= strLength)
		{
			while (end <= strLength)
			{
				if(cstr[end] == delim)
					break;
				end++;
			}
			elems.emplace_back(s.substr(start, end - start));
			start = end + 1;
			end = start;
		}


		return elems;
	}

	INTERNAL std::string stringFromFile(const std::string& filename) // function used to import strings from external files into code at compile
	{
		std::ifstream file; // declare a file stream
		file.open(std::string(BaseDirectories::Shaders + filename.c_str()), std::ios::in | std::ios::binary); // tell to open file.c_str()(to be defined later)
																	  // and to take input fromm the file
		std::string fileDirectory = FileSystem::getFileDirectory(filename) + "/";

		std::string output; // declares string output
		std::string line; // declares string line

		if (!file.is_open()) // check if there was an error finding/opening the file
		{
			throwRuntimeError(std::string("Failed to open File :") + filename);
		}
		else
		{
			while (file.good()) // while file is open
			{
				std::getline(file, line); // get a line from the file and define line as the string

				if(line.find("#include") == std::string::npos)
				{
					output.append(line + "\n");
				}
				else
				{
					std::string includeFilename = split(line, ' ')[1];

					// get path\name of #include file
					if(includeFilename[0] == '<') // absolute library path
					{
						size_t closingBracketPos = 0;
						for(size_t i = 1; i < includeFilename.length(); i++)
						{
							if(includeFilename[i] == '>')
							{
								closingBracketPos = i;
								break;
							}
						}

						if(closingBracketPos > 1)
						{
							includeFilename = includeFilename.substr(1, closingBracketPos - 1);
						}
						else
						{
							includeFilename = "";
						}
					}
					else if (includeFilename[0] == '\"') // relative path
					{
						size_t closingSpeechMarkPos = 0;
						for(size_t i = 1; i < includeFilename.length(); i++)
						{
							if(includeFilename[i] == '\"')
							{
								closingSpeechMarkPos = i;
								break;
							}
						}

						if(closingSpeechMarkPos > 1)
							includeFilename = includeFilename.substr(1, closingSpeechMarkPos - 1);
						else
							includeFilename = "";

					}

					if(includeFilename.length() > 0)
						output.append(stringFromFile(includeFilename) + "\n");
				}

			}
		}

		file.close();
		return output;
	}


		ShaderProgram::ShaderProgram()
			: m_handle(0) // set default values
			, m_isLinked(false)
			, m_errorLog()
		{
		}

		ShaderProgram::~ShaderProgram()
		{
			if (m_handle)
				glDeleteProgram(m_handle);
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
			if (!m_handle)
				m_handle = glCreateProgram();

			const char* shaderSource = source.c_str();

			u32 shader;
			if (type == ShaderType::Vertex) // check the shader type
				shader = glCreateShader(GL_VERTEX_SHADER); // create the shader
			else if (type == ShaderType::Fragment)
				shader = glCreateShader(GL_FRAGMENT_SHADER); // create the shader
			else
				throwRuntimeError("Shader Type Unknown - ShaderProgram::attachShaderFromMemory()");

			glShaderSource(shader, 1, &shaderSource, nullptr);
			glCompileShader(shader);

			// check for compile error
			s32 status; // declare var to check shader status
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // define status as the compile status
			if (status == GL_FALSE)
			{
			std::string msg("Compile failure in shader:\n");

				s32 infoLogLength;
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

			glAttachShader(m_handle, shader);

			return true;
		}

		void ShaderProgram::use() const
		{
			if(!isInUse()) // check that it's not in use already
				glUseProgram(m_handle);
		}
		bool ShaderProgram::isInUse() const
		{
			s32 currentProgram = 0;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

			return (currentProgram == (s32)m_handle);
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
			if (!m_handle)
				m_handle = glCreateProgram();


			if(!m_isLinked)
			{
				glLinkProgram(m_handle);

				s32 status; // check for link error
				glGetProgramiv(m_handle, GL_LINK_STATUS, &status);

				if (status == GL_FALSE)
				{
					std::string msg("Shader program linking failure:\n");

					s32 infoLogLength;
					glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &infoLogLength);
					char* strInfoLog = new char[infoLogLength +1];
					glGetProgramInfoLog(m_handle, infoLogLength, nullptr, strInfoLog);
					msg.append(strInfoLog);
					delete[] strInfoLog;

					msg.append("\n");
					m_errorLog.append(msg);

					glDeleteProgram(m_handle);
					m_handle = 0;

					m_isLinked = false;
					return m_isLinked;
				}

				m_isLinked = true;
			}

			return m_isLinked;
		}

		bool ShaderProgram::isLinked() const
		{
			return m_isLinked;
		}

		const std::string& ShaderProgram::getErrorLog() const
		{
			return m_errorLog;
		}

		GLuint ShaderProgram::getNativeHandle() const
		{
			return m_handle;
		}

		void ShaderProgram::bindAttribLocation(GLuint location, const std::string& name)
		{
			glBindAttribLocation(m_handle, location, name.c_str());
			m_attribLocations[name] = location;
		}
		s32 ShaderProgram::getAttribLocation(const std::string& name) const
		{
			auto found = m_attribLocations.find(name); // find the name of attrib location
			if (found != m_attribLocations.end()) // check if location was found
			{
				return found->second; // if not return found second position
			}
			s32 loc = glGetAttribLocation(m_handle, name.c_str());
			m_attribLocations[name] = loc;
			return loc;
			
		}
		s32 ShaderProgram::getUniformLocation(const std::string& name) const
		{
			auto found = m_uniformLocations.find(name);
			if (found != m_uniformLocations.end())
			{
				return found->second;
			}
			s32 loc = glGetUniformLocation(m_handle, name.c_str());
			m_uniformLocations[name] = loc;
			return loc;
		}

		void ShaderProgram::setUniform(const std::string& name, f32 x) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1) // check if getUniformLocation worked
				return;
			glUniform1f(loc, x);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform2f(loc, x, y);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y, f32 z) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform3f(loc, x, y, z);
		}
		void ShaderProgram::setUniform(const std::string& name, f32 x, f32 y, f32 z, f32 w) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform4f(loc, x, y, z, w);
		}

		void ShaderProgram::setUniform(const std::string& name, u32 x) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, x);
		}
		void ShaderProgram::setUniform(const std::string& name, s32 x) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, (s32)x);
		}
		void ShaderProgram::setUniform(const std::string& name, bool x) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, static_cast<int>(x));
		}

		void ShaderProgram::setUniform(const std::string& name, const Vector2& v) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform2fv(loc, 1, &v[0]);

		}
		void ShaderProgram::setUniform(const std::string& name, const Vector3& v) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform3fv(loc, 1, &v[0]);
		}
		void ShaderProgram::setUniform(const std::string& name, const Vector4& v) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform4fv(loc, 1, &v[0]);
		}
		void ShaderProgram::setUniform(const std::string& name, const Matrix4& m) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
		}


		void ShaderProgram::setUniform(const std::string& name, const Quaternion& q) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;

			glUniform4fv(loc, 1, &q.data[0]);

		}

		void ShaderProgram::setUniform(const std::string& name, const Transform& t) const
		{
			checkInUse();

			setUniform(name + ".position", t.position);
			setUniform(name + ".orientation", t.orientation);
			setUniform(name + ".scale", t.scale);
		}

		void ShaderProgram::setUniform(const std::string& name, const Color& c) const
		{
			f32 r, g, b, a;
			r = c.r / 255.0f;
			g = c.g / 255.0f;
			b = c.b / 255.0f;
			a = c.a / 255.0f;

			setUniform(name, r, g, b, a);
		}
} // end Dunjun
