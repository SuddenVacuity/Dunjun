

#include <Dunjun/Game.hpp>

namespace Dunjun
{

	//struct ModelInstance // copies an asset to use
	//{
	//	ModelAsset* asset;
	//	Transform transform;
	//};
	GLOBAL Window g_window;

	namespace
	{
		GLOBAL const Time TIME_STEP = seconds(1.0f / 60.0f);
		GLOBAL const u32 FrameLimit = 60;
		GLOBAL bool g_running = true;
	} // end anon namespace

	GLOBAL World* g_world;

	// textures
	GLOBAL Texture g_defaultTexture;
	GLOBAL Texture g_dunjunTextTexture;
	GLOBAL Texture g_stoneTexture;
	GLOBAL Texture g_terrainTexture;

	GLOBAL Material g_dunjunTextMaterial;
	GLOBAL Material g_stoneMaterial;
	GLOBAL Material g_terrainMaterial;

	namespace Game
	{
		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					IMPORT CONFIG FILE
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		enum ConfigType
		{
			ConfigType_Unknown,

			ConfigType_Uint,
			ConfigType_Sint,
			ConfigType_Float,
			ConfigType_Bool,
			ConfigType_String,
		};

		struct ConfigData
		{
			Array<u32> uints;
			Array<s32> sints;
			Array<f32> floats;
			Array<b8> bools;

			uSize_t stringsLength;
			String strings[1024];

			// this tracks the data type and position in the array
			struct Variable
			{
				u32 index;
				ConfigType type;
			};

			HashMap<Variable> map;

			ConfigData();
			~ConfigData() = default;
		};

		ConfigData::ConfigData()
			: uints(defaultAllocator())
			, sints(defaultAllocator())
			, floats(defaultAllocator())
			, bools(defaultAllocator())
			, stringsLength(0)
			, strings()
			, map(defaultAllocator())
		{
		}

		ConfigData::Variable getConfigDataVariable(const ConfigData& data, const String& name)
		{
			return get(data.map, murmurStringHash64(name), {0, ConfigType::ConfigType_Unknown});
		}

		bool addToConfigFile_uint(ConfigData& data, const String& name, const String& value)
		{
			uSize_t i = 0;
			if(value[0] == '+')
				i = 1;

			for(/**/; i < len(value); i++)
			{
				if(!Strings::isNumeric(value[i]))
					return false;
			}

			if(has(data.map, murmurStringHash64(name)))
			{
				std::cerr << name << " already exists\n";
				return false;
			}
			
			u32 uint = atoi(cString(value));

			ConfigData::Variable var = {};
			var.type = ConfigType_Uint;
			var.index = 0;

			if(len(data.uints) > 0)
				var.index = len(data.uints) - 1;

			set(data.map, murmurStringHash64(name), var);
			append(data.uints, uint);

			return true;
		}

		bool addToConfigFile_sint(ConfigData& data, const String& name, const String& value)
		{
			uSize_t i = 0;
			if (value[0] == '+' || value[0] == '-')
				i = 1;

			for (/**/; i < len(value); i++)
			{
				if (!Strings::isNumeric(value[i]))
					return false;
			}

			if (has(data.map, murmurStringHash64(name)))
			{
				std::cerr << name << " already exists\n";
				return false;
			}


			s32 sint = atoi(cString(value));

			ConfigData::Variable var = {};
			var.type = ConfigType_Sint;
			var.index = 0;

			if (len(data.sints) > 0)
				var.index = len(data.sints) - 1;

			set(data.map, murmurStringHash64(name), var);
			append(data.sints, sint);

			return true;
		}

		bool addToConfigFile_float(ConfigData& data, const String& name, const String& value)
		{
			f32 f;

			if(sscanf_s(cString(value), "%f", &f) != 1)
				return false; // value is not a float

			if (has(data.map, murmurStringHash64(name)))
			{
				std::cerr << name << " already exists\n";
				return false;
			}

			ConfigData::Variable var = {};
			var.type = ConfigType_Float;
			var.index = 0;

			if (len(data.floats) > 0)
				var.index = len(data.floats) - 1;

			set(data.map, murmurStringHash64(name), var);
			append(data.floats, f);

			return true;
		}

