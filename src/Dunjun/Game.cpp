

//#include <Dunjun/Game.hpp> // included in Input.hpp
#include <Dunjun/Input.hpp>
#include <Dunjun/Scene/SceneNode.hpp>

namespace Dunjun
{

	struct ModelInstance // copies an asset to use
	{
		ModelAsset* asset;
		Transform transform;
	};


	GLOBAL Camera g_cameraPlayer;
	GLOBAL Camera g_cameraWorld;
	GLOBAL Camera* g_currentCamera = &g_cameraPlayer;
	//GLOBAL Matrix4 g_projection;

	namespace
	{
		GLOBAL const f32 TIME_STEP = 1.0f / 60.0f;
		GLOBAL bool g_running = true;
	} // end anon namespace

	GLOBAL ShaderProgram* g_defaultShader;
	GLOBAL ModelAsset g_sprite;
	//GLOBAL ModelAsset g_floor;
	//GLOBAL ModelAsset g_wall;
	GLOBAL std::vector<ModelInstance> g_instances;

	GLOBAL SceneNode g_rootNode;
	GLOBAL SceneNode* g_player;

	GLOBAL Renderer g_renderer;

	GLOBAL std::map<std::string, Material> g_materials;
	GLOBAL std::map<std::string, Mesh*> g_meshes;

	GLOBAL Level g_level;



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
			if (glfwWindowShouldClose(Window::ptr) || // check if window was closed
				Input::isKeyPressed(Input::Key::Escape)) // checks if the escape key is pressed in window
				g_running = false;
		
			if (Input::isKeyPressed(Input::Key::F11)) // press F11 to toggle between default and fullscreen
			{
				Window::isFullscreen = !Window::isFullscreen; // toggles true/false boolean for fullscreen
				if (Window::isFullscreen) // action to take if fullscreen is true
				{
					GLFWwindow* w = Window::createWindow(glfwGetPrimaryMonitor());
					Window::destroyWindow();
					Window::ptr = w;
				}
				else // action to take if fullsscreen is not true
				{
					GLFWwindow* w = Window::createWindow(nullptr);
					Window::destroyWindow();
					Window::ptr = w;
				}

				Window::makeContextCurrent();
		
				//glfwDestroyWindow(Window::ptr); // destroys old window
				Window::swapInterval(1);
		
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
			// Shader Program
			g_defaultShader = new Dunjun::ShaderProgram();
			if (!g_defaultShader->attachShaderFromFile(Dunjun::ShaderType::Vertex, "data/shaders/default_vert.glsl")) // check if the file loaded
				throw std::runtime_error(g_defaultShader->errorLog);

			if (!g_defaultShader->attachShaderFromFile(Dunjun::ShaderType::Fragment, "data/shaders/default_frag.glsl")) // check if the file loaded
				throw std::runtime_error(g_defaultShader->errorLog);


			g_defaultShader->bindAttribLocation((u32)AttribLocation::Position, "a_position"); // bind the position of 1st attribute in shaders
			g_defaultShader->bindAttribLocation((u32)AttribLocation::TexCoord, "a_texCoord"); // bind the position of 3rd attribute in shaders
			g_defaultShader->bindAttribLocation((u32)AttribLocation::Color, "a_color"); // bind the position of 2nd attribute in shaders

			if (!g_defaultShader->link())
				throw std::runtime_error(g_defaultShader->errorLog);

		}

