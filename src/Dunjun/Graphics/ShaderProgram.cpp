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

	INTERNAL String shaderSourceFromFile(const String& filename)
	{
		String filePath = BaseDirectories::Shaders + filename;

		//const char* cstr = cString(filename);
		std::ifstream file;
		file.open(cString(filePath), std::ios::in | std::ios::binary);

		//FILE* file = fopen(cstr, "rb");
		if(!file.is_open())
		{
			showSimpleMessageBox("Shader file failed to open: " + filename);
			return {}; // TODO: make backup shader for when shaders fail to load
		}
		defer(file.close());

		String output;

		//const uSize_t bufferSize = 4096;
		//char lineBuffer[bufferSize];
		String line;

		while(file.good())
		{
			//line = Strings::trimSpace(lineBuffer);
			getline(file, line);
			line = Strings::trimSpace(line);

			Allocator& a = defaultAllocator();


			if(Strings::hasPrefix(line, "#include"))
			{
				String includeFilename = substring(line, 8, len(line));
				includeFilename = Strings::trimSpace(includeFilename);
				includeFilename = Strings::trim(includeFilename, "\"");

				append(output, shaderSourceFromFile(includeFilename));
			}
			else
			{
				append(output, line);
			}

			append(output, "\n");
		}

		return output;
	}

	//INTERNAL String stringFromFile(const String& filename) // function used to import strings from external files into code at compile
	//{
	//	std::ifstream file; // declare a file stream
	//	file.open(std::string(BaseDirectories::Shaders + filename.c_str()), std::ios::in | std::ios::binary); // tell to open file.c_str()(to be defined later)
	//																  // and to take input fromm the file
	//	std::string fileDirectory = FileSystem::getFileDirectory(filename) + "/";
	//
	//	std::string output; // declares string output
	//	std::string line; // declares string line
	//
	//	if (!file.is_open()) // check if there was an error finding/opening the file
	//	{
	//		throwRuntimeError(std::string("Failed to open File :") + filename);
	//	}
	//	else
	//	{
	//		while (file.good()) // while file is open
	//		{
	//			std::getline(file, line); // get a line from the file and define line as the string
	//
	//			if(line.find("#include") == std::string::npos)
	//			{
	//				output.append(line + "\n");
	//			}
	//			else
	//			{
	//				std::string includeFilename = split(line, ' ')[1];
	//
	//				// get path\name of #include file
	//				if(includeFilename[0] == '<') // absolute library path
	//				{
	//					size_t closingBracketPos = 0;
	//					for(size_t i = 1; i < includeFilename.length(); i++)
	//					{
	//						if(includeFilename[i] == '>')
	//						{
	//							closingBracketPos = i;
	//							break;
	//						}
	//					}
	//
	//					if(closingBracketPos > 1)
	//					{
	//						includeFilename = includeFilename.substr(1, closingBracketPos - 1);
	//					}
	//					else
	//					{
	//						includeFilename = "";
	//					}
	//				}
	//				else if (includeFilename[0] == '\"') // relative path
	//				{
	//					size_t closingSpeechMarkPos = 0;
	//					for(size_t i = 1; i < includeFilename.length(); i++)
	//					{
	//						if(includeFilename[i] == '\"')
	//						{
	//							closingSpeechMarkPos = i;
	//							break;
	//						}
	//					}
	//
	//					if(closingSpeechMarkPos > 1)
	//						includeFilename = includeFilename.substr(1, closingSpeechMarkPos - 1);
	//					else
	//						includeFilename = "";
	//
	//				}
	//
	//				if(includeFilename.length() > 0)
	//					output.append(stringFromFile(includeFilename) + "\n");
	//			}
	//
	//		}
	//	}
	//
	//	file.close();
	//	return output;
	//}
	//
	//

		ShaderProgram::ShaderProgram()
			: handle(0)
			, isLinked(false)
			, errorLog()
			, m_attribLocations(defaultAllocator())
			, m_uniformLocations(defaultAllocator())
		{

		}


		ShaderProgram::~ShaderProgram()
		{
			if (handle)
				glDeleteProgram(handle);
		}

		// add shaders here
		// get the shader type and file name for the shader
		bool ShaderProgram::attachShaderFromFile(ShaderType type, const String& filename)
		{
			const String source = shaderSourceFromFile(filename); // define source and the filename
			return attachShaderFromMemory(type, source);
		}

		bool ShaderProgram::attachShaderFromMemory(ShaderType type, const String& source)
		{
			// check if m_object got compiled
			if (!handle)
				handle = glCreateProgram();

			u32 shader = 0;
			if (type == ShaderType::Vertex) // check the shader type
				shader = glCreateShader(GL_VERTEX_SHADER); // create the shader
			else if (type == ShaderType::Fragment)
				shader = glCreateShader(GL_FRAGMENT_SHADER); // create the shader
			else
				throwRuntimeError("Shader Type Unknown - ShaderProgram::attachShaderFromMemory()");

			const char* cStrSource = cString(source);

			glShaderSource(shader, 1, &cStrSource, nullptr);
			glCompileShader(shader);

			// check for compile error
			s32 status;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // define status as the compile status
			if (status == GL_FALSE)
			{
				String msg;
				if(type == ShaderType::Vertex)
					msg = "Compile Error in Vertex shader. \n";
				else if(type == ShaderType::Fragment)
					msg = "Compile Error in Fragment Shader. \n";
				else
				{
					showSimpleMessageBox("Unknown Shader Type.", "ShaderProgram::attachShaderFromMemory()");
					//std::exit(EXIT_FAILURE); // TODO: check if showSimpleMessageBox error exits program.
				}

				s32 infoLogLength;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength); // get shader information i for integer and v for vector

				Allocator& a = defaultAllocator();

				char* strInfoLog = (char*)a.allocate((infoLogLength + 1) * sizeof(char));
				defer(a.deallocate(strInfoLog));

				glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog);

				append(msg, strInfoLog);
				append(msg, "\n");

				append(errorLog, msg);

				glDeleteShader(shader);

				return false;
			}

			glAttachShader(handle, shader);

			return true;
		}

		void ShaderProgram::use() const
		{
			if(!isInUse()) // check that it's not in use already
				glUseProgram(handle);
		}
		bool ShaderProgram::isInUse() const
		{
			s32 currentProgram = 0;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

			return (currentProgram == (s32)handle);
		}
		void ShaderProgram::stopUsing() const
		{
			if (isInUse())
				glUseProgram(0);
		}
		void ShaderProgram::checkInUse() const
		{
			if(!isInUse())
				throw showSimpleMessageBox("Shader not in use.", "ShaderProgram::checkInUse()");
		}

		bool ShaderProgram::link()
		{

			// check if m_object got compiled
			if (!handle)
				handle = glCreateProgram();


			if(!isLinked)
			{
				glLinkProgram(handle);

				s32 status; // check for link error
				glGetProgramiv(handle, GL_LINK_STATUS, &status);

				if (!status)
				{
					String msg("Shader program linking failure:\n");

					s32 infoLogLength;
					glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);

					Allocator& a = defaultAllocator();

					char* strInfoLog = (char*)a.allocate((infoLogLength + 1) * sizeof(char));
					defer(a.deallocate(strInfoLog));

					glGetProgramInfoLog(handle, infoLogLength, nullptr, strInfoLog);

					append(msg, strInfoLog);
					append(msg, "\n");

					append(errorLog, msg);

					glDeleteProgram(handle);
					handle = 0;

					isLinked = false;
					return isLinked;
				}

				isLinked = true;
			}

			return isLinked;
		}

		void ShaderProgram::bindAttribLocation(s32 location, const String& name)
		{
			glBindAttribLocation(handle, location, cString(name));
			set(m_attribLocations, murmurStringHash64(name), location);
			//m_attribLocations[name] = location;
		}
		s32 ShaderProgram::getAttribLocation(const String& name) const
		{
			const u64 hash = murmurStringHash64(name);
			//auto found = m_attribLocations.find(name); // find the name of attrib location

			if (has(m_attribLocations, hash)) // check if location was found
			{
				return get(m_attribLocations, hash, 0); // if not return found second position
			}
			s32 loc = glGetAttribLocation(handle, cString(name));
			set(m_attribLocations, hash, loc);

			return loc;
			
		}
		s32 ShaderProgram::getUniformLocation(const String& name) const
		{
			const u64 hash = murmurStringHash64(name);
			//auto found = m_uniformLocations.find(name);

			if (has(m_attribLocations, hash))
			{
				return get(m_attribLocations, hash, 0);
			}
			s32 loc = glGetUniformLocation(handle, cString(name));
			set(m_attribLocations, hash, loc);

			return loc;
		}

		void ShaderProgram::setUniform(const String& name, f32 x) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1) // check if getUniformLocation worked
				return;
			glUniform1f(loc, x);
		}
		void ShaderProgram::setUniform(const String& name, f32 x, f32 y) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform2f(loc, x, y);
		}
		void ShaderProgram::setUniform(const String& name, f32 x, f32 y, f32 z) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform3f(loc, x, y, z);
		}
		void ShaderProgram::setUniform(const String& name, f32 x, f32 y, f32 z, f32 w) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform4f(loc, x, y, z, w);
		}

		void ShaderProgram::setUniform(const String& name, u32 x) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, x);
		}
		void ShaderProgram::setUniform(const String& name, s32 x) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, (s32)x);
		}
		void ShaderProgram::setUniform(const String& name, bool x) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform1i(loc, static_cast<int>(x));
		}

		void ShaderProgram::setUniform(const String& name, const Vector2& v) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform2fv(loc, 1, &v.data[0]);

		}
		void ShaderProgram::setUniform(const String& name, const Vector3& v) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform3fv(loc, 1, &v.data[0]);
		}
		void ShaderProgram::setUniform(const String& name, const Vector4& v) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniform4fv(loc, 1, &v.data[0]);
		}
		void ShaderProgram::setUniform(const String& name, const Matrix4& m) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;
			glUniformMatrix4fv(loc, 1, GL_FALSE, &m.data[0].data[0]);
		}


		void ShaderProgram::setUniform(const String& name, const Quaternion& q) const
		{
			checkInUse();
			s32 loc = getUniformLocation(name);
			if (loc == -1)
				return;

			glUniform4fv(loc, 1, &q.data[0]);

		}

		void ShaderProgram::setUniform(const String& name, const Transform& t) const
		{
			checkInUse();

			{
				s32 loc = getUniformLocation(name + ".position");
				if(loc == -1)
					return;
				glUniform3fv(loc, 1, &t.position[0]);
			}
			{
				s32 loc = getUniformLocation(name + ".orientation");
				if (loc == -1)
					return;
				glUniform4fv(loc, 1, &t.orientation.x);
			}
			{
				s32 loc = getUniformLocation(name + ".scale");
				if (loc == -1)
					return;
				glUniform3fv(loc, 1, &t.scale[0]);
			}
		}

		void ShaderProgram::setUniform(const String& name, const Color& c) const
		{
			f32 r, g, b, a;
			r = c.r / 255.0f;
			g = c.g / 255.0f;
			b = c.b / 255.0f;
			a = c.a / 255.0f;

			setUniform(name, r, g, b, a);
		}

} // end Dunjun