		bool addToConfigFile_bool(ConfigData& data, const String& name, const String& value)
		{
			b8 b;

			if(value == '1' || Strings::toLowerCase(value) == "true")
				b = true;
			else if(value == '0' || Strings::toLowerCase(value) == "false")
				b = false;
			else
				return false; // add value failed

			if (has(data.map, murmurStringHash64(name)))
			{
				std::cerr << name << " already exists\n";
				return false;
			}

			ConfigData::Variable var = {};
			var.type = ConfigType_Bool;
			var.index = 0;

			if (len(data.bools) > 0)
				var.index = len(data.bools) - 1;

			set(data.map, murmurStringHash64(name), var);
			append(data.bools, b);

			return true;
		}

		bool addToConfigFile_string(ConfigData& data, const String& name, const String& value)
		{
			// check for quotation marks 
			// and that data is long enough to contain at least one char
			if(!(value[0] == '\"' && value[len(value) - 1] ==  '\"') || len(value) <= 2)
				return false; // add value failed

			if (has(data.map, murmurStringHash64(name)))
			{
				std::cerr << name << " already exists\n";
				return false;
			}

			ConfigData::Variable var = {};
			var.type = ConfigType_String;
			var.index = data.stringsLength;

			set(data.map, murmurStringHash64(name), var);

			// start +1 and end -1 to remove quotation marks
			data.strings[var.index] = substring(value, 1, len(value) - 1);
			data.stringsLength++;

			return true;
		}

