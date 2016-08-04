

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
	GLOBAL Texture g_texDiffuse_default;
	GLOBAL Texture g_texNormal_default;

	GLOBAL Texture g_texDiffuse_dunjunText;

	GLOBAL Texture g_texDiffuse_stone;

	GLOBAL Texture g_texDiffuse_brick;
	GLOBAL Texture g_texNormal_brick;

	GLOBAL Texture g_texDiffuse_terrain;

	// materials
	GLOBAL Material g_material_default;
	GLOBAL Material g_material_dunjunText;
	GLOBAL Material g_material_stone;
	GLOBAL Material g_material_brick;
	GLOBAL Material g_material_terrain;

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
			// load diffuse textures
			g_texDiffuse_default = loadTextureFromFile("data/textures/default.png");
			g_texDiffuse_dunjunText = loadTextureFromFile("data/textures/dunjunText.jpg");
			g_texDiffuse_stone = loadTextureFromFile("data/textures/stone.png");
			g_texDiffuse_terrain = loadTextureFromFile("data/textures/terrain.png", TextureFilter::Nearest);
			g_texDiffuse_brick = loadTextureFromFile("data/textures/bricks.jpg");

			// load normal maps
			g_texNormal_default = loadTextureFromFile("data/textures/default_normal.jpg");
			g_texNormal_brick = loadTextureFromFile("data/textures/bricks_normal.png");

			// create materials
			g_material_default = Material{};
			g_material_default.diffuseMap = &g_texDiffuse_default;
			g_material_default.normalMap  = &g_texNormal_default;

			g_material_dunjunText = Material{};
			g_material_dunjunText.diffuseMap = &g_texDiffuse_dunjunText;
			g_material_dunjunText.normalMap  = &g_texNormal_default;

			g_material_brick = Material{};
			g_material_brick.diffuseMap = &g_texDiffuse_brick;
			g_material_brick.normalMap  = &g_texNormal_brick;

			g_material_stone = Material{};
			g_material_stone.diffuseMap = &g_texDiffuse_stone;
			g_material_stone.normalMap  = &g_texNormal_default;

			g_material_terrain = Material{};
			g_material_terrain.diffuseMap = &g_texDiffuse_terrain;
			g_material_terrain.normalMap  = &g_texNormal_default;

			std::cout << "";
		}

		// sprite vertex info, vbo and ibo
		INTERNAL void loadSpriteAsset()
		{

			// Quad
			{
				// Here is where you add vertice information
				//
				Vertex vertices[] = { // define vertexes for a triangle
					//		  x		 y	   z		 s	   t		// for triangle strips organize vertexes in a backwards Z
					Vertex({ +1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f }),	// 0 vertex         1 ---- 0        
					Vertex({ -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f }),	// 1 vertex           \             
					Vertex({ +1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }),	// 2 vertex              \           
					Vertex({ -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }),	// 3 vertex         3 -----2       
				};

				u32 indices[] = { 3, 2, 0, 0, 1, 3 }; // vertex draw order for GL_TRIANGLES

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
					//		  x		 y	   z		 s	   t		// for triangle strips organize vertexes in a backwards Z
					Vertex({ +0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f }),	// 0 vertex         1 ---- 0        
					Vertex({ -0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f }),	// 1 vertex           \             
					Vertex({ +0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f }),	// 2 vertex              \           
					Vertex({ -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f }),	// 3 vertex         3 -----2       
				};
			
				u32 indices[] = { 3, 2, 0, 0, 1, 3 }; // vertex draw order for GL_TRIANGLES
			
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
					//		  x		 y	   z		 s	   t		// for triangle strips organize vertexes in a backwards Z
					Vertex({ +0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f }),	// 0 vertex         1 ---- 0        
					Vertex({ -0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f }),	// 1 vertex           \             
					Vertex({ +0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f }),	// 2 vertex              \           
					Vertex({ -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f }),	// 3 vertex         3 -----2       
				};

				u32 indices[] = { 3, 2, 0, 0, 1 ,3 }; // vertex draw order for GL_TRIANGLES

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

			f32 moveSin = a * Math::sin(Radian(wt));
			f32 moveCos = a * Math::cos(Radian(wt));

			Transform pos = sg.getLocalTransform(crateNode);
			pos.position.y = moveSin;
			pos.scale = Vector3{ 1.2f, 1.2f, 1.2f } + Vector3{ 0.4f, 0.4f, 0.4f } * Math::sin(Radian(wt));
			pos.orientation = offsetOrientation(pos.orientation,
												Degree(360.0f * dt.asSeconds()),
												Degree(10.0f * dt.asSeconds()));

			sg.setLocalTransform(crateNode, pos);

			Transform pos2 = sg.getLocalTransform(playerNode);
			pos2.position.x = moveCos;
			sg.setLocalTransform(playerNode, pos2);

			Camera& c = g_world->camera;
			c.transform.position.x = moveCos;
			c.transform.position.z = moveSin;

			c.transform.orientation = conjugate(Math::lookAtQuaternion(c.transform.position, Vector3::Zero));






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
				rs.addComponent(crate, { g_meshHolder.get("sprite"), g_material_dunjunText });
				rs.addComponent(player, { g_meshHolder.get("sprite"), g_material_dunjunText });
				rs.addComponent(floor, { g_meshHolder.get("surface"), g_material_brick });
				rs.addComponent(wall1, { g_meshHolder.get("surface"), g_material_stone });
				rs.addComponent(wall2, { g_meshHolder.get("surface"), g_material_stone });
				rs.addComponent(wall3, { g_meshHolder.get("surface"), g_material_stone });
			}
			{
				DirectionalLight light;

				light.direction = { -0.0f, +0.2f, -0.8f };
				light.color = ColorLib::Orange;
				light.intensity = 0.12f;
				light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				light.brightness = ColorLib::calculateBrightness(light.colorIntensity);
				append(rs.directionalLights, light);
			}

			for(u32 i = 0; i < 10; i++)
			{
				PointLight light;

				Random r;
				f32 x = r.getFloat(-3.5, 3.5f);
				f32 y = r.getFloat(0.5f, 4);
				f32 z = r.getFloat(-3.5, 3.5f);

				light.color = ColorLib::White;
				light.position = Vector3{x, y, z};
				light.intensity = 3.0f;
				light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				light.brightness = ColorLib::calculateBrightness(light.colorIntensity);
				light.range = calculateLightRange(light.intensity, light.color, light.attenuation);
				append(rs.pointLights, light);
			}

		}


		void init()
		{
			Memory::init();

			if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | 
						SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK) != 0)
			{
				std::cerr << "SDL Failed to initialize. Error: ";
				std::cerr << SDL_GetError;
				std::cerr << std::endl;

				std::exit(EXIT_FAILURE);
			}

			// import form config files
			ConfigData configData = loadConfigDataFromFile("data/defaultSettings.op");

			// test getConfigData
			{
			std::cout << "\nGetting ConfigData from Memory" <<
						 "\n==============================\n\n";

			String getString = getFromConfigData_string(configData, "NotWindow.var3", "derp");
			b8 getBool = getFromConfigData_bool(configData, "valFalse", true);
			String GetDoesntExist = getFromConfigData_string(configData, "thisDoesNotExist.varDoesntExist", "it really didnt' exist");
			}

			VideoMode vm = {};
			vm.width = getFromConfigData_uint(configData, "Window.width", 854);
			vm.height = getFromConfigData_uint(configData, "Window.height", 480);
			vm.bitsPerPixel = getFromConfigData_uint(configData, "Window.bitsPerPixel", 16);

			std::cout << "\n";

			g_window.create("Loading...", vm);
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

			uSize_t frames = 0;

			while (g_running) // create a loop that works until the window closes
			{
				//Window::pollEvents();
				//Window::makeContextCurrent();

				Time currentTime = Time::now();
				Time dt = currentTime - prevTime;

				prevTime = currentTime;
				accumulator += dt;

				// limit accumulator size
				if (accumulator > milliseconds(1200))
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
