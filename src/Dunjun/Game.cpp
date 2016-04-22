

//#include <Dunjun/Game.hpp> // included in Input.hpp
#include <Dunjun/Input.hpp>

namespace Dunjun
{
	struct Material
	{
		Dunjun::ShaderProgram* shaders;
		Dunjun::Texture* texture;
	};

	struct ModelAsset // includes the shader, the texture and the vbo's
	{
		const Material *material;
		const Mesh *mesh;
	};

	struct ModelInstance // copies an asset to use
	{
		ModelAsset* asset;
		Dunjun::Transform transform;
	};

	namespace
	{
		const f32 TIME_STEP = 1.0f / 60.0f;

		GLFWwindow* window;

		int windowWidth = 854;
		int windowHeight = 480;
	} // end anon namespace

	GLOBAL ShaderProgram* g_defaultShader;
	GLOBAL ModelAsset g_sprite;
	//GLOBAL ModelAsset g_floor;
	//GLOBAL ModelAsset g_wall;
	GLOBAL std::vector<ModelInstance> g_instances;
	GLOBAL Camera g_camera;
	GLOBAL std::map<std::string, Material> g_materials;
	GLOBAL std::map<std::string, Mesh*> g_meshes;

	namespace Game
	{
		INTERNAL void glfwHints() // use this when creating a window to define what version of glfw is used
		{
			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_VERSION_MAJOR, 2);
			glfwWindowHint(GLFW_VERSION_MINOR, 1);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		}

		INTERNAL void resizeCallback(GLFWwindow* window, int width, int height)
		{
			windowWidth = width;
			windowHeight = height;
		}