		INTERNAL void configTest()
		{
			String filepath = "data/defaultSettings.op";

			std::fstream file;
			file.open(cString(filepath), std::ios::in | std::ios::binary);
			defer(file.close());

			if(!file.is_open())
				showSimpleMessageBox("config file failed to open: " + filepath);

			ConfigData configData = {};

			uSize_t lineNum = 0;
			String line;
			while(file.good())
			{
				defer(lineNum++);

				getline(file, line);
				line = Strings::trimSpace(line);

				uSize_t lineLen = len(line);

				// check for a comment at the beginning of the line
				if(line[0] == '/' && line[1] == '/')
					lineLen = 0;

				// if line is empty continue
				if(lineLen == 0)
					continue;

				sSize_t definitionPos = -1; // value
				sSize_t declarationPos = -1; // name

				// read the line
				for(sSize_t i = 0; i < lineLen; i++)
				{
					// check for comments
					if (line[i] == '/' && i != lineLen)
					{
						if(line[i + 1] == '/')
						{
							// set lineLen to ignore all comment text
							lineLen = i;
							break;
						}
					}


					// check for tokens
					if(line[i] == ':' && declarationPos == -1)
						declarationPos = i;
					else if(line[i] == '=' && definitionPos == -1)
						definitionPos = i;
				}

				// check if tokens were found
				if(declarationPos == -1 || definitionPos == -1)
				{
					showSimpleMessageBox("Parsing Error in config file:\n" + 
										 filepath + "\n\n" + line, 
										 "Parsing Error", MessageBoxType::Warning);
					continue;
				}

				// seperate each portion into seperate strings
				String type =  substring(line, 0, declarationPos);
				String name =  substring(line, declarationPos + 1, definitionPos);
				String value = substring(line, definitionPos + 1, lineLen);

				// remove whitespace
				type  = Strings::toLowerCase(Strings::trimSpace(type));
				name  = Strings::trimSpace(name);
				value = Strings::trimSpace(value);

				bool succeeded;

				if (type == "u_int")
					succeeded = addToConfigFile_uint(configData, name, value);
				else if (type == "s_int")
					succeeded = addToConfigFile_sint(configData, name, value);
				else if (type == "float")
					succeeded = addToConfigFile_float(configData, name, value);
				else if (type == "bool")
					succeeded = addToConfigFile_bool(configData, name, value);
				else if (type == "string")
					succeeded = addToConfigFile_string(configData, name, value);
				else
				{
					succeeded = false;
					showSimpleMessageBox("Unknown data type in config file:\n" + 
										 filepath + "\n\n" + line, 
										 "Unknown Type", MessageBoxType::Warning);
					continue;
				}

				if(succeeded == false)
					continue;
				
				// get data through its hash
				ConfigData::Variable v = getConfigDataVariable(configData, name);
				switch(v.type)
				{
				default: std::cout << "Unlisted value - Should never get here\n"; break;
				case ConfigType::ConfigType_Unknown: std::cout << "Unknown Type - Should never get here\n"; break;
				case ConfigType::ConfigType_Uint:	 std::cout << type + "  : "  + name + " " << configData.uints[v.index] << std::endl;   break;
				case ConfigType::ConfigType_Sint:	 std::cout << type + "  : "  + name + " " << configData.sints[v.index] << std::endl;   break;
				case ConfigType::ConfigType_Float:	 std::cout << type + "  : "  + name + " " << configData.floats[v.index] << std::endl;  break;
				case ConfigType::ConfigType_Bool:	 std::cout << type + "   : " + name + " " << configData.bools[v.index] << std::endl;   break;
				case ConfigType::ConfigType_String:	 std::cout << type + " : "   + name + " " << configData.strings[v.index] << std::endl; break;
				}
				
			} // end while(file.good())

			// confirm data was added
			std::cout << "\n\nOutput all data\n";
			const char* boolNames[] = { "false", "true" };

			std::cout << "\nuints:\n";
			for (u32 u : configData.uints)
				std::cout << u << "\n";

			std::cout << "\nsints:\n";
			for (s32 s : configData.sints)
				std::cout << s << "\n";

			std::cout << "\nfloats:\n";
			for (f32 f : configData.floats)
				std::cout << f << "\n";

			std::cout << "\nbools:\n";
			for (b8 b : configData.bools)
				std::cout << boolNames[b] << "\n";

			std::cout << "\nstrings:\n";
			for (u32 i = 0; i < configData.stringsLength; i++)
				std::cout << cString(configData.strings[i]) << "\n";

			STOP

		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					.
		)
		)				OpenGL INIT
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		void glInit()
		{
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					LOAD ASSETS
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		// File path for shader files and define and bind attributes
		INTERNAL void loadShaders()
		{
			u32 shaderCounter = 0;
			std::cout << "Loading shader " << shaderCounter++;
			g_shaderHolder.insertFromFile("default", "default_vert.glsl", 
													 "default_frag.glsl");
			std::cout << " " << shaderCounter++;
			g_shaderHolder.insertFromFile("texturePass", "texPass_vert.glsl", 
														 "texPass_frag.glsl");
			std::cout << " " << shaderCounter++;
			g_shaderHolder.insertFromFile("deferredGeometryPass", "deferredGeometryPass_vert.glsl", 
																  "deferredGeometryPass_frag.glsl");
			std::cout << " " << shaderCounter++;
			g_shaderHolder.insertFromFile("deferredDirectionalLight", "deferredLightPass_vert.glsl",
																	  "deferredDirectionalLightPass_frag.glsl");
			std::cout << " " << shaderCounter++;
			g_shaderHolder.insertFromFile("deferredAmbientLight", "deferredLightPass_vert.glsl", 
																  "deferredAmbientLightPass_frag.glsl");
			std::cout << " " << shaderCounter++;
			g_shaderHolder.insertFromFile("deferredPointLight", "deferredLightPass_vert.glsl",
																"deferredPointLightPass_frag.glsl");
			std::cout << " " << shaderCounter++;
			g_shaderHolder.insertFromFile("deferredSpotLight", "deferredLightPass_vert.glsl",
															   "deferredSpotLightPass_frag.glsl");



			std::cout << " " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("deferredFinalPass", "deferredLightPass_vert.glsl",
															   "deferredFinalPass_frag.glsl");
		}

		INTERNAL void loadMaterials()
		{
			// load textures
			g_defaultTexture = loadTextureFromFile("data/textures/default.jpg");
			g_dunjunTextTexture = loadTextureFromFile("data/textures/dunjunText.jpg");
			g_stoneTexture = loadTextureFromFile("data/textures/stone.png");
			g_terrainTexture = loadTextureFromFile("data/textures/terrain.png", TextureFilter::Nearest);


			g_dunjunTextMaterial = Material{};
			g_dunjunTextMaterial.diffuseMap = &g_dunjunTextTexture;

			g_stoneMaterial = Material{};
			g_stoneMaterial.diffuseMap = &g_stoneTexture;

			g_terrainMaterial = Material{};
			g_terrainMaterial.diffuseMap = &g_terrainTexture;

		}

		// sprite vertex info, vbo and ibo
		INTERNAL void loadSpriteAsset()
		{

			// Quad
			{
				// Here is where you add vertice information
				//
				Vertex vertices[] = { // define vertexes for a triangle
									  //  x	    y	  z		  s	    t	       r	 g	   b	 a		normals				// for triangle strips organize vertexes in a backwards Z
					{ { +1.0f,  1.0f, 0.0f },{ 1.0f, 1.0f },{ 0xFF,0xFF,0xFF,0xFF },{ 0, 0, 0 } },	// 0 vertex         1 ---- 0        
					{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 1.0f },{ 0xFF,0xFF,0xFF,0xFF },{ 0, 0, 0 } },	// 1 vertex           \             
					{ { +1.0f, -1.0f, 0.0f },{ 1.0f, 0.0f },{ 0xFF,0xFF,0xFF,0xFF },{ 0, 0, 0 } },	// 2 vertex              \           
					{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f },{ 0xFF,0xFF,0xFF,0xFF },{ 0, 0, 0 } },	// 3 vertex         3 -----2       
				};

				u32 indices[] = { 0, 1, 2, 1, 3, 2 }; // vertex draw order for GL_TRIANGLES

													  // get number of entries
				u32 numVertices = sizeof(vertices) / sizeof(vertices[0]);
				u32 numIndices = sizeof(indices) / sizeof(indices[0]);

				MeshData meshData = { defaultAllocator() };
				meshData.drawType = DrawType::Triangles;

				reserve(meshData.vertices, numVertices);
				reserve(meshData.indices, numIndices);

				// add the data
				for (u32 i = 0; i < numVertices; i++)
					append(meshData.vertices, vertices[i]);

				for (u32 i = 0; i < numIndices; i++)
					append(meshData.indices, indices[i]);

				meshData.generateNormals();

				// TODO:FIXME: seemingly random access violations happenjl here
				g_meshHolder.insert("quad", std::unique_ptr<Mesh>(new Mesh(generateMesh(meshData))));
			}

