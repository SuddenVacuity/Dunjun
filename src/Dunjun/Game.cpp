

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

	GLOBAL World g_world;

	GLOBAL bool toggleCulling = true;

	INTERNAL std::string consoleText = "";
	INTERNAL std::string consoleBuffer = "";
	INTERNAL bool acceptInput = true;
	INTERNAL bool checkForCommand = false;
	INTERNAL bool useConsole = false;

	namespace Game
	{
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
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
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
			std::cout << "Loading shader " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("default", "default_vert.glsl", 
													 "default_frag.glsl");
			std::cout << "Loading shader " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("texturePass", "texPass_vert.glsl", 
														 "texPass_frag.glsl");
			std::cout << "Loading shader " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("deferredGeometryPass", "deferredGeometryPass_vert.glsl", 
																  "deferredGeometryPass_frag.glsl");
			std::cout << "Loading shader " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("deferredDirectionalLight", "deferredLightPass_vert.glsl",
																	  "deferredDirectionalLightPass_frag.glsl");
			std::cout << "Loading shader " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("deferredAmbientLight", "deferredLightPass_vert.glsl", 
																  "deferredAmbientLightPass_frag.glsl");
			std::cout << "Loading shader " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("deferredPointLight", "deferredLightPass_vert.glsl",
																"deferredPointLightPass_frag.glsl");
			std::cout << "Loading shader " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("deferredSpotLight", "deferredLightPass_vert.glsl",
															   "deferredSpotLightPass_frag.glsl");



			std::cout << "Loading shader " << shaderCounter++ << std::endl;
			g_shaderHolder.insertFromFile("deferredFinalPass", "deferredLightPass_vert.glsl",
															   "deferredFinalPass_frag.glsl");
		}

		INTERNAL void loadMaterials()
		{
			// load textures
			g_textureHolder.insertFromFile("default", "dunjunText.jpg");
			g_textureHolder.insertFromFile("dunjunText", "dunjunText.jpg");
			g_textureHolder.insertFromFile("stone", "stone.png");
			g_textureHolder.insertFromFile("terrain", "terrain.png", TextureFilter::Nearest);

			// load materials
			{
				auto mat = make_unique<Material>();
				mat->shaders = &g_shaderHolder.get("deferredGeometryPass");
				mat->diffuseMap = &g_textureHolder.get("default");
				g_materialHolder.insert("default", std::move(mat));
			}
			{
				auto mat = make_unique<Material>();
				mat->shaders = &g_shaderHolder.get("deferredGeometryPass");
				mat->diffuseMap = &g_textureHolder.get("dunjunText");
				mat->specularExponent = 100000.0f;
				g_materialHolder.insert("dunjunText", std::move(mat));
			}
			{
				auto mat = make_unique<Material>();
				mat->shaders = &g_shaderHolder.get("deferredGeometryPass");
				mat->diffuseMap = &g_textureHolder.get("stone");
				g_materialHolder.insert("stone", std::move(mat));
			}
			{
				auto mat = make_unique<Material>();
				mat->shaders = &g_shaderHolder.get("deferredGeometryPass");
				mat->diffuseMap = &g_textureHolder.get("terrain");
				g_materialHolder.insert("terrain", std::move(mat));
			}
		}

		// sprite vertex info, vbo and ibo
		INTERNAL void loadSpriteAsset()
		{
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

				Mesh::Data meshData;

				meshData.vertices.reserve(numVertices);
				meshData.indices.reserve(numIndices);

				// add the data
				for(u32 i = 0; i < numVertices; i++)
					meshData.vertices.append(vertices[i].position, vertices[i].texCoord, vertices[i].color);

				for(u32 i = 0; i <  numIndices; i++)
					meshData.indices.push_back(indices[i]);

				meshData.generateNormals();

				g_meshHolder.insert("player", make_unique<Mesh>(meshData));
				//g_meshes["sprite"] = new Mesh(meshData);
			}

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

				Mesh::Data meshData;

				meshData.vertices.reserve(numVertices);
				meshData.indices.reserve(numIndices);

				// add the data
				for (u32 i = 0; i < numVertices; i++)
					meshData.vertices.append(vertices[i].position, vertices[i].texCoord, vertices[i].color);

				for (u32 i = 0; i < numIndices; i++)
					meshData.indices.push_back(indices[i]);

				meshData.generateNormals();

				g_meshHolder.insert("quad", make_unique<Mesh>(meshData));
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
					// TODO: fix inconsistant delay/ignore for this event
					g_running = false;
					g_window.close();
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
					// console
					if (useConsole == true)
					{
						// exit console
						if (Input::isKeyPressed(Input::Key::Tab))
						{
							useConsole = false;
						}

						// check for a command
						if (Input::isKeyPressed(Input::Key::Return))
						{
							checkForCommand = true;
						}

						// Erase letters
						if (Input::isKeyPressed(Input::Key::Backspace))
							if (consoleText.size() > 0)
								consoleText.erase(consoleText.size() - 1);

						if (Input::isKeyPressed(Input::Key::Delete))
						{
							consoleText.clear();
							consoleBuffer.clear();
						}

						// TODO: press up or down to cycle previous commands
						if (Input::isKeyPressed(Input::Key::Up))
							consoleText.append(" ");
						if (Input::isKeyPressed(Input::Key::Down))
							consoleText.append(" ");

						// add letters
						for (int i = 0; i < 26; i++)
						{
							std::string s = ""; // can't initialize as i or cast in .append()

							if (event.key.capsLock == true || event.key.shift == true)
								s = i + 65; // capital letters
							else
								s = i + 97; // lower case letters

							if (Input::isKeyPressed(Input::Key(i)))
								if ((consoleBuffer.find(s) == consoleBuffer.npos)) // only add if letter is not already in buffer
									consoleBuffer.append(s);
						}

						if (1)
						{
							// TODO: make this only happen when a key is released and while no keys are pressed
							// SDL UP/DOWN events don't seem to work for this
							// buffer is added twice when this happens here
							consoleText.append(consoleBuffer);
							consoleBuffer.clear();
						}

						std::cout << "\n\nType in a command and press enter. [HELP] [QUIT]" << std::endl;
						std::cout << ">> [" << consoleText << "." << consoleBuffer << "]" << std::endl;

					}
					// normal input
					else
					{
						if (Input::isKeyPressed(Input::Key::Tab))
							useConsole = true;
					}
					break;
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

				g_world.handleEvent(event);
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
				g_window.close();
			}

			if (Input::isKeyPressed(Input::Key::F11)) // press F11 to toggle between default and fullscreen
			{
				// TODO: Fullscreen
				glInit();
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
			g_world.update(dt);
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
			g_world.render();
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

		void init()
		{

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

			// pass context to world
			g_world.init(Context{ g_window,
								  g_textureHolder,
								  g_shaderHolder,
								  g_meshHolder,
								  g_materialHolder });
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
			std::cout << "Type in a command and press enter. [HELP] [QUIT]" << std::endl;
			std::cout << ">> []" << std::endl;

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

				if (tc.update(milliseconds(500)) && (g_running == true))
				{
					// dynamic window title
					g_window.setTitle(stringFormat("Dunjun - %.3f ms - %d fps",
												   1000.f / tc.getTickRate(), 
												   (u32)tc.getTickRate()
												  ).c_str());
				}

				if(g_running)
					render();


				// framerate limiter
				//const Time framelimitTime = MaxFrameTime - frameClock.getElapsedTime();
				//
				//if (framelimitTime > Time::Zero)
				//	Time::sleep(framelimitTime);
				//frameClock.restart();

			}
		}

		void cleanUp()
		{
			Input::cleanup();
		}

	} // end Game
} // end Dunjun