		INTERNAL void loadMaterials()
		{
			g_materials["default"].shaders = g_defaultShader; // apply the default shader to sprite
			g_materials["default"].texture = new Texture(); // apply new texture to sprite
			g_materials["default"].texture->loadFromFile("data/textures/dunjunText.jpg"); // Path to the image

			g_materials["dunjunText"].shaders = g_defaultShader; // apply the default shader to sprite
			g_materials["dunjunText"].texture = new Texture(); // apply new texture to sprite
			g_materials["dunjunText"].texture->loadFromFile("data/textures/dunjunText.jpg"); // Path to the image

			g_materials["stone"].shaders = g_defaultShader; // apply the default shader to sprite
			g_materials["stone"].texture = new Texture(); // apply new texture to sprite
			g_materials["stone"].texture->loadFromFile("data/textures/stone.png", TextureFilter::Nearest); // Path to the image

			g_materials["terrain"].shaders = g_defaultShader; // apply the default shader to sprite
			g_materials["terrain"].texture = new Texture(); // apply new texture to sprite
			g_materials["terrain"].texture->loadFromFile("data/textures/terrain.png", TextureFilter::Nearest); // Path to the image
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
			// Here is where you add vertice information
			//
			Vertex vertices[] = { // define vertexes for a triangle
				//  x	    y	  z		  s	    t	       r	 g	   b	 a				// for triangle strips organize vertexes in a backwards Z
				{ { +0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f },{ 0x00, 0xFF, 0xFF, 0xFF } },	// 0 vertex         1 ---- 0        
				{ { -0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f },{ 0xFF, 0xFF, 0x00, 0xFF } },	// 1 vertex           \             
				{ { +0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f },{ 0x00, 0x00, 0xFF, 0xFF } },	// 2 vertex              \           
				{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f },{ 0xFF, 0x00, 0xFF, 0xFF } },	// 3 vertex         3 -----2       
			};

			u32 indices[] = { 0, 1, 2, 1, 3, 2 }; // vertex draw order for GL_TRIANGLES
		
			Mesh::Data meshData;

			// get number of entries
			int numVertices = sizeof(vertices) / sizeof(vertices[0]);
			int numIndices = sizeof(indices) / sizeof(indices[0]);

			// add the data
			for(int i = 0; i < numVertices; i++)
				meshData.vertices.append(vertices[i].position, vertices[i].texCoord, vertices[i].color);

			for(int i = 0; i <  numIndices; i++)
				meshData.indices.push_back(indices[i]);

			g_meshes["sprite"] = new Mesh(meshData); // NOTE: new Mesh remember to delete