			// Sprite
			{
				// Here is where you add vertice information
				//
				Vertex vertices[] = { // define vertexes for a triangle
					//  x	    y	  z		  s	    t	       r	 g	   b	 a		normals				// for triangle strips organize vertexes in a backwards Z
					{ { +0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f },{ 0xFF,0xFF,0xFF,0xFF }, { 0, 0, 0 } },	// 0 vertex         1 ---- 0        
					{ { -0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f },{ 0xFF,0xFF,0xFF,0xFF }, { 0, 0, 0 } },	// 1 vertex           \             
					{ { +0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f },{ 0xFF,0xFF,0xFF,0xFF }, { 0, 0, 0 } },	// 2 vertex              \           
					{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f },{ 0xFF,0xFF,0xFF,0xFF }, { 0, 0, 0 } },	// 3 vertex         3 -----2       
				};
			
				u32 indices[] = { 0, 1, 2, 1, 3, 2 }; // vertex draw order for GL_TRIANGLES
			
				// get number of entries
				u32 numVertices = sizeof(vertices) / sizeof(vertices[0]);
				u32 numIndices = sizeof(indices) / sizeof(indices[0]);

				MeshData meshData = { defaultAllocator() };
				meshData.drawType = DrawType::Triangles;
			
				reserve(meshData.vertices, numVertices);
				reserve(meshData.indices, numIndices);
			
