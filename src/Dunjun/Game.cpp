

#include <Dunjun/Game.hpp>
#include <Dunjun/Entity.hpp>
#include <Dunjun/SceneGraph.hpp>


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

			// load materials
			{
				auto mat = make_unique<Material>();
				mat->shaders = &g_shaderHolder.get("deferredGeometryPass");
				mat->diffuseMap = &g_defaultTexture;
				g_materialHolder.insert("default", std::move(mat));
			}
			{
				auto mat = make_unique<Material>();
				mat->shaders = &g_shaderHolder.get("deferredGeometryPass");
				mat->diffuseMap = &g_dunjunTextTexture;
				mat->specularExponent = 100000.0f;
				g_materialHolder.insert("dunjunText", std::move(mat));
			}
			{
				auto mat = make_unique<Material>();
				mat->shaders = &g_shaderHolder.get("deferredGeometryPass");
				mat->diffuseMap = &g_stoneTexture;
				g_materialHolder.insert("stone", std::move(mat));
			}
			{
				auto mat = make_unique<Material>();
				mat->shaders = &g_shaderHolder.get("deferredGeometryPass");
				mat->diffuseMap = &g_terrainTexture;
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

				Mesh::Data meshData = {};

				reserve(meshData.vertices, numVertices);
				reserve(meshData.indices, numIndices);

				// add the data
				for(u32 i = 0; i < numVertices; i++)
					append(meshData.vertices, vertices[i]);

				for(u32 i = 0; i <  numIndices; i++)
					append(meshData.indices, indices[i]);

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

				Mesh::Data meshData = {};

				reserve(meshData.vertices, numVertices);
				reserve(meshData.indices, numIndices);

				// add the data
				for (u32 i = 0; i < numVertices; i++)
					append(meshData.vertices, vertices[i]);

				for (u32 i = 0; i < numIndices; i++)
					append(meshData.indices, indices[i]);

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
					if (g_world->useConsole == true)
					{
						// exit console
						if (Input::isKeyPressed(Input::Key::Tab))
						{
							g_world->useConsole = false;
						}

						// check for a command
						if (Input::isKeyPressed(Input::Key::Return))
							g_world->checkForCommand = true;

						if (Input::isKeyPressed(Input::Key::Space))
							g_world->consoleBuffer.append(" ");

						// Erase letters
						if (Input::isKeyPressed(Input::Key::Backspace))
							if (g_world->consoleText.size() > 0)
								g_world->consoleText.erase(g_world->consoleText.size() - 1);

						if (Input::isKeyPressed(Input::Key::Delete))
						{
							g_world->consoleText.clear();
							g_world->consoleBuffer.clear();
						}

						// TODO: press up or down to cycle previous commands
						if (Input::isKeyPressed(Input::Key::Up))
							g_world->consoleText.append(" ");
						if (Input::isKeyPressed(Input::Key::Down))
							g_world->consoleText.append(" ");

						//////////////////////////////////
						//								//
						//		   ADD LETTERS			//
						//								//
						//////////////////////////////////

						for (int i = 0; i < 26; i++)
						{
							std::string s = ""; // can't initialize as i or cast in .append()

							if (event.key.capsLock == true || event.key.shift == true)
								s = i + 65; // capital letters
							else
								s = i + 97; // lower case letters

							if (Input::isKeyPressed(Input::Key(i)))
								if ((g_world->consoleBuffer.find(s) == g_world->consoleBuffer.npos)) // only add if letter is not already in buffer
									g_world->consoleBuffer.append(s);
						}

						if (1)
						{
							// TODO: make this only happen when a key is released and while no keys are pressed
							// SDL UP/DOWN events don't seem to work for this
							// buffer is added twice when this happens here
							g_world->consoleText.append(g_world->consoleBuffer);
							g_world->consoleBuffer.clear();
						}

						std::cout << "\n\nType in a command and press enter. [HELP] [QUIT]" << std::endl;
						std::cout << ">> [" << g_world->consoleText << "." << g_world->consoleBuffer << "]" << std::endl;

					}
					// normal input
					else
					{
						if (Input::isKeyPressed(Input::Key::Tab))
							g_world->useConsole = true;
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

		void init()
		{
			Memory::init();

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

			// pass context to world
			g_world->init(Context{ g_window,
								 // g_textureHolder,
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

				if(g_running)
					render();


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
			defaultAllocator().makeDelete<World>(g_world);

			Input::cleanup();
			g_window.close();
			SDL_Quit();
			Memory::shutdown();

			std::exit(EXIT_SUCCESS);
		}

	} // end Game
} // end Dunjun