			g_sprite.material = &g_materials["dunjunText"]; // apply material
			g_sprite.mesh = g_meshes["sprite"];
		}

		// generate world objects
		INTERNAL void generateWorld()
		{
			g_level.material = &g_materials["terrain"];
			
			//// number of instances to create
			//int mapSizeX = 24;
			//int mapSizeY = 3;
			//int mapSizeZ = 16;
			//
			//// location of texture in image map
			//Level::TileId lightWoodTile = { 0, 11 };
			//
			//Level::RandomTileSet mossyStoneTiles;
			//for (u32 i = 1; i <= 2; i++)
			//	mossyStoneTiles.emplace_back(Level::TileId{ i, 15 });

			// size of image map
			//f32 tileWidth = 1.0f / 16.0f;
			//f32 tileHeight = 1.0f / 16.0f;
			//
			//// location of texture in image
			//int tileS = 0; // horizontal coordinate
			//int tileT = 15; // vertical coordinate
			//
			//Vertex vertices[] = { // define vertexes for a square
			//	//  x	    y	  z		  s	    t											r	   g	   b	 a	
			//	{ { 0.5f, 0.0f, -0.5f },{ 1.0f, 1.0f },{ 0xFF, 0xFF, 0xFF, 0xFF } },
			//	{ { -0.5f, 0.0f, -0.5f },{ 0.0f, 1.0f },{ 0xFF, 0xFF, 0xFF, 0xFF } },
			//	{ { 0.5f, 0.0f,  0.5f },{ 1.0f, 0.0f },{ 0xFF, 0xFF, 0xFF, 0xFF } },
			//	{ { -0.5f, 0.0f,  0.5f },{ 0.0f, 0.0f },{ 0xFF, 0xFF, 0xFF, 0xFF } },
			//};
			//
			//u32 indices[] = { 0, 1, 2, 1, 3, 2 }; // vertex draw order for GL_TRIANGLES
			//
			//int numVertex = sizeof(vertices) / sizeof(vertices[0]);
			//int numIndices = sizeof(indices) / sizeof(indices[0]);

			//// create array of floor tiles
			//for (int i = 0; i < mapSizeX; i++)
			//{
			//	for (int j = 0; j < mapSizeZ; j++)
			//		g_level.addTileSurface({ (f32)i, 0, (f32)j }, Level::TileSurfaceFace::Up, lightWoodTile);
			//
			//	//size_t index = floorMD.vertices.size();
				//
				//// individual tile
				//for (int k = 0; k < numVertex; k++)
				//{
				//	Dunjun::Vertex v = vertices[k]; // create vertex to carry data
				//
				//	v.position.x = v.position.x + i; // add mapSize increments to create a tile grid
				//	v.position.z = v.position.z + j; // add mapSize increments to create a tile grid
				//
				//	floorMD.vertices.push_back(v);
				//}
				//
				//for (int l = 0; l < numIndices; l++)
				//	floorMD.indices.push_back(index + indices[l]);
			//}
			//
			//// create array of wall tiles
			//// first for loop moves upward from one row to the next
			//for (int i = 0; i < mapSizeY; i++)
			//{
			//	// create row of back wall tiles
			//	for (int j = 0; j < mapSizeX; j++)
			//		g_level.addTileSurface({ (f32)j, (f32)i, 0 }, Level::TileSurfaceFace::Forward, mossyStoneTiles);
			//
			//	// create row of front wall tiles
			//	for (int j = 0; j < mapSizeX; j++)
			//		g_level.addTileSurface({ (f32)j, (f32)i, (f32)mapSizeZ }, Level::TileSurfaceFace::Backward, mossyStoneTiles);
			//
			//	// create row of left wall tiles
			//	for (int j = 0; j < mapSizeZ; j++)
			//		g_level.addTileSurface({ 0, (f32)i, (f32)j }, Level::TileSurfaceFace::Left, mossyStoneTiles);
			//
			//	// create row of right wall tiles
			//	for (int j = 0; j < mapSizeZ; j++)
			//		g_level.addTileSurface({ (f32)mapSizeX, (f32)i, (f32)j }, Level::TileSurfaceFace::Right, mossyStoneTiles);
			//} // end create walls

			g_level.generate();

			g_rootNode.onStart();
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

		// create instances of vertex info
		INTERNAL void loadInstances()
		{
			{ // test scene node
				SceneNode::u_ptr player = make_unique<SceneNode>();

				player->transform.position = { 4.0f, 5.5f, 4.0f };
				player->transform.scale = { 1.0f, 2.0f, 1.0f };
				player->name = "player";

				player->addComponent<MeshRenderer>(g_sprite);
				player->addComponent<FaceCamera>(g_cameraWorld);

				// no idea why this says it's an error
				g_player = player.get();

				g_rootNode.attachChild(std::move(player));
			}
			// test multiple transforms
			// FIXME parent cannot pass on 45 degree orientation
			//Transform parent;
			//parent.position = { 0, 0, 0 };
			//parent.orientation = angleAxis(Degree(90), { 1, 0, 0 });
			//parent.scale = { 1.0f, 1.0f, 1.0f };

			generateWorld();

			//ModelInstance player;
			//player.asset = &g_sprite;
			//player.transform.position = { 4, 1, 4 }; // translation
			//player.transform.scale = { 1, 2, 1 };
			//player.transform.orientation = angleAxis(Degree(0), { 1, 0, 0 }); // rotation

			for(int j = 0; j < g_level.sizeX; j++)
			{
				bool escape = false;
				for(int i = 0; i < g_level.sizeZ; i++)
				{
					if(g_level.mapGrid[i][j] != Level::TileId(0xFFFFFFFF, 0xFFFFFFFF))
					{
						g_player->transform.position = Vector3(i, 1.0f, j);
						escape = true;
						break;
					}
					if(escape == true)
						break;
				}
				if (escape == true)
					break;
			}

			//g_instances.push_back(player);

			{
				//Initialize camera
				g_cameraPlayer.viewportAspectRatio = 16.0f / 9.0f;
				g_cameraPlayer.transform.position = { g_player->transform.position.x - 3, g_player->transform.position.y + 2, g_player->transform.position.z + 3 };
				//g_cameraPlayer.lookAt({ g_cameraPlayer.transform.position.x, g_cameraPlayer.transform.position.y, g_cameraPlayer.transform.position.x - 1 });
				g_cameraPlayer.lookAt(g_player->transform.position);

				g_cameraPlayer.projectionType = ProjectionType::Orthographic;
				g_cameraPlayer.fieldOfView = Degree(50.0f); // for perspective view
				g_cameraPlayer.orthoScale = 10.0f; // for perspective view

				
				// initialize world camera
				g_cameraWorld = g_cameraPlayer;
				g_cameraWorld.projectionType = ProjectionType::Perspective;
				g_cameraWorld.lookAt({ g_cameraWorld.transform.position.x, g_cameraWorld.transform.position.y, g_cameraWorld.transform.position.z - 1 });

				// temp variables used in lerp()
				const Matrix4 op = g_cameraPlayer.getProjection();
				const Matrix4 pp = g_cameraWorld.getProjection();

				/*// initialize projection types
				Matrix4 g_projection;

				// TODO: make Camera::Matrix4 view editable for blending
				g_projection = lerp(pp, op, 0.95f); // mostly orthographic
				g_cameraPlayer.getView() * g_projection;
				g_projection = lerp(pp, op, 0.01f); // mostly perspective
				g_cameraWorld.getView() * g_projection; */
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

		INTERNAL void update(f32 dt)
		{
			// test scene node
			//SceneNode* playerPtr = g_rootNode.findChildByName("player");
			g_rootNode.update(dt);

			//ModelInstance &player = g_instances[0];
			//g_instances[0].transform.position.x = std::sin(3.0f * Input::getTime());
			//g_instances[0].transform.position.z = std::cos(3.0f * Input::getTime());

			f32 camVel = 8.0f; // multiplier for camera speed
			f32 playerVelX = 3.5f;
			f32 playerVelZ = 5.5f;

			// TEST FUNCTION: delete this
			//  y and z are not multipling correctly when transform * transform
			if(Input::isKeyPressed(Input::Key::T)) // test multipling transforms
			{
			Transform parent;

			parent.orientation = angleAxis(Degree(360 * std::sin(glfwGetTime())) * dt / 5, { 1, 0, 0 }) * parent.orientation;
			
			g_player->transform = g_player->transform * parent;

			std::cout << "Scale: " << g_player->transform.scale << std::endl;
			std::cout << "Oreintation: " << g_player->transform.orientation << std::endl;
			std::cout << "\n";
			}
			else if (Input::isKeyPressed(Input::Key::R)) // test multipling transforms
			{
				g_player->transform.position = { 0, 0, 0 }; // translation
				g_player->transform.scale = { 1, 1, 1 };
				g_player->transform.orientation = angleAxis(Degree(0), { 0, 0, 1 }); // rotation
			}

			//g_instances[2].transform.orientation = angleAxis(Degree(120) * dt, { 1, 0, 0 }) * g_instances[2].transform.orientation;
		
			// game pad input
				Input::GamepadAxes axes = Input::getGamepadAxes(Input::Gamepad_1);
		
				const f32 lookSensitivityX = 2.0f;
				const f32 lookSensitivityY = 1.5f;
				const f32 deadZone = 0.21f;
		
				// gamepad camera rotation
				Vector2 rts = axes.rightThumbStick;
		
				if (std::abs(rts.x) < deadZone) // ignore anything in the deadZone
					rts.x = 0;
				if (std::abs(rts.y) < deadZone)
					rts.y = 0;
		
				g_cameraWorld.offsetOrientation(lookSensitivityX * Radian(rts.x * dt)
										  , lookSensitivityY * Radian(rts.y * dt));
		
				// gamepad camera translation
				Vector2 lts = axes.leftThumbStick;
		
				if (std::abs(lts.x) < deadZone) // ignore anything in the deadZone
					lts.x = 0;
				if (std::abs(lts.y) < deadZone)
					lts.y = 0;
		
				if(length(lts) > 1.0f) // keep diagonals from being faster then straight x, y or z
					lts = normalize(lts);

				Vector3 velocityDirection = { 0, 0, 0 };
				Vector3 camVelocityDirection = { 0, 0, 0 };
		
				Vector3 forward = g_cameraWorld.forward();
				forward.y = 0;
				forward = normalize(forward);
		
				camVelocityDirection += lts.x * g_cameraWorld.right();
				camVelocityDirection += lts.y * forward;
		
				Input::GamepadButtons buttons = Input::getGamepadButtons(Input::Gamepad_1);
		
				if (buttons[(size_t)Input::XboxButton::RightShoulder])
					camVelocityDirection.y += 1;
				if (buttons[(size_t)Input::XboxButton::LeftShoulder])
					camVelocityDirection.y -= 1;

				if(length(camVelocityDirection) > 1.0f)
					camVelocityDirection = normalize(camVelocityDirection);
		
				g_cameraWorld.transform.position += camVel * camVelocityDirection * dt;
		
				// gamepad player movement
				if (buttons[(size_t)Input::XboxButton::DpadUp])
				{
					Vector3 f = g_cameraWorld.forward();
					f.y = 0;
					f = normalize(f);
					velocityDirection += f;
				}
				if (buttons[(size_t)Input::XboxButton::DpadDown])
				{
					Vector3 b = g_cameraWorld.backward();
					b.y = 0;
					b = normalize(b);
					velocityDirection += b;
				}
				if (buttons[(size_t)Input::XboxButton::DpadLeft])
				{
					Vector3 l = g_cameraWorld.left();
					l.y = 0;
					l = normalize(l);
					velocityDirection += l;
				}
				if (buttons[(size_t)Input::XboxButton::DpadRight])
				{
					Vector3 r = g_cameraWorld.right();
					r.y = 0;
					r = normalize(r);
					velocityDirection += r;
				}

				// vibration test
				if(Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::X))
					Input::setGamepadVibration(Input::Gamepad_1, 0.5f, 0.5f);
				else
					Input::setGamepadVibration(Input::Gamepad_1, 0.0f, 0.0f);


				// temp variables used in lerp()
				//g_cameraWorld.projectionType = ProjectionType::Perspective;
				//const Matrix4 pp = g_cameraWorld.getProjection(); // perspective projection
				//
				//g_cameraWorld.projectionType = ProjectionType::Orthographic;
				//const Matrix4 op = g_cameraWorld.getProjection(); // perspective projection


				// projection type swap
				if (Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::B))
				{
					// test camera swap
					g_cameraWorld.transform = g_cameraPlayer.transform;
					g_currentCamera = &g_cameraPlayer;
					//g_projectionTest = lerp(pp, op, 0.95f);
				}
				if (Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::A))
				{
					// test camera swap
					// TODO: reset forward vector when pressed
					g_cameraWorld.transform = g_cameraPlayer.transform;
					g_cameraWorld.transform.position.y = g_cameraWorld.transform.position.y + 1.0f;
					g_cameraWorld.lookAt({ g_cameraWorld.transform.position.x, g_cameraWorld.transform.position.y, g_cameraWorld.transform.position.z - 1});
					g_currentCamera = &g_cameraWorld;
					//g_projectionTest = lerp(pp, op, 0.01f);
				}

				if (Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::Y))
				{
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
					g_player->transform.position += Vector3{playerVelX, 0, playerVelZ} * velocityDirection * dt;

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
				Radian a(std::atan(f.z / f.x));
				a += Radian(Constants::TAU / 4);

				if(f.x < 0) // prevent flipping
					a += Radian(Constants::TAU / 2);

				g_player->transform.orientation = angleAxis(-a, {0, 1, 0});
				}