				// add the data
				for(u32 i = 0; i < numVertices; i++)
					append(meshData.vertices, vertices[i]);
			
				for(u32 i = 0; i <  numIndices; i++)
					append(meshData.indices, indices[i]);
			
				meshData.generateNormals();
			
				g_meshHolder.insert("sprite", std::unique_ptr<Mesh>(new Mesh(generateMesh(meshData))));
			}

			// Walls
			{
				// Here is where you add vertice information
				//
				Vertex vertices[] = { // define vertexes for a triangle
									  //  x	    y	  z		  s	    t	       r	 g	   b	 a		normals				// for triangle strips organize vertexes in a backwards Z
					{ { +0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f },{ 0xFF,0xFF,0xFF,0xFF },{ 0, 0, 0 } },	// 0 vertex         1 ---- 0        
					{ { -0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f },{ 0xFF,0xFF,0xFF,0xFF },{ 0, 0, 0 } },	// 1 vertex           \             
					{ { +0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f },{ 0xFF,0xFF,0xFF,0xFF },{ 0, 0, 0 } },	// 2 vertex              \           
					{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f },{ 0xFF,0xFF,0xFF,0xFF },{ 0, 0, 0 } },	// 3 vertex         3 -----2       
				};

				u32 indices[] = { 0, 1, 2, 1, 3, 2 }; // vertex draw order for GL_TRIANGLES

													  // get number of entries
				u32 numVertices = sizeof(vertices) / sizeof(vertices[0]);
				u32 numIndices = sizeof(indices) / sizeof(indices[0]);

				MeshData meshData = { defaultAllocator() };
				meshData.drawType = DrawType::Triangles;

				reserve(meshData.vertices, numVertices);
				reserve(meshData.indices, numIndices);

				// add the data
				for (u32 i = 0; i < numVertices; i++)
					append(meshData.vertices, vertices[i]);

				for (u32 i = 0; i < numIndices; i++)
					append(meshData.indices, indices[i]);

				meshData.generateNormals();

				g_meshHolder.insert("surface", std::unique_ptr<Mesh>(new Mesh(generateMesh(meshData))));

			}
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					.
		)
		)				HANDLE INPUT
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		INTERNAL void handleRealtimeInput()
		{
			Event event;
			while (g_window.pollEvent(event))
			{
				switch (event.type)
				{
				default: break;
				case Event::Closed:
				{
					g_running = false;
					break;
				}
				case Event::Resized:
				{
					glViewport(0, 0, event.size.width, event.size.height);
					break;
				}

				/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
				)				.
				)					HANDLE KEYBOARD
				)
				%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

				case Event::KeyPressed:// && acceptInput == true
				{
					if (Input::isKeyPressed(Input::Key::Escape))
					{
						g_running = false;
						break;
					}
				//	// console
				//	if (g_world->useConsole == true)
				//	{
				//		// exit console
				//		if (Input::isKeyPressed(Input::Key::Tab))
				//		{
				//			g_world->useConsole = false;
				//		}
				//
				//		// check for a command
				//		if (Input::isKeyPressed(Input::Key::Return))
				//			g_world->checkForCommand = true;
				//
				//		if (Input::isKeyPressed(Input::Key::Space))
				//			g_world->consoleBuffer.append(" ");
				//
				//		// Erase letters
				//		if (Input::isKeyPressed(Input::Key::Backspace))
				//			if (g_world->consoleText.size() > 0)
				//				g_world->consoleText.erase(g_world->consoleText.size() - 1);
				//
				//		if (Input::isKeyPressed(Input::Key::Delete))
				//		{
				//			g_world->consoleText.clear();
				//			g_world->consoleBuffer.clear();
				//		}
				//
				//		// TODO: press up or down to cycle previous commands
				//		if (Input::isKeyPressed(Input::Key::Up))
				//			g_world->consoleText.append(" ");
				//		if (Input::isKeyPressed(Input::Key::Down))
				//			g_world->consoleText.append(" ");
				//
				//		//////////////////////////////////
				//		//								//
				//		//		   ADD LETTERS			//
				//		//								//
				//		//////////////////////////////////
				//
				//		for (int i = 0; i < 26; i++)
				//		{
				//			std::string s = ""; // can't initialize as i or cast in .append()
				//
				//			if (event.key.capsLock == true || event.key.shift == true)
				//				s = i + 65; // capital letters
				//			else
				//				s = i + 97; // lower case letters
				//
				//			if (Input::isKeyPressed(Input::Key(i)))
				//				if ((g_world->consoleBuffer.find(s) == g_world->consoleBuffer.npos)) // only add if letter is not already in buffer
				//					g_world->consoleBuffer.append(s);
				//		}
				//
				//		if (1)
				//		{
				//			// TODO: make this only happen when a key is released and while no keys are pressed
				//			// SDL UP/DOWN events don't seem to work for this
				//			// buffer is added twice when this happens here
				//			g_world->consoleText.append(g_world->consoleBuffer);
				//			g_world->consoleBuffer.clear();
				//		}
				//
				//		std::cout << "\n\nType in a command and press enter. [HELP] [QUIT]" << std::endl;
				//		std::cout << ">> [" << g_world->consoleText << "." << g_world->consoleBuffer << "]" << std::endl;
				//
				//	}
				//	// normal input
				//	else
				//	{
				//		if (Input::isKeyPressed(Input::Key::Tab))
				//			g_world->useConsole = true;
				//	}
				//	break;
				}

				/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
				)				.
				)					HANDLE GAMEPADS
				)
				%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

				case Event::GamepadAdded:
				{
					// cout from window.cpp
					break;
				}
				case Event::GamepadRemoved:
				{
					// cout from window.cpp
					break;
				}
				case Event::GamepadRemapped:
				{
					// cout from window.cpp
					break;
				}
				} // end switch(event.type)

				g_world->handleEvent(event);
			} // end while (g_window.pollEvent(event))

			  /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			  )				.
			  )					HANDLE CLOSE WINDOW
			  )
			  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

			if (!g_window.isOpen() || // check if window was closed
				Input::isKeyPressed(Input::Key::Escape)) // checks if the escape key is pressed in window
			{
				g_running = false;
			}

			if (Input::isKeyPressed(Input::Key::F11)) // press F11 to toggle between default and fullscreen
			{
				// TODO: Fullscreen
				glInit();
			}

			if(Input::isKeyPressed(Input::Key::D))
			{
				Vector3 f = forwardVector(g_world->camera.transform.orientation);

				s32 angle = f.y * 90;

				std::string direction;

				if (f.x > 0) // right side
				{
					if (f.z > 0) // right-bottom quadrant
					{
						if (f.x > f.z)
							direction = "East";
						else
							direction = "South";
					}
					else // right-top quadrant
					{
						if (f.x > -f.z)
							direction = "East";
						else
							direction = "North";
					}
				}
				else // left side
				{
					if (f.z > 0) // left-bottom quadrant
					{
						if (-f.x > f.z)
							direction = "West";
						else
							direction = "South";
					}
					else // left-top quadrant
					{
						if (f.z > f.x)
							direction = "West";
						else
							direction = "North";
					}
				}

				std::cout << "You are facing " << direction << " and looking " << angle << " degrees vertically." << std::endl;
			}
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					.
		)
		)				UPDATE
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		INTERNAL void update(Time dt)
		{
			g_world->update(dt);

			SceneGraph& sg = g_world->sceneGraph;
			SceneGraph::NodeId crateNode = sg.getNodeId(g_world->crate);
			SceneGraph::NodeId playerNode = sg.getNodeId(g_world->player);

			f32 wt = 1.0f * Time::now().asSeconds();
			f32 a = 2.0f;

			Transform pos = sg.getLocalTransform(crateNode);
			pos.position.y = a * Math::sin(Radian(wt));
			pos.scale = Vector3{ 1.2f, 1.2f, 1.2f } + Vector3{ 0.4f, 0.4f, 0.4f } * Math::sin(Radian(wt));
			pos.orientation = offsetOrientation(pos.orientation,
												Degree(360.0f * dt.asSeconds()),
												Degree(10.0f * dt.asSeconds()));

			sg.setLocalTransform(crateNode, pos);

			Transform pos2 = sg.getLocalTransform(playerNode);
			pos2.position.x = 1.0f * a * Math::cos(Radian(wt));
			sg.setLocalTransform(playerNode, pos2);

			std::cout << "";
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					.
		)
		)				RENDER
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		INTERNAL void render()
		{
			g_world->renderSystem.fbSize.x = g_window.getSize().x;
			g_world->renderSystem.fbSize.y = g_window.getSize().y;

			glViewport(0, 0, g_window.getSize().x, g_window.getSize().y);
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			g_world->render();
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					INIT
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		INTERNAL void initWorld()
		{
			EntitySystem& es = g_world->entitySystem;
			SceneGraph& sg = g_world->sceneGraph;
			RenderSystem& rs = g_world->renderSystem;

			//sg.allocate(16);
			//rs.allocate(16);

			EntityId crate = g_world->crate = es.addEntity(ComponentName | ComponentRender);
			EntityId player = g_world->player = es.addEntity(ComponentName | ComponentRender);
			EntityId floor = es.addEntity(ComponentName | ComponentRender);
			EntityId wall1 = es.addEntity(ComponentName | ComponentRender);
			EntityId wall2 = es.addEntity(ComponentName | ComponentRender);
			EntityId wall3 = es.addEntity(ComponentName | ComponentRender);

			g_world->names[crate] = NameComponent{"crate"};
			g_world->names[player] = NameComponent{"Bob"};

			Transform crateTransform = Transform::Identity;

			crateTransform.position = {0, 0, 0};
			crateTransform.orientation = angleAxis(Degree(-90), { 1, 0, 0 });
			crateTransform.scale = {1, 1, 1};

			sg.addNode(crate, crateTransform);
			sg.addNode(player, Transform::Identity);

			sg.linkNodes(crate, player);

			{
				Transform wallTransform = Transform::Identity;

				wallTransform.position = {0, 0, -4};
				wallTransform.scale = {8, 8, 8};
				sg.addNode(wall1, wallTransform);
			
				wallTransform.position = { -4, 0, 0};
				wallTransform.orientation = angleAxis(Degree(90), {0, 1, 0});
				sg.addNode(wall2, wallTransform);
			
				wallTransform.position = {4, 0, 0};
				wallTransform.orientation = angleAxis(Degree(-90), {0, 1, 0});
				sg.addNode(wall3, wallTransform);

				wallTransform.position = { 0, -4, 0 };
				wallTransform.orientation = angleAxis(Degree(-90), { 1, 0, 0 });
				sg.addNode(floor, wallTransform);
			
			
			}
			{
				rs.addComponent(crate, { g_meshHolder.get("sprite"), g_dunjunTextMaterial });
				rs.addComponent(player, { g_meshHolder.get("sprite"), g_dunjunTextMaterial });
				rs.addComponent(floor, { g_meshHolder.get("surface"), g_stoneMaterial });
				rs.addComponent(wall1, { g_meshHolder.get("surface"), g_stoneMaterial });
				rs.addComponent(wall2, { g_meshHolder.get("surface"), g_stoneMaterial });
				rs.addComponent(wall3, { g_meshHolder.get("surface"), g_stoneMaterial });
			}
			{
				DirectionalLight light;

				light.direction = { -0.0f, -1.0f, -0.2f };
				light.color = ColorLib::Orange;
				light.intensity = 1.0f;
				light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				light.brightness = ColorLib::calculateBrightness(light.colorIntensity);
				append(rs.directionalLights, light);
			}
			{
				PointLight light;

				light.color = ColorLib::Blue;
				light.position = Vector3{0, 0, -10.5f};
				light.intensity = 100.0f;
				light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				light.brightness = ColorLib::calculateBrightness(light.colorIntensity);
				light.range = calculateLightRange(light.intensity, light.color, light.attenuation);
				append(rs.pointLights, light);
			}

		}


		void init()
		{
			Memory::init();

			configTest();

			std::cout << "\n\n\n";

			if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | 
						SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK) != 0)
			{
				std::cerr << "SDL Failed to initialize. Error: ";
				std::cerr << SDL_GetError;
				std::cerr << std::endl;

				std::exit(EXIT_FAILURE);
			}

			g_window.create("Loading...", {854, 480});
			g_window.setFramerateLimit(FrameLimit);

			glewInit();

			// Initial OpenGL settings
			glInit();

			// initialize input
			Input::setUp();
			Input::setCursorPosition({ 0, 0 });

			// load assets
			loadShaders();
			loadMaterials();
			loadSpriteAsset();

			g_world = defaultAllocator().makeNew<World>();
			initWorld();

			//g_world = defaultAllocator().makeNew<World>();

			// pass context to world
			//g_world->init(Context{ g_window,
			//					 // g_textureHolder,
			//					  g_shaderHolder,
			//					  g_meshHolder,
			//					  g_materialHolder });
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					RUN
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		void run()
		{
			std::stringstream titleStream;

			//=================================================================================================
			// OPENING THE MAIN WINDOW							 
			//=================================================================================================
			//=================================================================================================

			const Time tickLimit = seconds(1.0 / (2.0f * 144.0f + 1.0f));

			TickCounter tc;
			//Clock frameClock;

			Time accumulator;
			Time prevTime = Time::now();

			std::cout << "\n\n\n\n" << std::endl;
			std::cout << "Press Tab to open the Console." << std::endl;

			while (g_running) // create a loop that works until the window closes
			{
				//Window::pollEvents();
				//Window::makeContextCurrent();

				Time currentTime = Time::now();
				Time dt = currentTime - prevTime;

				prevTime = currentTime;
				accumulator += dt;

				// limit accumulator size
				if (accumulator >= milliseconds(1200))
					accumulator = milliseconds(1200);

				// render update
				while (accumulator >= TIME_STEP)
				{
					accumulator -= TIME_STEP;
					handleRealtimeInput(); // input handler
					update(TIME_STEP);
				}

				if (tc.update(milliseconds(500)) && 
				   (g_running == true))
				{
					// dynamic window title
					g_window.setTitle(stringFormat("Dunjun - %.3f ms - %d fps",
												   1000.f / tc.tickRate, 
												   (u32)tc.tickRate).c_str());
				}

				render();
				g_window.display();


				// framerate limiter
				//const Time framelimitTime = MaxFrameTime - frameClock.getElapsedTime();
				//
				//if (framelimitTime > Time::Zero)
				//	Time::sleep(framelimitTime);
				//frameClock.restart();

			} // end while(g_running)
		}

		void cleanUp()
		{
			//defaultAllocator().makeDelete<World>(g_world);
			defaultAllocator().makeDelete(g_world);
			Input::cleanup();
			g_window.close();
			SDL_Quit();
			Memory::shutdown();

			std::exit(EXIT_SUCCESS);
		}

	} // end Game
} // end Dunjun
