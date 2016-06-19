

#include <Dunjun/Game.hpp>


namespace Dunjun
{

	//struct ModelInstance // copies an asset to use
	//{
	//	ModelAsset* asset;
	//	Transform transform;
	//};
	GLOBAL Window g_window;

	GLOBAL Camera g_cameraPlayer;
	GLOBAL Camera g_cameraWorld;
	GLOBAL Camera* g_currentCamera = &g_cameraWorld;
	//GLOBAL Matrix4 g_projection;

	namespace
	{
		GLOBAL const Time TIME_STEP = seconds(1.0f / 60.0f);
		GLOBAL const u32 FrameLimit = 60;
		GLOBAL bool g_running = true;
	} // end anon namespace

	GLOBAL ModelAsset g_sprite;

	GLOBAL SceneNode g_rootNode;
	GLOBAL SceneNode* g_player = nullptr;

	GLOBAL std::vector<DirectionalLight> g_directionalLights;
	GLOBAL std::vector<PointLight> g_pointLights;
	GLOBAL std::vector<SpotLight> g_spotLights;

	GLOBAL SceneRenderer g_renderer;

	//GLOBAL std::map<std::string, Material> g_materials;
	//GLOBAL std::map<std::string, Mesh*> g_meshes;

	GLOBAL Level* g_level = nullptr;

	GLOBAL std::unique_ptr<World> g_world;