#endif // end billboard

				}

				// update aspect ratio each update
				f32 aspectRatio = Window::getFramebufferSize().x / Window::getFramebufferSize().y;
				if (aspectRatio && Window::getFramebufferSize().y > 0)
				{
					g_cameraPlayer.viewportAspectRatio = aspectRatio;
					g_cameraWorld.viewportAspectRatio = aspectRatio;
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
					//f32 dxAbs = std::abs(dx);
					//f32 dyAbs = std::abs(dy);
					//f32 dzAbs = std::abs(dz);
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
					//		x = std::sin(x); // give start/stop an S curve
					//
					//		g_cameraPlayer.transform.position[i] += speed * sgn * x * dt;
					//	}
					//}

					g_cameraPlayer.transform.position.x = lerp(g_cameraPlayer.transform.position.x - 3, g_player->transform.position.x, 10.0f * dt);
					g_cameraPlayer.transform.position.z = lerp(g_cameraPlayer.transform.position.z + 3, g_player->transform.position.z, 10.0f * dt);

					g_cameraPlayer.lookAt({ g_player->transform.position.x, g_player->transform.position.y + 0.5f, g_player->transform.position.z });

				}


		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					.
		)
		)				RENDER FUNCTIONS
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


		// shader info
		INTERNAL void renderInstance(const ModelInstance& inst)
		{
			ModelAsset* asset = inst.asset;
			Dunjun::ShaderProgram* shaders = asset->material->shaders;

			shaders->setUniform("u_camera", g_currentCamera->getMatrix()); // shaderprogram.cpp
			shaders->setUniform("u_transform", inst.transform); // shaderprogram.cpp
			shaders->setUniform("u_tex", (Dunjun::u32)0); // shaderprogram.cpp

			asset->mesh->draw(); // mesh.cpp
		}

		INTERNAL void renderLevel(const Level& level)
		{
			Dunjun::ShaderProgram* shaders = level.material->shaders;

			shaders->setUniform("u_camera", g_currentCamera->getMatrix()); // shaderprogram.cpp
			shaders->setUniform("u_transform", level.transform); // shaderprogram.cpp
			shaders->setUniform("u_tex", (Dunjun::u32)0); // shaderprogram.cpp

			level.mesh->draw();
		}

		INTERNAL void render()
		{
			//{
			//	// vars used to define the size of the viewport
			//	Vector2 fbSize = Window::getFramebufferSize();
			//	glViewport(0, 0, fbSize.x, fbSize.y);
			//}

			glClearColor(0.02f, 0.02f, 0.02f, 1.0f); // set the default color (R,G,B,A)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			const Dunjun::ShaderProgram* currentShaders = nullptr;
			const Texture* currentTexture = nullptr;

			
			// render instances
			for (const auto& inst : g_instances)
			{
				if (inst.asset->material->shaders != currentShaders) // swap to new shaders
				{
					if (currentShaders) // checkif currentshader is in use
						currentShaders->stopUsing();

					currentShaders = inst.asset->material->shaders;
					currentShaders->use();
				}

				if (inst.asset->material->texture != currentTexture) // swap to new shaders
				{
					currentTexture = inst.asset->material->texture;
					Texture::bind(currentTexture, 0);
				}

				renderInstance(inst);
			}

			//render level
			{
				if (g_level.material->shaders != currentShaders) // swap to new shaders
				{
					if (currentShaders) // checkif currentshader is in use
						currentShaders->stopUsing();
			
					currentShaders = g_level.material->shaders;
					currentShaders->use();
				}
			
				if (g_level.material->texture != currentTexture) // swap to new shaders
				{
					currentTexture = g_level.material->texture;
					Texture::bind(currentTexture, 0);
				}
			
				renderLevel(g_level);
			}

			if (currentShaders) // checkif currentshader is in use
				currentShaders->stopUsing();

			Texture::bind(nullptr, 0); // unbind texture

			g_renderer.setCamera(*g_currentCamera);

			g_rootNode.draw(g_renderer);

			g_renderer.reset();

			Window::swapBuffers();; // switches information between the front buffer and the back buffer
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

			if(!Window::init())
				return;


			glewInit();

			// Initial OpenGL settings
			glInit();

			//glfwSetErrorCallback(errorCallback);

			Input::setUp();

			//Input::setCursorPosition({ 0, 0 });
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
		}

		void run()
		{
			std::stringstream titleStream;

			//=================================================================================================
			// OPENING THE MAIN WINDOW							 
			//=================================================================================================
			//=================================================================================================

			TickCounter tc;
			Clock frameClock;

			Clock deltaClock;

			f64 accumulator = 0;
			f64 prevTime = Input::getTime();

			while (g_running) // create a loop that works until the window closes
			{
				//Window::pollEvents();

				Window::makeContextCurrent();

				f64 currentTime = Input::getTime();
				f64 dt = currentTime - prevTime;
				prevTime = currentTime;
				accumulator += dt;

				// limit accumulator size
				if (accumulator > 1.2f)
					accumulator = 1.2f;

				// render update
				while (accumulator >= TIME_STEP)
				{
					accumulator -= TIME_STEP;
					Window::pollEvents();
					handleInput(); // input handler
					Input::updateGamepads();
					update(TIME_STEP);

				}

				if (tc.update(0.5))
				{
					//std::cout << tc.getTickRate() << std::endl;
					titleStream.str("");
					titleStream.clear();
					titleStream << "Dunjun - F/S: ~" <<  tc.getTickRate() << " - Your current speed"; // dynamic window title
					Window::setTitle(titleStream.str().c_str());
				}

				render();

				// framerate limiter
				while (frameClock.getElapsedTime() < 1.0 / 240.0)
					;
				frameClock.restart();

			}
		}

		void cleanUp()
		{
			Input::cleanup();
			Window::cleanup();
		}

	} // end Game
} // end Dunjun
