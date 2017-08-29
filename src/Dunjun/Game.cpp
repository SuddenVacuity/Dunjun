
#include <Dunjun/System/Logger.hpp>
#include <Dunjun/Game.hpp>

#ifdef DUNJUN_SYSTEM_WINDOWS
#include <Windows.h>
#endif


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
		GLOBAL const u32 FrameLimit = 288;
		GLOBAL bool g_running = true;
		GLOBAL u32 logLevel = 1;

		GLOBAL bool useConsole = false;		   // TODO: move these to UI manager
		GLOBAL bool checkForCommand = false;   // TODO: move these to UI manager
		// consoleText reference is created in Game::run()
		GLOBAL String* consoleText;			   // TODO: move these to UI manager

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

	// out texture
	GLOBAL const Texture* g_currentOutputTexture = nullptr;

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
			u32 counter = 0;
			logPrint(g_loggerModel, "Loading shader %d", counter++);
			g_shaderHolder.insertFromFile("default", "default_vert.glsl", 
													 "default_frag.glsl");
			logPrint(g_loggerModel, "Loading shader %d", counter++);
			g_shaderHolder.insertFromFile("texturePass", "texPass_vert.glsl", 
														 "texPass_frag.glsl");
			logPrint(g_loggerModel, "Loading shader %d", counter++);
			g_shaderHolder.insertFromFile("deferredGeometryPass", "deferredGeometryPass_vert.glsl", 
																  "deferredGeometryPass_frag.glsl");
			logPrint(g_loggerModel, "Loading shader %d", counter++);
			g_shaderHolder.insertFromFile("deferredDirectionalLight", "deferredLightPass_vert.glsl",
																	  "deferredDirectionalLightPass_frag.glsl");

			logPrint(g_loggerModel, "Loading shader %d", counter++);
			g_shaderHolder.insertFromFile("deferredAmbientLight", "deferredLightPass_vert.glsl", 
																  "deferredAmbientLightPass_frag.glsl");
			logPrint(g_loggerModel, "Loading shader %d", counter++);
			g_shaderHolder.insertFromFile("deferredPointLight", "deferredLightPass_vert.glsl",
																"deferredPointLightPass_frag.glsl");

			logPrint(g_loggerModel, "Loading shader %d", counter++);
			g_shaderHolder.insertFromFile("deferredSpotLight", "deferredLightPass_vert.glsl",
															   "deferredSpotLightPass_frag.glsl");



			logPrint(g_loggerModel, "Loading shader %d", counter++);
			g_shaderHolder.insertFromFile("deferredFinalPass", "deferredLightPass_vert.glsl",
															   "deferredFinalPass_frag.glsl");
		}

		INTERNAL void loadMaterials()
		{
			u32 counter = 0;
			// load diffuse textures
			logPrint(g_loggerModel, "Loading diffuse texture %d", counter++);
			g_texDiffuse_default = loadTextureFromFile("data/textures/default.png");

			logPrint(g_loggerModel, "Loading diffuse texture %d", counter++);
			g_texDiffuse_dunjunText = loadTextureFromFile("data/textures/dunjunText.jpg");

			logPrint(g_loggerModel, "Loading diffuse texture %d", counter++);
			g_texDiffuse_stone = loadTextureFromFile("data/textures/stone.png");

			logPrint(g_loggerModel, "Loading diffuse texture %d", counter++);
			g_texDiffuse_terrain = loadTextureFromFile("data/textures/terrain.png", TextureFilter::Nearest);

			logPrint(g_loggerModel, "Loading diffuse texture %d", counter++);
			g_texDiffuse_brick = loadTextureFromFile("data/textures/bricks.jpg");

			counter = 0;
			// load normal maps
			logPrint(g_loggerModel, "Loading normal texture %d", counter++);
			g_texNormal_default = loadTextureFromFile("data/textures/default_normal.jpg");
			logPrint(g_loggerModel, "Loading normal texture %d", counter++);
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
		)					CHECK FOR CONSOLE COMMAND
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


		INTERNAL void checkForConsoleCommand()
		{
			if(!checkForCommand)
				return;

			String ConsoleCommand = Strings::toUpperCase(*consoleText);
			logPrint(g_loggerConsole, "Input: %s", cString(ConsoleCommand));

			if (ConsoleCommand == "HELP")
			{
				std::cout << "\n";
				std::cout << "Commands suck for now.\n--------------------" << std::endl;

				//std::cout << "GamePad::Left Stick = Move camera" << std::endl;
				//std::cout << "GamePad::Right Stick = Turn camera" << std::endl;
				//std::cout << "GamePad::D-Pad Move = sprite" << std::endl;
				//std::cout << "GamePad::Shoulder Buttons = Move camera up/down" << std::endl;
				//std::cout << "GamePad::X = Test vibration" << std::endl;
				//std::cout << "GamePad::B = Render Texture on sprite" << std::endl;
				//std::cout << "GamePad::A = Move Light to current Camera location" << std::endl;
				std::cout << "\n";
				//std::cout << "Keyboard::ArrowKeys = Move sprite" << std::endl;
				//std::cout << "Keyboard::L/R Ctrl = Move sprite up/down" << std::endl;
				std::cout << "\n";
				std::cout << "[SYSTEM] = Show system information" << std::endl;
				//std::cout << "[REGEN] = Regenerate level with culling" << std::endl;
				//std::cout << "[REGENNC] = Regenerate level without culling" << std::endl;
				std::cout << "[LTCHRED] = Test ColorLib::removeChannelRed on point lights" << std::endl;
				std::cout << "[LTCHGRN] = Test ColorLib::removeChannelGreen on point lights" << std::endl;
				std::cout << "[LTCHBLU] = Test ColorLib::removeChannelBlue on point lights" << std::endl;
				std::cout << "[LTGREYS] = Test ColorLib::greyScale on point lights" << std::endl;
				std::cout << "[LTRESET] = Set light to default values" << std::endl;
				std::cout << "[DIR] = Return views cardinal direction and vertical angle" << std::endl;
				//std::cout << "[ROOMS] = Return number of rooms currently rendering" << std::endl;
				std::cout << "[HELP] = Show help" << std::endl;
				std::cout << "[QUIT] = Close Program" << std::endl;
				//std::cout << "Keyboard::T = Test multiply transforms" << std::endl;
				//std::cout << "Keyboard::R = Reset sprite position, orientation and scale" << std::endl;
				//std::cout << "Keyboard::L = Change to orthographic camera" << std::endl;
				//std::cout << "Keyboard::K = Change to perspective camera" << std::endl;


			}
			else if (ConsoleCommand == "TODO")
			{
				std::cout << "\n";
				std::cout << "stuff" << std::endl;
			}
			// room visibility test
			//else if (ConsoleCommand == "ROOMS")
			//{
			//	std::cout << "\n";
			//	std::cout << "Rendering " << level->roomsRendered << " Rooms" << std::endl;
			//}
			//// room visibility test
			else if (ConsoleCommand == "CHAR")
			{
				std::cout << "\n";
				for (int i = 0; i < 255; i++)
				{
					// skip the beep
					if (i == 7)
						continue;

					std::cout << "char# " << i << " - " << (char)i << std::endl;
				}

			}
			// room visibility test
			else if (ConsoleCommand == "SYSTEM")
			{
				std::cout << "\n";
				std::cout << "Using Grapics Card:\n-------------------" << std::endl;
				std::cout << glGetString(GL_VENDOR) << std::endl;
				std::cout << glGetString(GL_RENDERER) << std::endl;
				std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

				std::cout << "\n\n";
			}
			// return direction
			else if (ConsoleCommand == "DIR")
			{
				std::cout << "\n";
				Vector3 f = forwardVector(g_world->camera.transform.orientation);

				s32 angle = f.y * 90;

				String direction;

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

				std::cout << "You are facing " << direction << " and looking " << angle << " degrees vertically.\n";

			}
			// cout test iterator
			else if (ConsoleCommand == "ITERATOR")
			{
				std::cout << "\n";
				std::cout << "Test Iterator 0: " << testIterator_5[0] << std::endl;
				std::cout << "Test Iterator 1: " << testIterator_5[1] << std::endl;
				std::cout << "Test Iterator 2: " << testIterator_5[2] << std::endl;
				std::cout << "Test Iterator 3: " << testIterator_5[3] << std::endl;
				std::cout << "Test Iterator 4: " << testIterator_5[4] << std::endl;
			}
			// level regeneration
			//else if (ConsoleCommand == "REGEN")
			//{
			//		std::cout << "\n";
			//		toggleCulling = true;
			//		SceneNode* level = sceneGraph.findChildByName("level");
			//		sceneGraph.detachChild(*level);
			//	
			//		{ // test level generation
			//			auto level = make_unique<Level>();
			//	
			//			level->material = &context.materialHolder->get("dunjunText");
			//			level->name = "level";
			//	
			//			level->generate();
			//	
			//			this->level = level.get();
			//	
			//			sceneGraph.attachChild(std::move(level));
			//		}
			//}
			// regenerate world without culling
			//else if (ConsoleCommand == "REGENNC")
			//{
			//		std::cout << "\n";
			//		toggleCulling = false;
			//	
			//		SceneNode* level = sceneGraph.findChildByName("level");
			//		sceneGraph.detachChild(*level);
			//	
			//		{ // test level generation
			//			auto level = make_unique<Level>();
			//	
			//			level->material = &context.materialHolder->get("dunjunText");
			//			level->name = "level";
			//	
			//			level->generate();
			//	
			//			this->level = level.get();
			//	
			//			sceneGraph.attachChild(std::move(level));
			//		}
			//}
			// remove red from point lights
			else if (ConsoleCommand == "LTCHRED")
			{
				std::cout << "\n";
				for (PointLight& light : g_world->renderSystem.pointLights)
				{
					light.color = ColorLib::removeChannelRed(light.color);
					light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				}
			}
			// remove green from point lights
			else if (ConsoleCommand == "LTCHGRN")
			{
				std::cout << "\n";
				for (PointLight& light : g_world->renderSystem.pointLights)
				{
					light.color = ColorLib::removeChannelGreen(light.color);
					light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				}
			}
			// remove blue from point lights
			else if (ConsoleCommand == "LTCHBLU")
			{
				std::cout << "\n";
				for (PointLight& light : g_world->renderSystem.pointLights)
				{
					light.color = ColorLib::removeChannelBlue(light.color);
					light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				}
			}
			// make point lights greyscale
			else if (ConsoleCommand == "LTGREYS")
			{
				std::cout << "\n";
				for (PointLight& light : g_world->renderSystem.pointLights)
				{
					light.color = ColorLib::greyScale(light.brightness);
					light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				}
			}
			// reset point lights to default
			else if (ConsoleCommand == "LTRESET")
			{
				std::cout << "\n";
				u32 n = 0;
				for (PointLight& light : g_world->renderSystem.pointLights)
				{
					switch (n)
					{
					default: break;
					case 0:
					{
						light.color = ColorLib::White;
						light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
						break;
					}
					case 1:
					{
						light.color = ColorLib::Red;
						light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
						break;
					}
					case 2:
					{
						light.color = ColorLib::Blue;
						light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
						break;
					}
					case 3:
					{
						light.color = ColorLib::Green;
						light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
						break;
					}
					}
					n++;
				}

			}
			else if (ConsoleCommand == "QUIT")
			{
				//g_window->close();
				g_running = false;
				return;
			}
			else
			{
				logPrint(g_loggerConsole, "\nInvalid command");
			}

			*consoleText = "";
			checkForCommand = false;
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
					// console
					if (useConsole == true)
					{
						String consoleBuffer = "";
						// exit console
						if (Input::isKeyPressed(Input::Key::Tab))
						{
							logPrint(g_loggerConsole, "Console Closed");
							useConsole = false;
						}
				
						// check for a command
						if (Input::isKeyPressed(Input::Key::Return))
							checkForCommand = true;
				
						if (Input::isKeyPressed(Input::Key::Space))
							append(consoleBuffer, " ");
				
						// Erase letters
						if (Input::isKeyPressed(Input::Key::Backspace))
							if (len(*consoleText) > 0)
								popBack(*consoleText);
				
						if (Input::isKeyPressed(Input::Key::Delete))
						{
							consoleBuffer = "";
							*consoleText = "";
						}
				
						// TODO: press up or down to cycle previous commands
						if (Input::isKeyPressed(Input::Key::Up))
							append(consoleBuffer, " ");
						if (Input::isKeyPressed(Input::Key::Down))
							append(consoleBuffer, " ");
				
						//////////////////////////////////
						//								//
						//		   ADD LETTERS			//
						//								//
						//////////////////////////////////
				
						for (int i = 0; i < 26; i++)
						{
							char s = 0;
				
							if (event.key.capsLock == true || event.key.shift == true)
								s = i + 65; // upper case letters
							else
								s = i + 97; // lower case letters
				
							if (Input::isKeyPressed(Input::Key(i)))
								if (!Strings::contains(consoleBuffer, s)) // only add if letter is not already in buffer
									append(consoleBuffer, s);
						}

						if (consoleBuffer != "")
						{
							// TODO: make this only happen when a key is released and while no keys are pressed
							// SDL UP/DOWN events don't seem to work for this
							// buffer is added twice when this happens here

							*consoleText = *consoleText + consoleBuffer;
							 
							consoleBuffer = "";
							shrinkToFit(consoleBuffer);
						}

						std::cout << "\n\nType in a command and press enter. [HELP] [QUIT]" << std::endl;
						std::cout << ">> [" << *consoleText << "." << consoleBuffer << "]" << std::endl;
				
					}
					// normal input
					else
					{
						if (Input::isKeyPressed(Input::Key::Tab))
						{
							logPrint(g_loggerConsole, "Console Opened");
							useConsole = true;
						}
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

			checkForConsoleCommand();

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
			//g_world->update(dt);

			SceneGraph& sg = g_world->sceneGraph;

			f32 wt = 1.0f * Time::now().asSeconds();
			f32 a = 2.0f;

			f32 moveSin = Math::sin(Radian(wt));
			f32 moveCos = Math::cos(Radian(wt));

			{
				SceneGraph::NodeId crateNode = sg.getNodeId(g_world->crate);
				SceneGraph::NodeId playerNode = sg.getNodeId(g_world->player);


				if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadDown))
				{
					std::cout << "Controller 1 is working!\n";
				}
				if (Input::isGamepadButtonPressed(1, Input::GamepadButton::DpadDown))
				{
					std::cout << "Controller 2 is working!\n";
				}

				if(Input::isKeyPressed(Input::Key::LShift))
				{
					Transform pos = sg.getLocalTransform(crateNode);
					//pos.position.y = moveSin * 5.0f * dt.asSeconds();
					//pos.scale = Vector3{ 1.2f, 1.2f, 1.2f } + Vector3{ 0.4f, 0.4f, 0.4f } * Math::sin(Radian(wt));
					//pos.orientation = offsetOrientation(pos.orientation,
					//									Degree(360.0f * dt.asSeconds()),
					//									Degree(10.0f * dt.asSeconds()));


					if (Input::isKeyPressed(Input::Key::Left))
					{
						if (Input::isKeyPressed(Input::Key::W))
							pos.position.x -= 10 * dt.asSeconds();
						if (Input::isKeyPressed(Input::Key::E))
							pos.orientation = offsetOrientation(pos.orientation, Degree(360.0f * dt.asSeconds()), Degree(0));
						if(Input::isKeyPressed(Input::Key::R))
							pos.scale += Vector3{ 10 * dt.asSeconds(), 0, 0 };

					}
					if (Input::isKeyPressed(Input::Key::Right))
					{
						if (Input::isKeyPressed(Input::Key::W))
							pos.position.x += 10 * dt.asSeconds();
						if (Input::isKeyPressed(Input::Key::E))
							pos.orientation = offsetOrientation(pos.orientation, -Degree(360.0f * dt.asSeconds()), Degree(0));
						if (Input::isKeyPressed(Input::Key::R))
							pos.scale -= Vector3{ 10 * dt.asSeconds(), 0, 0 };
					}
					if (Input::isKeyPressed(Input::Key::Up))
					{
						if (Input::isKeyPressed(Input::Key::W))
							pos.position.z -= 10 * dt.asSeconds();
						if (Input::isKeyPressed(Input::Key::E))
							pos.orientation = offsetOrientation(pos.orientation, Degree(0), Degree(360.0f * dt.asSeconds()));
						if (Input::isKeyPressed(Input::Key::R))
							pos.scale += Vector3{ 0, 10 * dt.asSeconds(), 0 };
					}
					if (Input::isKeyPressed(Input::Key::Down))
					{
						if (Input::isKeyPressed(Input::Key::W))
							pos.position.z += 10 * dt.asSeconds();
						if (Input::isKeyPressed(Input::Key::E))
							pos.orientation = offsetOrientation(pos.orientation, Degree(0), -Degree(360.0f * dt.asSeconds()));
						if (Input::isKeyPressed(Input::Key::R))
							pos.scale -= Vector3{ 0, 10 * dt.asSeconds(), 0 };
					}

					sg.setLocalTransform(crateNode, pos);
				}

				f32 leftAxis = Input::getGamepadAxis(0, Input::GamepadAxis::LeftX);
				if (leftAxis > 0.3f)
				{
					std::cout << "Looking: " << g_world->camera.transform.orientation << std::endl;
					g_world->camera.transform.orientation = offsetOrientation(g_world->camera.transform.orientation, Degree(0), Degree(360.0f * dt.asSeconds()));
				}

				if(Input::isKeyPressed(Input::Key::RShift))
				{
					Transform pos = sg.getLocalTransform(playerNode);
					//pos2.position.x = moveCos * 5.0f * dt.asSeconds();


					if (Input::isKeyPressed(Input::Key::Left))
					{
						if (Input::isKeyPressed(Input::Key::W))
							pos.position.x -= 10 * dt.asSeconds();
						if (Input::isKeyPressed(Input::Key::E))
							pos.orientation = offsetOrientation(pos.orientation, Degree(360.0f * dt.asSeconds()), Degree(0));
						if (Input::isKeyPressed(Input::Key::R))
							pos.scale += Vector3{ 10 * dt.asSeconds(), 0, 0 };

					}
					if (Input::isKeyPressed(Input::Key::Right))
					{
						if (Input::isKeyPressed(Input::Key::W))
							pos.position.x += 10 * dt.asSeconds();
						if (Input::isKeyPressed(Input::Key::E))
							pos.orientation = offsetOrientation(pos.orientation, -Degree(360.0f * dt.asSeconds()), Degree(0));
						if (Input::isKeyPressed(Input::Key::R))
							pos.scale -= Vector3{ 10 * dt.asSeconds(), 0, 0 };
					}
					if (Input::isKeyPressed(Input::Key::Up))
					{
						if (Input::isKeyPressed(Input::Key::W))
							pos.position.z -= 10 * dt.asSeconds();
						if (Input::isKeyPressed(Input::Key::E))
							pos.orientation = offsetOrientation(pos.orientation, Degree(0), Degree(360.0f * dt.asSeconds()));
						if (Input::isKeyPressed(Input::Key::R))
							pos.scale += Vector3{ 0, 10 * dt.asSeconds(), 0 };
					}
					if (Input::isKeyPressed(Input::Key::Down))
					{
						if (Input::isKeyPressed(Input::Key::W))
							pos.position.z += 10 * dt.asSeconds();
						if (Input::isKeyPressed(Input::Key::E))
							pos.orientation = offsetOrientation(pos.orientation, Degree(0), -Degree(360.0f * dt.asSeconds()));
						if (Input::isKeyPressed(Input::Key::R))
							pos.scale -= Vector3{ 0, 10 * dt.asSeconds(), 0 };
					}

					sg.setLocalTransform(playerNode, pos);
				}
			}

			{
				Camera& c = g_world->camera;
				//c.transform.position.x = moveCos * 2;
				//c.transform.position.z = moveSin * 2;
				c.transform.position.x = 0;
				c.transform.position.z = 5;
				//
				c.transform.orientation = conjugate(Math::lookAtQuaternion(c.transform.position, Vector3::Zero));
			}

			{
				PointLight& pl = g_world->renderSystem.pointLights[0];

				if (Input::isKeyPressed(Input::Key::Left))
					pl.position.x -= 10 * dt.asSeconds();
				if (Input::isKeyPressed(Input::Key::Right))
					pl.position.x += 10 * dt.asSeconds();
				if (Input::isKeyPressed(Input::Key::Up))
					pl.position.z -= 10 * dt.asSeconds();
				if (Input::isKeyPressed(Input::Key::Down))
					pl.position.z += 10 * dt.asSeconds();

				if (Input::isKeyPressed(Input::Key::S))
				{
					logPrint(g_loggerInfo, "Test change hue with RGBtoHSL");
					Vector3 hsl = ColorLib::RGBtoHSL(pl.color);

					std::cout << "\nEnter Hue value (0.0~5.9999)";
					std::cin >> hsl.x;
					std::cout << "\nEnter saturation value (0.0~1.0)";
					std::cin >> hsl.y;
					std::cout << "\nEnter lightness value (0.0~1.0)";
					std::cin >> hsl.z;

					Color color = ColorLib::HSLtoRGB(hsl.x, hsl.y, hsl.z);

					printf("\n\npl.color {%d, %d, %d, %d} color {%d, %d, %d, %d}\n",
							pl.color.r, pl.color.g, pl.color.b, pl.color.a,
							color.r, color.g, color.b, color.a);

					hsl = ColorLib::RGBtoHSL(color);
					printf("\n\nRGBtoHSL() hue %f, saturation %f, lightness %f}\n", hsl.x, hsl.y, hsl.z);

					pl.color = color;
					pl.colorIntensity = calculateLightIntensities(pl.color, pl.intensity);
					pl.brightness = ColorLib::calculateBrightness(pl.colorIntensity);
				}
				if (Input::isKeyPressed(Input::Key::A))
				{
					logPrint(g_loggerInfo, "Reset Point Light Color");
					pl.color = ColorLib::Yellow;
					pl.colorIntensity = calculateLightIntensities(pl.color, pl.intensity);
					pl.brightness = ColorLib::calculateBrightness(pl.colorIntensity);
				}



				if (Input::isKeyPressed(Input::Key::B))
				{
					Vector3 hsl = ColorLib::RGBtoHSL(pl.color);
					Color color = ColorLib::HSLtoRGB(hsl.x, hsl.y, hsl.z);

					u32 stop = 0;
				}
			}



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

		INTERNAL Vector2 absoluteSize(const Window& window, f32 width, f32 height)
		{
			const Vector2 size = { g_window.currentSize[0], g_window.currentSize[1] };
			return {width / size.x, height / size.y};
		}

		INTERNAL Vector2 absolutePosition(const Window& window, f32 width, f32 height)
		{
			const Vector2 size = { g_window.currentSize[0], g_window.currentSize[1] };
			const f32 posX = (2.0f * width / size.x);
			const f32 posY = (2.0f * height / size.y);
			return { posX, posY };
		}

		INTERNAL void drawSprite(const Window& window, const Rectangle& rec, ShaderProgram& shaders, const Texture* tex)
		{
			shaders.setUniform("u_scale", absoluteSize(window, rec.width, rec.height));
			shaders.setUniform("u_position", absolutePosition(window, rec.x, rec.y));

			shaders.setUniform("u_tex", 0);
			bindTexture(tex, 0);

			drawMesh(g_meshHolder.get("quad"));
		}

		INTERNAL void renderUi()
		{
			ShaderProgram& shaders = g_shaderHolder.get("texturePass");
			shaders.use();
			defer(shaders.stopUsing());
			glDisable(GL_DEPTH_TEST);

			const Vector2 size = g_window.currentSize;
			const f32 aspect = g_window.currentAspectRatio;

			Vector2 UiSize = {100, 100};

			Rectangle rec = Rectangle();
			rec.x = ((UiSize.x / 2) * aspect) - (size.x / 2);
			rec.y = ((UiSize.y / 2) + UiSize.y * (GBuffer::Count)) - (size.y / 2);
			rec.width = UiSize.x * aspect;
			rec.height = UiSize.y;

			
			for(u32 i = 0; i < GBuffer::Count; i++)
			{
				drawSprite(g_window, rec, shaders, &g_world->renderSystem.gBuffer.textures[i]);
				rec.y -= UiSize.y;
			}
			drawSprite(g_window, rec, shaders,&g_world->renderSystem.lightingBuffer.colorTexture);
		}

		INTERNAL void render()
		{
			//g_world->renderSystem.fbSize.x = g_window.getSize().x;
			//g_world->renderSystem.fbSize.y = g_window.getSize().y;

			RenderSystem& rs = g_world->renderSystem;

			if (g_currentOutputTexture == nullptr)
				g_currentOutputTexture = &rs.finalTexture.colorTexture;

			g_window.setSize(g_window.getSize());
			rs.fbSize = g_window.currentSize;


			rs.resetAllPointers();

			g_world->camera.viewportAspectRatio = g_window.currentAspectRatio;

			rs.camera = &g_world->camera;

			rs.render();

			glViewport(0, 0, g_window.currentSize.x, g_window.currentSize.y);
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			{

				ShaderProgram& shaders = g_shaderHolder.get("texturePass");
				shaders.use();
				defer(shaders.stopUsing());

				shaders.setUniform("u_tex", 0);
				shaders.setUniform("u_scale", Vector3{ 1, 1, 1 });

				glDisable(GL_DEPTH_TEST);

				drawSprite(g_window, Rectangle(0, 0, rs.fbSize.x, rs.fbSize.y), 
						   shaders, g_currentOutputTexture);

			}

			renderUi();

			std::cout << "";
			//g_world->render();
		}
		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					GET SYSTEM INFO
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		INTERNAL void getSystemInformation()
		{
			logSection(g_loggerInfo, "Get System Information");

			{
				SDL_version compiled, linked;
				SDL_VERSION(&compiled);
				SDL_GetVersion(&linked);
				logPrint(g_loggerInfo, "Compile SDL version %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
				logPrint(g_loggerInfo, "Running SDL version %d.%d.%d", linked.major, linked.minor, linked.patch);
			}
			{
				SDL_Renderer* renderer = SDL_CreateRenderer(g_window.getSDLHandle(), -1, 0);
				SDL_RendererInfo rendererInfo;
				SDL_GetRendererInfo(renderer, &rendererInfo);
				logPrint(g_loggerInfo, "Renderer name: %s", rendererInfo.name);
			}

			logPrint(g_loggerInfo, "Graphics Card: %s - %s v%s", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));


			logPrint(g_loggerInfo, "System Platform: %s", SDL_GetPlatform());
			logPrint(g_loggerInfo, "OS Version: %s", getSystemOS());
			logPrint(g_loggerInfo, "Number of logical cores: %d", SDL_GetCPUCount());
			logPrint(g_loggerInfo, "System RAM: %dMB", SDL_GetSystemRAM());
			logPrint(g_loggerInfo, "GetCurrentVideoDiver: %s", SDL_GetCurrentVideoDriver());



			u32 count; // shared variable for limiting iterators
			count = SDL_GetNumVideoDrivers();
			for (uSize_t i = 0; i < count; i++)
				logPrint(g_loggerInfo, "Video Driver %d: %s", i, SDL_GetVideoDriver(i));

			count = SDL_GetNumVideoDisplays();
			for (uSize_t i = 0; i < count; i++)
				logPrint(g_loggerInfo, "Display Device %d: %s", i, SDL_GetDisplayName(i));
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
			g_world->camera.transform.position = {0, 1, 3};
			g_world->camera.transform.orientation = conjugate(Math::lookAtQuaternion(g_world->camera.transform.position, Vector3::Zero));
			g_world->camera.fieldOfView = Degree(90);
			g_world->camera.projectionType = ProjectionType::Perspective;
			

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
				light.intensity = 0.0f;
				light.colorIntensity = calculateLightIntensities(light.color, light.intensity);
				light.brightness = ColorLib::calculateBrightness(light.colorIntensity);
				append(rs.directionalLights, light);
			}

			for(u32 i = 0; i < 1; i++)
			{
				PointLight light;

				Random r;
				f32 x = 0; //r.getFloat(-3.5, 3.5f);
				f32 y = 1; //r.getFloat(0.5f, 4);
				f32 z = 0; //r.getFloat(-3.5, 3.5f);

				light.color = ColorLib::White;
				light.position = Vector3{x, y, z};
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

			// init loggers
			{
				std::remove("log.txt");
				g_logFile = fopen("log.txt", "a+");

				setLogger(g_loggerDebug, g_logFile, "[DEBUG]",
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_Time |
					LogFlag::LogFlag_Date |
					LogFlag::LogFlag_Text_NewLine |
					LogFlag::LogFlag_Disable);
				setLogger(g_loggerError, g_logFile, "[ERROR]",
					LogFlag::LogFlag_PrintToTerminal |
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_Time |
					LogFlag::LogFlag_Date |
					LogFlag::LogFlag_ColorText_Red |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerInfo, g_logFile, "[INFO]",
					LogFlag::LogFlag_PrintToTerminal |
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerModel, g_logFile, "[MODEL]",
					LogFlag::LogFlag_PrintToTerminal |
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_ColorText_Cyan |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerConsole, g_logFile, "[CONSOLE]",
					LogFlag::LogFlag_PrintToTerminal |
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_ColorText_Green |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerWindow, g_logFile, "[WINDOW]",
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerInput, g_logFile, "[INPUT]",
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerEvent, g_logFile, "[EVENT]",
					LogFlag::LogFlag_PrintToTerminal |
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_ColorText_Yellow |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerRender, g_logFile, "[RENDER]",
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerGame, g_logFile, "[GAME]",
					LogFlag::LogFlag_PrintToTerminal |
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_ColorText_Magenta |
					LogFlag::LogFlag_Text_NewLine);
				setLogger(g_loggerGraphics, g_logFile, "[GRAPHICS]",
					LogFlag::LogFlag_PrintToTerminal |
					LogFlag::LogFlag_SaveToFile |
					LogFlag::LogFlag_ColorText_White |
					LogFlag::LogFlag_Text_NewLine);
			}

			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER |
				SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK) != 0)
			{
				//std::cerr << "SDL Failed to initialize. Error: ";
				//std::cerr << SDL_GetError;
				//std::cerr << std::endl;
				logPrint(g_loggerError, "SDL Failed to initialize: %s", SDL_GetError);

				std::exit(EXIT_FAILURE);
			}

			// import form config files
			logSection(g_loggerInfo, "Import Config File to Memory");
			ConfigData configData = loadConfigDataFromFile("data/defaultSettings.op");

			// test getConfigData
			{
				logSection(g_loggerInfo, "Get ConfigData from Memory");

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
			g_window.setFramerateLimit(0);

			getSystemInformation();

			glewInit();

			// Initial OpenGL settings
			glInit();

			// initialize input
			Input::setUp();
			Input::setCursorPosition({ 0, 0 });

			// load assets
			logSection(g_loggerInfo, "Load Assets");

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

			// consoleText must be created after Memory::init()
			// and needs to persist throughout while(g_running)
			String consoleTextRef = "";
			consoleText = &consoleTextRef;

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

					render();

				}


				if (tc.update(milliseconds(500)))
				{
					// dynamic window title
					g_window.setTitle(stringFormat("Dunjun - %.3f ms - %d fps",
												   1000.f / tc.tickRate, 
												   (u32)tc.tickRate).c_str());
				}

				g_window.display();

			} // end while(g_running)
		}

		void cleanUp()
		{
			defaultAllocator().makeDelete(g_world);
			Input::cleanup();
			g_window.close();
			SDL_Quit();
			fclose(g_logFile);
			Memory::shutdown();

			std::exit(EXIT_SUCCESS);
		}

	} // end Game
} // end Dunjun