	GLOBAL Transform g_parentTest;

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
		)				WINDOW HANDLING FUNCTIONS
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
		)					.
		)
		)				HANDLE INPUT
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		//
		INTERNAL void handleInput()
		{
			Event event;
			while (g_window.pollEvent(event))
			{
				switch(event.type)
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
				)				.
				)					.
				)
				)				.
				%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

				case Event::KeyPressed:// && acceptInput == true
				{		
					// console
					if(useConsole == true)
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
						for(int i = 0; i < 26; i++)
						{
							std::string s = ""; // can't initialize as i or cast in .append()

							if(event.key.capsLock == true || event.key.shift == true)
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
			)				.
			)					.
			)
			)				.
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

				g_rootNode.handleEvent(event);
			} // end while (g_window.pollEvent(event))

			/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			)				.
			)					HANDLE CLOSE WINDOW
			)
			)				.
			)					.
			)
			)				.
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
		)				LOAD SHADERS AND MATERIALS
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

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					LOAD ASSETS AND GENERATE WORLD
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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

				g_meshHolder.insert("sprite", make_unique<Mesh>(meshData));
				//g_meshes["sprite"] = new Mesh(meshData);

				g_sprite.mesh = &g_meshHolder.get("sprite");
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
		)					LOAD INSTANCES
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		// create instances
		INTERNAL void loadInstances()
		{
			//g_rootNode.init();

			{ // player scene node
				SceneNode::u_ptr player = make_unique<SceneNode>();

				player->transform.position = { 2.5f, 1.0f, 2.5f };
				player->transform.scale = { 2.0f, 2.0f, 2.0f };
				g_sprite.material = &g_materialHolder.get("dunjunText"); // apply material
				player->name = "player";

				player->addComponent<MeshRenderer>(g_sprite);
				//player->addComponent<FaceCamera>(g_cameraPlayer);

				g_player = player.get();

				g_rootNode.attachChild(std::move(player));
			}

			{ // level generation
				auto level = make_unique<Level>();

				level->material = &g_materialHolder.get("terrain");
				level->name = "level";

				level->generate();

				g_level = level.get();

				g_rootNode.attachChild(std::move(level));
			}

			// add point lights
			{
			PointLight light;

				light.setIntensities(ColorLib::White, 20.0f);
				light.position = { 0.0f, -300.0f, 0.0f };
				g_pointLights.emplace_back(light);

				light.setIntensities(ColorLib::Red, 5.0f);
				light.position = { 3.0f, 0.5f, 3.0f };
				g_pointLights.emplace_back(light);

				light.setIntensities(ColorLib::Blue, 5.0f);
				light.position = { 2.5f, 0.5f, 2.0f };
				g_pointLights.emplace_back(light);

				light.setIntensities(ColorLib::Green, 5.0f);
				light.position = { 2.0f, 0.5f, 3.0f };
				g_pointLights.emplace_back(light);
			}

			// add directional lights
			{
				DirectionalLight light;

				light.setIntensities(ColorLib::Orange, 0.02f);
				light.direction = Vector3(-0.8, -1.0, -0.2);
				g_directionalLights.emplace_back(light);
			}

			// add spot lights
			{
				SpotLight light;

				light.position = {2.5f, 2.0f, 2.5f};
				light.setIntensities(ColorLib::White, 50.0f);
				g_spotLights.emplace_back(light);
			}

			{
				//Initialize camera
				// aspect ratio
				g_cameraPlayer.viewportSize = g_window.getSize();
				g_cameraPlayer.viewportAspectRatio = g_window.getSize().x / g_window.getSize().y;

				g_cameraPlayer.transform.position = g_player->transform.position + Vector3(8 * 3, 8 * 2, 8 * 3);
				//g_cameraPlayer.transform.orientation = angleAxis(Degree(45), {0, 1, 0}) * angleAxis(Degree(-30), {1, 0, 0});
				g_cameraPlayer.lookAt(g_player->transform.position);

				g_cameraPlayer.projectionType = ProjectionType::Orthographic;
				g_cameraPlayer.fieldOfView = Degree(50.0f); // for perspective view
				g_cameraPlayer.orthoScale = 15.0f; // for perspective view

				
				// initialize world camera
				g_cameraWorld = g_cameraPlayer;
				g_cameraWorld.projectionType = ProjectionType::Perspective;
				g_cameraWorld.lookAt(g_player->transform.position);

				// temp variables used in lerp()
				//const Matrix4 op = g_cameraPlayer.getProjection();
				//const Matrix4 pp = g_cameraWorld.getProjection();

				/*// initialize projection types
				Matrix4 g_projection;

				// TODO: make Camera::Matrix4 view editable for blending
				g_projection = lerp(pp, op, 0.95f); // mostly orthographic
				g_cameraPlayer.getView() * g_projection;
				g_projection = lerp(pp, op, 0.01f); // mostly perspective
				g_cameraWorld.getView() * g_projection; */

			}
			g_rootNode.init();
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
			// test scene node
			//SceneNode* playerPtr = g_rootNode.findChildByName("player");
			g_rootNode.update(dt);

			f32 camVel = 20.0f; // multiplier for camera speed
			f32 playerVelX = 5.5f;
			f32 playerVelY = 3.5f;
			f32 playerVelZ = 5.5f;

			if(checkForCommand == true)
			{
				std::cout << "\n";
				if (consoleText == "HELP")
				{
					std::cout << "\n" << std::endl;
					std::cout << "Commands suck for now.\n--------------------" << std::endl;

					std::cout << "GamePad::Left Stick = Move camera" << std::endl;
					std::cout << "GamePad::Right Stick = Turn camera" << std::endl;
					std::cout << "GamePad::D-Pad Move = sprite" << std::endl;
					std::cout << "GamePad::Shoulder Buttons = Move camera up/down" << std::endl;
					std::cout << "GamePad::X = Test vibration" << std::endl;
					//std::cout << "GamePad::B = Render Texture on sprite" << std::endl;
					std::cout << "GamePad::A = Move Light to current Camera location" << std::endl;
					std::cout << "\n";
					std::cout << "Keyboard::ArrowKeys = Move sprite" << std::endl;
					std::cout << "Keyboard::L/R Ctrl = Move sprite up/down" << std::endl;
					std::cout << "\n";
					std::cout << "[SYSTEM] = Shwo system information" << std::endl;
					std::cout << "[REGEN] = Regenerate level with culling" << std::endl;
					std::cout << "[REGENNC] = Regenerate level without culling" << std::endl;
					std::cout << "[DIR] = Return views cardinal direction and vertical angle" << std::endl;
					std::cout << "[ROOMS] = Return number of rooms currently rendering" << std::endl;
					std::cout << "[HELP] = Show help" << std::endl;
					std::cout << "[QUIT] = Close Program" << std::endl;
					//std::cout << "Keyboard::T = Test multiply transforms" << std::endl;
					//std::cout << "Keyboard::R = Reset sprite position, orientation and scale" << std::endl;
					//std::cout << "Keyboard::L = Change to orthographic camera" << std::endl;
					//std::cout << "Keyboard::K = Change to perspective camera" << std::endl;


				}
				else if (consoleText == "TODO")
				{
					std::cout << "stuff" << std::endl;
				}
				// room visibility test
				else if (consoleText == "ROOMS")
				{
					std::cout << "Rendering " << g_level->roomsRendered << " Rooms" << std::endl;
				}
				// room visibility test
				else if (consoleText == "CHAR")
				{
					for(int i = 0; i < 255; i++)
					{
						// skip the beep
						if(i == 7)
							continue;

						std::cout << "char# " << i << " - " << (char)i << std::endl;
					}

				}
				// room visibility test
				else if (consoleText == "SYSTEM")
				{
					std::cout << "Using Grapics Card:\n-------------------" << std::endl;
					std::cout << glGetString(GL_VENDOR) << std::endl;
					std::cout << glGetString(GL_RENDERER) << std::endl;
					std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

					std::cout << "\n\n";
				}
				// return direction
				else if (consoleText == "DIR")
				{
					Vector3 f = g_cameraWorld.forward();

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
				// cout test iterator
				else if (consoleText == "ITERATOR")
				{
					std::cout << "Test Iterator 0: " << testIterator_5[0] << std::endl;
					std::cout << "Test Iterator 1: " << testIterator_5[1] << std::endl;
					std::cout << "Test Iterator 2: " << testIterator_5[2] << std::endl;
					std::cout << "Test Iterator 3: " << testIterator_5[3] << std::endl;
					std::cout << "Test Iterator 4: " << testIterator_5[4] << std::endl;
				}
				// regenerate world without culling
				else if (consoleText == "REGENNC")
				{
					toggleCulling = false;

					SceneNode* level = g_rootNode.findChildByName("level");
					g_rootNode.detachChild(*level);

					{ // test level generation
						auto level = make_unique<Level>();

						level->material = &g_materialHolder.get("terrain");
						level->name = "level";
						level->generate();

						g_level = level.get();

						g_rootNode.attachChild(std::move(level));
					}
				}
				// level regeneration
				else if (consoleText == "REGEN")
				{
					toggleCulling = true;
					SceneNode* level = g_rootNode.findChildByName("level");
					g_rootNode.detachChild(*level);

					{ // test level generation
						auto level = make_unique<Level>();

						level->material = &g_materialHolder.get("terrain");
						level->name = "level";
						level->generate();

						g_level = level.get();

						g_rootNode.attachChild(std::move(level));
					}
				}




				else if (consoleText == "QUIT")
				{
					g_window.close();
					g_running = false;
				}
				else
				{
					std::cout << "Invalid command." << std::endl;
				}
				consoleText.clear();
				checkForCommand = false;
			}

			// TEST FUNCTION: delete this
			//  y and z are not multipling correctly when transform * transform
			//if(Input::isKeyPressed(Input::Key::T)) // test multipling transforms
			//{
			//	g_parentTest.scale = g_parentTest.scale * (1.0f + 0.05f * dt.asSeconds());
			//	g_parentTest.position += {0.2f * dt.asSeconds(), 0.0f * dt.asSeconds(), 0.0f * dt.asSeconds()};
			//	
			//	g_player->transform = g_player->transform * g_parentTest;
			//
			//	std::cout << "Scale: " << g_player->transform.scale << std::endl;
			//	std::cout << "Oreintation: " << g_player->transform.orientation << std::endl;
			//	std::cout << "\n";
			//}
			//else if (Input::isKeyPressed(Input::Key::R)) // test multipling transforms
			//{
			//	g_player->transform.position = { 0.0f, 1.0f, 0.0f }; // translation
			//	g_player->transform.scale = { 1.0f, 2.0f, 1.0f };
			//}




			//// help
			//if(Input::isKeyPressed(Input::Key::H))
			//{
			//	std::cout << "\n\n\n\n\n\n\n\n\n\n" << std::endl;
			//	std::cout << "/////////////////////////////////////" << std::endl;
			//
			//	std::cout << "GamePad::Left Stick = Move camera" << std::endl;
			//	std::cout << "GamePad::Right Stick = Turn camera" << std::endl;
			//	std::cout << "GamePad::D-Pad Move = sprite" << std::endl;
			//	std::cout << "GamePad::Shoulder Buttons = Move camera up/down" << std::endl;
			//	std::cout << "GamePad::X = Test vibration" << std::endl;
			//	std::cout << "GamePad::Y = Regenerate level with culling" << std::endl;
			//	//std::cout << "GamePad::B = Render Texture on sprite" << std::endl;
			//	std::cout << "GamePad::A = Move Light to current Camera location" << std::endl;
			//	std::cout << "\n" << std::endl;
			//	std::cout << "Keyboard::ArrowKeys = Move sprite" << std::endl;
			//	std::cout << "Keyboard::L/R Ctrl = Move sprite up/down" << std::endl;
			//	std::cout << "Keyboard::C = Regenerate level without culling" << std::endl;
			//	std::cout << "Keyboard::Space = Return views cardinal direction and vertical angle" << std::endl;
			//	std::cout << "Keyboard::B = Return number of rooms currently rendering" << std::endl;
			//	std::cout << "Keyboard::T = Test multiply transforms" << std::endl;
			//	std::cout << "Keyboard::R = Reset sprite position, orientation and scale" << std::endl;
			//	std::cout << "Keyboard::L = Change to orthographic camera" << std::endl;
			//	std::cout << "Keyboard::K = Change to perspective camera" << std::endl;
			//
			//
			//}

			//if (Input::isKeyPressed(Input::Key::L))
			//{
			//	// test camera swap
			//	g_cameraWorld.transform = g_cameraPlayer.transform;
			//	g_currentCamera = &g_cameraPlayer;
			//	//g_projectionTest = lerp(pp, op, 0.95f);
			//}


			//if (Input::isKeyPressed(Input::Key::K))
			//{
			//	// test camera swap
			//	// TODO: reset forward vector when pressed
			//	g_cameraWorld.transform = g_cameraPlayer.transform;
			//	g_cameraWorld.lookAt(g_player->transform.position);
			//	g_currentCamera = &g_cameraWorld;
			//	//g_projectionTest = lerp(pp, op, 0.01f);
			//}


			Vector3 velocityDirection = { 0, 0, 0 };
		
			// game pad input
			//Input::GamepadAxes axes = Input::getGamepadAxes(Input::Gamepad_1);
			if(Input::isGamepadPresent(0))
			{
				const f32 lookSensitivityX = 2.0f;
				const f32 lookSensitivityY = 1.5f;
				const f32 deadZone = 0.21f;

				//// gamepad camera rotation
				//Vector2 rts = axes.rightThumbStick;

				Vector2 rts = {-Input::getGamepadAxis(0, Input::GamepadAxis::RightX),
							   -Input::getGamepadAxis(0, Input::GamepadAxis::RightY)};
				
				if (Math::abs(rts.x) < deadZone) // ignore anything in the deadZone
					rts.x = 0;
				if (Math::abs(rts.y) < deadZone)
					rts.y = 0;
				
				g_cameraWorld.offsetOrientation(lookSensitivityX * Radian(rts.x * dt.asSeconds()),
												lookSensitivityY * Radian(rts.y * dt.asSeconds()));
				
				// gamepad camera translation
				//Vector2 lts = axes.leftThumbStick;

				Vector2 lts = {Input::getGamepadAxis(0, Input::GamepadAxis::LeftX),
							   -Input::getGamepadAxis(0, Input::GamepadAxis::LeftY)};

				if (Math::abs(lts.x) < deadZone) // ignore anything in the deadZone
					lts.x = 0;
				if (Math::abs(lts.y) < deadZone)
					lts.y = 0;
				
				if(length(Vector2(lts.x, lts.y)) > 1.0f) // keep diagonals from being faster then straight x, y or z
					lts = normalize(lts);
				
				Vector3 camVelocityDirection = { 0, 0, 0 };
				
				Vector3 forward = g_cameraWorld.forward();
				forward = normalize(forward);
				
				camVelocityDirection += lts.x * g_cameraWorld.right();
				camVelocityDirection += lts.y * forward;
				
				//Input::GamepadButtons buttons = Input::getGamepadButtons(Input::Gamepad_1);
				
				if (Input::isGamepadButtonPressed(0, Input::GamepadButton::RightShoulder))
					camVelocityDirection.y += 1;
				if (Input::isGamepadButtonPressed(0, Input::GamepadButton::LeftShoulder))
					camVelocityDirection.y -= 1;
				
				if(length(camVelocityDirection) > 1.0f)
					camVelocityDirection = normalize(camVelocityDirection);
				
				g_cameraWorld.transform.position += camVel * camVelocityDirection * dt.asSeconds();
				
				// gamepad player movement
				if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadUp))
				{
					Vector3 f = g_cameraWorld.forward();
					f.y = 0;
					f = normalize(f);
					velocityDirection += f;
				}
				if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadDown))
				{
					Vector3 b = g_cameraWorld.backward();
					b.y = 0;
					b = normalize(b);
					velocityDirection += b;
				}
				if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadLeft))
				{
					Vector3 l = g_cameraWorld.left();
					l.y = 0;
					l = normalize(l);
					velocityDirection += l;
				}
				if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadRight))
				{
					Vector3 r = g_cameraWorld.right();
					r.y = 0;
					r = normalize(r);
					velocityDirection += r;
				}
				//// vibration test
				//if(Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::X))
				//	Input::setGamepadVibration(Input::Gamepad_1, 0.5f, 0.5f);
				//else
				//	Input::setGamepadVibration(Input::Gamepad_1, 0.0f, 0.0f);
				//
				//
				//// temp variables used in lerp()
				////g_cameraWorld.projectionType = ProjectionType::Perspective;
				////const Matrix4 pp = g_cameraWorld.getProjection(); // perspective projection
				////
				////g_cameraWorld.projectionType = ProjectionType::Orthographic;
				////const Matrix4 op = g_cameraWorld.getProjection(); // perspective projection
				//
				//
				//// 
				//if (Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::B))
				//{
				//	std::cout << "un-mapped button." << std::endl;
				//}
				//
				if (Input::isGamepadButtonPressed(0, Input::GamepadButton::A))
				{
					g_spotLights[0].position.x = g_cameraWorld.transform.position.x;
					g_spotLights[0].position.y = g_cameraWorld.transform.position.y - 0.5f;
					g_spotLights[0].position.z = g_cameraWorld.transform.position.z;
					g_spotLights[0].direction = g_cameraWorld.forward();
				}

			}
			// end Gamepad Input
		//
		//
		//
		//	{ // mouse and keyboard input
				// mouse input
				//Vector2 curPos = Input::getCursorPosition();
				//
				//const f32 mouseSensitivityX = 0.06f;
				//const f32 mouseSensitivityY = 0.09f;
				//
				//g_cameraPlayer.offsetOrientation(-mouseSensitivityX * Radian(curPos.x * dt), -mouseSensitivityY * Radian(curPos.y * dt));
				//
				//Input::setCursorPosition({0, 0});
		
				// keyboard input
				//Vector3& camPos = g_cameraPlayer.transform.position;

				if (Input::isKeyPressed(Input::Key::Up))
					velocityDirection += {0, 0 ,-1};
				if (Input::isKeyPressed(Input::Key::Down))
					velocityDirection += {0, 0, 1};
				if (Input::isKeyPressed(Input::Key::Left))
					velocityDirection += {-1, 0, 0};
				if (Input::isKeyPressed(Input::Key::Right))
					velocityDirection += {1, 0, 0};
		
				if (Input::isKeyPressed(Input::Key::RShift))
					velocityDirection += {0, 1, 0};
				if (Input::isKeyPressed(Input::Key::RControl))
					velocityDirection += {0, -1, 0};
		
				if (length(velocityDirection) > 0)
					velocityDirection = normalize(velocityDirection);
				{
					// update player position
					g_player->transform.position += Vector3{playerVelX, playerVelY, playerVelZ} * velocityDirection * dt.asSeconds();

					// update pplayer orientation
#if 0 // BillBoard
				Quaternion pRot = 
							conjugate(quaternionLookAt(g_player->transform.position,
								g_cameraWorld.transform.position, {0, 1, 0}));

				g_player->transform.orientation = pRot;
#elif 0 // Billboard fixed Y axis
				Vector3 f = g_player->transform.position - g_cameraWorld.transform.position;
				f.y = 0;

				if(f.x == 0 && f.z == 0)
					g_player->transform.orientation = Quaternion();
				else
				{
				Radian a(Math::atan(f.z / f.x));
				a += Radian(Constants::TAU / 4);

				if(f.x < 0) // prevent flipping
					a += Radian(Constants::TAU / 2);

				g_player->transform.orientation = angleAxis(-a, {0, 1, 0});
				}
#endif // end billboard

				}

				// camera follow movement
				{
					//// delta between player and camera movement
					//// this only works correctly if player and camera have 
					//// the same coordinate in the axis being changed
					//f32 dx = player.transform.position.x - g_cameraPlayer.transform.position.x;
					//f32 dy = player.transform.position.y - g_cameraPlayer.transform.position.y;
					//f32 dz = player.transform.position.z - g_cameraPlayer.transform.position.z;
					//
					//// range the player can move before the camera reacts
					//f32 w = 1.0f;
					//
					//// rate at which camera catches up to player
					//f32 speed = 7.0f;
					//
					//f32 dxAbs = Math::abs(dx);
					//f32 dyAbs = Math::abs(dy);
					//f32 dzAbs = Math::abs(dz);
					//
					//// vectors for easier for loop
					//Vector3 v = {dx, dy, dz};
					//Vector3 vAbs = {dxAbs, dyAbs, dzAbs};
					//
					//// creates a box that the player can move in where the camera doesn't move
					//// check each position in vector to check wether to move camera
					//for(int i = 0; i < 3; i++)
					//{
					//	if(vAbs[i] > 1.0f)
					//	{
					//		f32 sgn = v[i] / vAbs[i];
					//		f32 x = vAbs[i] - w;
					//
					//		x = Math::sin(x); // give start/stop an S curve
					//
					//		g_cameraPlayer.transform.position[i] += speed * sgn * x * dt;
					//	}
					//}

					g_cameraPlayer.transform.position.x = Math::lerp(g_cameraPlayer.transform.position.x, g_player->transform.position.x + 8.0f * 3.0f, 12.0f * dt.asSeconds());
					g_cameraPlayer.transform.position.y = Math::lerp(g_cameraPlayer.transform.position.y, g_player->transform.position.y + 8.0f * 2.0f, 12.0f * dt.asSeconds());
					g_cameraPlayer.transform.position.z = Math::lerp(g_cameraPlayer.transform.position.z, g_player->transform.position.z + 8.0f * 3.0f, 12.0f * dt.asSeconds());

				}
				// make player face the camera
				g_cameraPlayer.lookAt({ g_player->transform.position });
				g_player->transform.orientation = -g_cameraWorld.transform.orientation;

				// object culling
				g_level->roomsRendered = 0;
				const Vector3 viewPos = g_cameraWorld.transform.position;
				const Vector3 cameraOrientation = g_cameraWorld.forward();
				const f32 viewDistance = 50;
				if (toggleCulling == true)
				for( auto& room : g_level->rooms)
				{
					Vector3 cullPoint = room->transform.position;

					// check if cullPoint should add room size to itself
					// decide based on the 3d (octo)-quadrant the camera is facing
					if(cameraOrientation.x > 0) // +X
					{
						cullPoint.x += room->size.x;
						if (cameraOrientation.y > 0) // +Y
						{
							cullPoint.y += room->size.y;
							if (cameraOrientation.z > 0) // +Z
								cullPoint.z += room->size.z;
						}
						else // stay -Y
							if (cameraOrientation.z > 0) // +Z
								cullPoint.z += room->size.z;
					}
					else // stay -X
					{
						if (cameraOrientation.y > 0) // +Y
						{
							cullPoint.y += room->size.y;
							if (cameraOrientation.z > 0) // +Z
								cullPoint.z += room->size.z;
						}
						else // stay -Y
							if (cameraOrientation.z > 0) // +Z
								cullPoint.z += room->size.z;
					}

					const Vector3 dp = cullPoint - viewPos;

					const f32 dist = length(dp);

					// distance culling
					if(dist < viewDistance)
					{

						f32 cosTheta = dot(cameraOrientation, normalize(dp));

						Radian theta = Math::acos(cosTheta);

						// cheap cone culling
						if(Math::abs(theta) <= g_cameraWorld.fieldOfView || dist < 20)
						{
							g_level->roomsRendered++;
							room->visible = true;
						}
						else
							room->visible = false;
					}
					else
						room->visible = false;
				}
		} // end update

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					.
		)
		)				RENDER FUNCTIONS
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		INTERNAL void render()
		{

			// check whether to update aspectratio and size each cycle
			if (g_window.currentSize != g_window.getSize())
			{
				Vector2 viewSize = g_window.getSize();

				g_window.currentSize = viewSize;
				g_window.currentAspectRatio = viewSize.x / viewSize.y;
			
				//g_cameraPlayer.viewportAspectRatio = aspectRatio;
				//g_cameraWorld.viewportAspectRatio = aspectRatio;
			}

			g_renderer.reset();
			g_renderer.clearAll();

			g_renderer.addSceneGraph(g_rootNode);

			for (const auto& light : g_directionalLights)
				g_renderer.addDirectionalLight(&light);
			for (const auto& light : g_pointLights)
				g_renderer.addPointLight(&light);
			for (const auto& light : g_spotLights)
				g_renderer.addSpotLight(&light);

			g_renderer.camera = g_currentCamera;

			//g_renderer.quad = g_meshes["quad"];

			g_renderer.gBuffer.create(g_window.currentSize.x, g_window.currentSize.y);

			g_renderer.deferredGeometryPass();
			g_renderer.deferredLightPass();
			g_renderer.deferredFinalPass();

			//g_materialHolder.get("dunjunText").diffuseMap = &g_renderer.finalTexture.colorTexture;

			glViewport(0, 0, g_window.currentSize.x, g_window.currentSize.y);
			glClearColor(0.02f, 0.02f, 0.02f, 1.0f); // set the default color (R,G,B,A)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			{
				ShaderProgram& shaders = g_shaderHolder.get("texturePass");

				shaders.use();
				shaders.setUniform("u_tex", 0);
				shaders.setUniform("u_scale", Vector3(1.0f));

				Texture::bind(&g_renderer.finalTexture.colorTexture, 0);

				g_renderer.draw(&g_meshHolder.get("quad"));

				shaders.stopUsing();
			}


			g_window.display(); // switches information between the front buffer and the back buffer
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					.
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

			//glfwSetErrorCallback(errorCallback);

			Input::setUp();

			Input::setCursorPosition({ 0, 0 });
			//Input::setCursorMode(Input::CursorMode::Disabled);

			//glEnable(GL_CULL_FACE); // enable culling faces
			//glCullFace(GL_BACK); // specify to cull the back face
			//
			//glEnable(GL_DEPTH_TEST);
			//glDepthFunc(GL_LEQUAL);

			// load internal render functions
			loadShaders();

			loadMaterials();

			// load assets
			loadSpriteAsset();
			//loadFloorAsset();

			// load instances
			loadInstances();

			g_world = make_unique<World>(Context{g_window,
												 g_textureHolder,
												 g_shaderHolder,
												 g_meshHolder,
												 g_materialHolder});
		}

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
					handleInput(); // input handler
					update(TIME_STEP);

				}

				if (tc.update(milliseconds(500)) && (g_running == true))
				{
					// dynamic window title
					g_window.setTitle(stringFormat("Dunjun - %.3f ms - %d fps",
												  1000.f / tc.getTickRate(), 
												  (u32)tc.getTickRate()));
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