		INTERNAL void handleInput(bool* running, bool* fullscreen)
		{
			if (glfwWindowShouldClose(window) || // check if window was closed
				Input::isKeyPressed(Input::Key::Escape)) // checks if the escape key is pressed in window
				*running = false;

			/*
			if (Input::getKey(GLFW_KEY_F11)) // press F11 to toggle between default and fullscreen
			{
			*fullscreen = !(*fullscreen); // toggles true/false boolean for fullscreen

			GLFWwindow* newwindow;

			glfwHints(); // define glfw version before opening a window
			if (*fullscreen) // action to take if fullscreen is true
			{
			int count;												// declares mode count for monitor
			const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count); // creates GLFWvidmode array of monitor modes
			newwindow = glfwCreateWindow(modes[count - 1].width,	// modes[count - 1].width gets the width from the last mode
			modes[count - 1].height,								// modes[count - 1].height gets the height from the last mode
			"Dunjun", glfwGetPrimaryMonitor(), window);				// and create the window
			}
			else // action to take if fullsscreen is not true
			{
			newwindow = glfwCreateWindow(G_windowwidth, G_windowheight, "Dunjun", nullptr, window);
			}

			glfwDestroyWindow(window); // destroys old window
			window = newwindow;
			glfwMakeContextCurrent(window);
			}
			*/
		}

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
		)					LOAD ASSETS
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
				{ { 0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f },{ 0x00, 0xFF, 0xFF, 0xFF } },	// 0 vertex         1 ---- 0        
				{ { -0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f },{ 0xFF, 0xFF, 0x00, 0xFF } },	// 1 vertex           \             
				{ { 0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f },{ 0x00, 0x00, 0xFF, 0xFF }},	// 2 vertex              \           
				{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f },{ 0xFF, 0x00, 0xFF, 0xFF } },	// 3 vertex         3 -----2       
			};

			u32 indices[] = { 0, 1, 2, 1, 3, 2 }; // vertex draw order for GL_TRIANGLES
		
			Mesh::Data meshData;

			int numVertex = sizeof(vertices) / sizeof(vertices[0]);
			int numIndices = sizeof(indices) / sizeof(indices[0]);

			for(int i = 0; i < numVertex; i++)
				meshData.vertices.push_back(vertices[i]);

			for(int i = 0; i <  numIndices; i++)
				meshData.indices.push_back(indices[i]);

			g_meshes["sprite"] = new Mesh(meshData); // NOTE: new Mesh remember to delete

			g_sprite.material = &g_materials["dunjunText"]; // apply material
			g_sprite.mesh = g_meshes["sprite"];
		}

		// generate world objects
		INTERNAL void generateWorld()
		{
			Mesh::Data floorMD;
			
			// number of instances to create
			int mapSizeX = 8;
			int mapSizeY = 3;
			int mapSizeZ = 7;

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

			// create array of floor tiles
			for (int i = 0; i < mapSizeX; i++)
			{
				for (int j = 0; j < mapSizeZ; j++)
				{
					// size of image map
					f32 tileWidth = 1.0f / 16;
					f32 tileHeight = 1.0f / 16;

					// location of texture in image map
					int tileS = 0; // horizontal coordinate
					int tileT = 11; // vertical coordinate

					size_t index = floorMD.vertices.size();

					floorMD.vertices.push_back({ { 0.5f + i, 0.0f, -0.5f + j },{ (tileS + 1)*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f + i, 0.0f, -0.5f + j },{ tileS*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { 0.5f + i, 0.0f,  0.5f + j },{ (tileS + 1)*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f + i, 0.0f,  0.5f + j },{ tileS*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });

					floorMD.indices.push_back(index + 0);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 2);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 3);
					floorMD.indices.push_back(index + 2);


					//size_t index = floorMD.vertices.size();
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
				}
			}

			// create array of wall tiles
			// first for loop moves upward from one row to the next
			for (int i = 0; i < mapSizeY; i++)
			{
				// create row of back wall tiles
				for (int j = 0; j < mapSizeX; j++)
				{
					// size of image map
					f32 tileWidth = 1.0f / 16;
					f32 tileHeight = 1.0f / 16;

					// location of texture in image map
					int tileS = 1 + rand() % 2; // horizontal coordinate
					int tileT = 15; // vertical coordinate

					size_t index = floorMD.vertices.size();

					floorMD.vertices.push_back({ { 0.5f + j, 1.0f + i, -0.5f},{ (tileS + 1)*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f + j, 1.0f + i, -0.5f},{ tileS*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { 0.5f + j, 0.0f + i,  -0.5f},{ (tileS + 1)*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f + j, 0.0f + i,  -0.5f},{ tileS*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });

					floorMD.indices.push_back(index + 0);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 2);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 3);
					floorMD.indices.push_back(index + 2);

				}

			// create row of front wall tiles
				for (int j = 0; j < mapSizeX; j++)
				{
					// size of image map
					f32 tileWidth = 1.0f / 16;
					f32 tileHeight = 1.0f / 16;

					// location of texture in image map
					int tileS = 1 + rand() % 2; // horizontal coordinate
					int tileT = 15; // vertical coordinate

					size_t index = floorMD.vertices.size();

					floorMD.vertices.push_back({ { -0.5f + j, 1.0f + i, -0.5f + mapSizeZ },{ (tileS + 1)*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { 0.5f + j, 1.0f + i, -0.5f + mapSizeZ },{ tileS*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f + j, 0.0f + i,  -0.5f + mapSizeZ },{ (tileS + 1)*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { 0.5f + j, 0.0f + i,  -0.5f + mapSizeZ },{ tileS*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });

					floorMD.indices.push_back(index + 0);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 2);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 3);
					floorMD.indices.push_back(index + 2);

				}

			// create row of left wall tiles
				for (int j = 0; j < mapSizeZ; j++)
				{
					// size of image map
					f32 tileWidth = 1.0f / 16;
					f32 tileHeight = 1.0f / 16;

					// location of texture in image map
					int tileS = 1 + rand() % 2; // horizontal coordinate
					int tileT = 15; // vertical coordinate

					size_t index = floorMD.vertices.size();

					floorMD.vertices.push_back({ { -0.5f, 1.0f + i, -0.5f + j },{ (tileS + 1)*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f, 1.0f + i, 0.5f + j },{ tileS*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f, 0.0f + i,  -0.5f + j },{ (tileS + 1)*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f, 0.0f + i,  0.5f + j },{ tileS*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });

					floorMD.indices.push_back(index + 0);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 2);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 3);
					floorMD.indices.push_back(index + 2);

				}

			// create row of right wall tiles
				for (int j = 0; j < mapSizeZ; j++)
				{
					// size of image map
					f32 tileWidth = 1.0f / 16;
					f32 tileHeight = 1.0f / 16;

					// location of texture in image map
					int tileS = 1 + rand() % 2; // horizontal coordinate
					int tileT = 15; // vertical coordinate

					size_t index = floorMD.vertices.size();

					floorMD.vertices.push_back({ { -0.5f + mapSizeX, 1.0f + i, 0.5f + j },{ (tileS + 1)*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f + mapSizeX, 1.0f + i, -0.5f + j },{ tileS*tileWidth, (tileT + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f + mapSizeX, 0.0f + i,  0.5f + j },{ (tileS + 1)*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
					floorMD.vertices.push_back({ { -0.5f + mapSizeX, 0.0f + i,  -0.5f + j },{ tileS*tileWidth, tileT*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });

					floorMD.indices.push_back(index + 0);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 2);
					floorMD.indices.push_back(index + 1);
					floorMD.indices.push_back(index + 3);
					floorMD.indices.push_back(index + 2);

				}
			} // end create walls


			ModelInstance floorMI;

			floorMI.asset = new ModelAsset();
			floorMI.asset->mesh = new Mesh(floorMD);

			floorMI.asset->material = &g_materials["terrain"]; // apply material

			g_instances.push_back(floorMI);
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
			// test multiple transforms
			// FIXME parent cannot pass on 45 degree orientation
			//Transform parent;
			//parent.position = { 0, 0, 0 };
			//parent.orientation = angleAxis(Degree(90), { 1, 0, 0 });
			//parent.scale = { 1.0f, 1.0f, 1.0f };

			ModelInstance a;
			a.asset = &g_sprite;
			a.transform.position = { 0, 0.5, 0 }; // translation
			a.transform.scale = { 1, 1, 1 };
			a.transform.orientation = angleAxis(Degree(0), { 1, 0, 0 }); // rotation
			g_instances.push_back(a);

			generateWorld();

			//for (auto& inst : g_instances)
			//{
			//	auto& t = inst.transform;
			//	t = parent * t;
			//}

			//Initialize camera
			g_camera.transform.position = { 0, 3, 10 };

			g_camera.lookAt({ 0, 0, 0 });

			g_camera.projectionType = ProjectionType::Perspective;

			//g_camera.projectionType = ProjectionType::Orthographic;

			g_camera.fieldOfView = Degree(50.0f); // for perspective view
			g_camera.orthoScale = 4.0f; // for ortho view
		}

		INTERNAL void update(f32 dt)
		{
			ModelInstance &player = g_instances[0];
			//g_instances[0].transform.position.x = std::sin(3.0f * Input::getTime());
			//g_instances[0].transform.position.z = std::cos(3.0f * Input::getTime());


			f32 camVel = 8.0f; // multiplier for camera speed
			f32 playerVel = 2.5f;

			// TEST FUNCTION: delete this
			//  y and z are not multipling correctly when transform * transform
			if(Input::isKeyPressed(Input::Key::T)) // test multipling transforms
			{
			Transform parent;

			parent.orientation = angleAxis(Degree(360 * std::sin(glfwGetTime())) * dt / 5, { 1, 0, 0 }) * parent.orientation;
			
			player.transform = player.transform * parent;

			std::cout << "Scale: " << player.transform.scale << std::endl;
			std::cout << "Oreintation: " << player.transform.orientation << std::endl;
			std::cout << "\n";
			}
			else if (Input::isKeyPressed(Input::Key::R)) // test multipling transforms
			{
				player.transform.position = { 0, 0, 0 }; // translation
				player.transform.scale = { 1, 1, 1 };
				player.transform.orientation = angleAxis(Degree(0), { 0, 0, 1 }); // rotation
			}

			//g_instances[2].transform.orientation = angleAxis(Degree(120) * dt, { 1, 0, 0 }) * g_instances[2].transform.orientation;
		
			{ // game pad input
				Input::GamepadAxes axes = Input::getGamepadAxes(Input::Gamepad_1);
		
				const f32 lookSensitivityX = 2.0f;
				const f32 lookSensitivityY = 1.5f;
				const f32 deadZone = 0.21f;
		
				// camera rotation
				Vector2 rts = axes.rightThumbStick;
		
				if (std::abs(rts.x) < deadZone) // ignore anything in the deadZone
					rts.x = 0;
				if (std::abs(rts.y) < deadZone)
					rts.y = 0;
		
				g_camera.offsetOrientation(-lookSensitivityX * Radian(rts.x * dt)
										  , lookSensitivityY * Radian(rts.y * dt));
		
				// camera translation
				Vector2 lts = axes.leftThumbStick;
		
				if (std::abs(lts.x) < deadZone) // ignore anything in the deadZone
					lts.x = 0;
				if (std::abs(lts.y) < deadZone)
					lts.y = 0;
		
				if(length(lts) > 1.0f) // keep diagonals from being faster then straight x, y or z
					lts = normalize(lts);
		
				Vector3 velocityDirection = {0, 0, 0};
		
				Vector3 forward = g_camera.forward();
				forward.y = 0;
				forward = normalize(forward);
		
				velocityDirection += lts.x * g_camera.right();
				velocityDirection += lts.y * forward;
		
				Input::GamepadButtons buttons = Input::getGamepadButtons(Input::Gamepad_1);
		
				if (buttons[(size_t)Input::XboxButton::RightShoulder])
					velocityDirection.y += 1;
				if (buttons[(size_t)Input::XboxButton::LeftShoulder])
					velocityDirection.y -= 1;
		
				if (buttons[(size_t)Input::XboxButton::DpadUp])
				{
					Vector3 f = g_camera.forward();
					f.y = 0;
					f = normalize(f);
					velocityDirection += f;
				}
				if (buttons[(size_t)Input::XboxButton::DpadDown])
				{
					Vector3 b = g_camera.backward();
					b.y = 0;
					b = normalize(b);
					velocityDirection += b;
				}
				if (buttons[(size_t)Input::XboxButton::DpadLeft])
				{
					Vector3 l = g_camera.left();
					l.y = 0;
					l = normalize(l);
					velocityDirection += l;
				}
				if (buttons[(size_t)Input::XboxButton::DpadRight])
				{
					Vector3 r = g_camera.right();
					r.y = 0;
					r = normalize(r);
					velocityDirection += r;
				}
		
				if(length(velocityDirection) > 1.0f)
					velocityDirection = normalize(velocityDirection);
		
				g_camera.transform.position += camVel * velocityDirection * dt;
		
				// vibration test
				if(Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::X))
					Input::setGamepadVibration(Input::Gamepad_1, 0.5f, 0.5f);
				else
					Input::setGamepadVibration(Input::Gamepad_1, 0.0f, 0.0f);

				// camera swap
				if (Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::B))
					g_camera.projectionType = ProjectionType::Orthographic;
				if (Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::A))
					g_camera.projectionType = ProjectionType::Perspective;
				if (Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::Y))
					g_camera.projectionType = ProjectionType::InfinitePerspective;
			}
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
				//g_camera.offsetOrientation(-mouseSensitivityX * Radian(curPos.x * dt), -mouseSensitivityY * Radian(curPos.y * dt));
				//
				//Input::setCursorPosition({0, 0});
		
				// keyboard input
				//Vector3& camPos = g_camera.transform.position;

				Vector3 velocityDirection = { 0, 0, 0 };
		
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
				//camPos += camVel * velocityDirection * dt;
				player.transform.position += playerVel * velocityDirection * dt;

#if 0 // BillBoard
				Quaternion pRot = 
							conjugate(quaternionLookAt(player.transform.position, 
							g_camera.transform.position, {0, 1, 0}));

				player.transform.orientation = pRot;
#elif 0 // Billboard fixed Y axis
				Vector3 f = player.transform.position - g_camera.transform.position;
				f.y = 0;

				if(f.x == 0 && f.z == 0)
					player.transform.orientation = Quaternion();
				else
				{
				Radian a(std::atan(f.z / f.x));
				a += Radian(Constants::TAU / 4);

				if(f.x < 0) // prevent flipping
					a += Radian(Constants::TAU / 2);

				player.transform.orientation = angleAxis(-a, {0, 1, 0});
				}

#endif

				}
				//
		//		// change fov with scroll wheel
		//		// FIXME: view goes insane when scroll is used
		//		// g_camera.fieldOfView = Radian(static_cast<f32>(g_camera.fieldOfView) + Input::getScrollOffset().y);
		//
		//	}

				//g_camera.transform.position.x = player.transform.position.x;
				//g_camera.lookAt(player.transform.position, {0, 1, 0});
				g_camera.viewportAspectRatio = getWindowSize().x / getWindowSize().y;
		
		}

		// shader info
		INTERNAL void renderInstance(const ModelInstance& inst)
		{
			ModelAsset* asset = inst.asset;
			Dunjun::ShaderProgram* shaders = asset->material->shaders;

			shaders->setUniform("u_camera", g_camera.getMatrix()); // shaderprogram.cpp
			shaders->setUniform("u_transform", inst.transform); // shaderprogram.cpp
			shaders->setUniform("u_tex", (Dunjun::u32)0); // shaderprogram.cpp

			asset->material->texture->bind(0); // texture.cpp

			asset->mesh->draw(); // mesh.cpp
		}

		INTERNAL void render()
		{
			// vars used to define the size of the viewport
			glViewport(0, 0, windowWidth, windowHeight);

			glClearColor(0.02f, 0.02f, 0.02f, 1.0f); // set the default color (R,G,B,A)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Dunjun::ShaderProgram* currentShaders = nullptr;

			for (const auto& inst : g_instances)
			{
				if (inst.asset->material->shaders != currentShaders) // swap to new shaders
				{
					currentShaders = inst.asset->material->shaders;
					currentShaders->use();
				}
				renderInstance(inst);

			}

			if (currentShaders) // checkif currentshader is in use
				currentShaders->stopUsing();

			glfwSwapBuffers(window); // switches information between the front buffer and the back buffer
			glfwPollEvents(); // waits for input and considers that input lower priorty than interrupt input

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
			if(!glfwInit())
				return;

			glfwHints();
			window = glfwCreateWindow(windowWidth, windowHeight, "Dunjun", nullptr, nullptr);

			if(!window)
			{
				glfwTerminate();
				return;
			}
			glfwSetWindowSizeCallback(window, resizeCallback);
			glfwMakeContextCurrent(window); // set the context for the window

			glewInit();

			Input::setUp();

			//Input::setCursorPosition({ 0, 0 });
			//Input::setCursorMode(Input::CursorMode::Disabled);

			glEnable(GL_CULL_FACE); // enable culling faces
			glCullFace(GL_BACK); // specify to cull the back face

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

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
			bool running = true;
			bool fullscreen = false; // sets fullscreen to be off by default

			TickCounter tc;
			Clock frameClock;

			Clock deltaClock;

			f64 accumulator = 0;
			f64 prevTime = Input::getTime();

			while (running) // create a loop that works until the window closes
			{
				f64 currentTime = Input::getTime();
				f64 dt = currentTime - prevTime;
				prevTime = currentTime;
				accumulator += dt;


				// render update
				while (accumulator >= TIME_STEP)
				{
					accumulator -= TIME_STEP;
					update(TIME_STEP);
					Input::updateGamepads();
					handleInput(&running, &fullscreen); // input handler

				}

				if (tc.update(0.5))
				{
					//std::cout << tc.getTickRate() << std::endl;
					titleStream.str("");
					titleStream.clear();
					titleStream << "Dunjun - F/S: ~" <<  tc.getTickRate() << " - Your current speed"; // dynamic window title
					glfwSetWindowTitle(window, titleStream.str().c_str());
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
			glfwDestroyWindow(window); // closes window named window
			glfwTerminate(); // terminates GLFW
		}

		GLFWwindow* getGlfwWindow()
		{
			return window;
		}

		Vector2 getWindowSize()
		{
			return Vector2(windowWidth, windowHeight);
		}



	} // end Game
} // end Dunjun
