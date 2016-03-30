

//#include <Dunjun/Game.hpp> // included in Input.hpp
#include <Dunjun/Input.hpp>

namespace Dunjun
{
	struct ModelAsset // includes the shader, the texture and the vbo's
	{
		Dunjun::ShaderProgram* shaders;
		Dunjun::Texture* texture;

		// create a vertex buffer object to move vertex data to the graphics card
		GLuint vbo;
		GLuint ibo;

		GLenum drawType;
		//GLint drawStart;
		GLint drawCount;
	};

	struct ModelInstance // copies an asset to use
	{
		ModelAsset* asset;
		Dunjun::Transform transform;
		//Dunjun::Matrix4 transform;

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
	GLOBAL std::vector<ModelInstance> g_instances;
	GLOBAL Camera g_camera;

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
				Input::getKey(GLFW_KEY_ESCAPE)) // checks if the escape key is pressed in window
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


			g_defaultShader->bindAttribLocation(0, "a_position"); // bind the position of 1st attribute in shaders
			g_defaultShader->bindAttribLocation(1, "a_color"); // bind the position of 2nd attribute in shaders
			g_defaultShader->bindAttribLocation(2, "a_texCoord"); // bind the position of 3rd attribute in shaders

			if (!g_defaultShader->link())
				throw std::runtime_error(g_defaultShader->errorLog);

		}

		// vertex info, vbo and ibo
		INTERNAL void loadSpriteAsset()
		{
			// Here is where you add vertice information
			//
			Vertex vertices[] = { // define vertexes for a triangle
								  //  x	    y	  z		   r	 g	   b	 a		  s	    t
				{ { 0.5f,  0.5f, 0.0f },{ 0x00, 0xFF, 0xFF, 0xFF },{ 1.0f, 1.0f } },	// 0 vertex         1 ---- 0        
				{ { -0.5f,  0.5f, 0.0f },{ 0xFF, 0xFF, 0x00, 0xFF },{ 0.0f, 1.0f } },	// 1 vertex           \             
				{ { 0.5f, -0.5f, 0.0f },{ 0x00, 0x00, 0xFF, 0xFF },{ 1.0f, 0.0f } },	// 2 vertex              \           
				{ { -0.5f, -0.5f, 0.0f },{ 0xFF, 0x00, 0xFF, 0xFF },{ 0.0f, 0.0f } },	// 3 vertex         3 -----2       
																						// for triangle strips organize vertexes in a backwards Z
			};

			//g_sprite.vbo;
			glGenBuffers(1, &g_sprite.vbo); // make vbo an array with a single entry
			glBindBuffer(GL_ARRAY_BUFFER, g_sprite.vbo); // bind the buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // upload the data to the buffer
																					   /* Types of DRAW
																					   GL_STATIC DRAW   / Draws once and is repeated many times
																					   GL_DYNAMIC_DRAW  / Draw can be changed but repeats more often than it changes
																					   GL_STREAM_DRAW   / Redraws every frame (memory intensive)
																					   */

			u32 indices[] = { 0, 1, 2, 2, 3, 1 }; // vertex draw order for GL_TRIANGLES

			glGenBuffers(1, &g_sprite.ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_sprite.ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



			g_sprite.shaders = g_defaultShader; // apply the default shader to sprite
			g_sprite.texture = new Texture(); // apply new texture to sprite
			g_sprite.texture->loadFromFile("data/textures/dunjunText.jpg");

			g_sprite.drawType = GL_TRIANGLES;
			// g_sprite.drawStart = 0;
			g_sprite.drawCount = 6;
		}

		// create instances of vertex info
		INTERNAL void loadInstances()
		{
			// test multiple transforms
			// FIXME parent cannot pass on 45 degree orientation
			Transform parent;
			parent.position = { 0, 0, 0 };
			parent.orientation = angleAxis(Degree(90), { 1, 0, 0 });
			parent.scale = { 1.0f, 1.0f, 1.0f };

			ModelInstance a;
			a.asset = &g_sprite;
			a.transform.position = { 0, 2, 0 }; // translation
			a.transform.scale = { 3, 3, 3 };
			a.transform.orientation = angleAxis(Degree(0), { 0, 0, 1 }); // rotation
			g_instances.push_back(a);

			ModelInstance b;
			b.asset = &g_sprite;
			b.transform.position = { 2, 0, 0 };
			b.transform.scale = { 1, 1, 1 };
			b.transform.orientation = angleAxis(Degree(0), { 1, 0, 0 }); // rotation
			g_instances.push_back(b);

			ModelInstance c;
			c.asset = &g_sprite;
			c.transform.position = { 0, 0, 1 };
			c.transform.scale = { 1, 1, 1 };
			c.transform.orientation = angleAxis(Degree(0), { 0, 1, 0 }); // rotation
			g_instances.push_back(c);

			for (auto& inst : g_instances)
			{
				auto& t = inst.transform;
				t = parent * t;
			}

			//Initialize camera
			g_camera.transform.position = { 2, 0, 7 };

			g_camera.lookAt({ 0, 0, 0 });
			g_camera.projectionType = ProjectionType::Perspective;
			g_camera.fieldOfView = Degree(50.0f);
		}

		INTERNAL void update(f32 dt)
		{
			f32 camVel = 8.0f; // multiplier for camera speed

			g_instances[0].transform.orientation = angleAxis(Degree(120) * dt, { 1, 0, 0 }) * g_instances[0].transform.orientation;

			//{ // game pad input
			//	if(!g_gamePad1)
			//		g_gamePad1 = new Gamepad(0);
			//
			//	g_gamePad1->update();
			//	if(g_gamePad1->isConnected())
			//	{
			//		printf("Gamepad Connected.\n");
			//		g_gamePad1->setVibration(0.0f, 0.0f);
			//	}
			//	else
			//		printf("No Gamepad Connected.\n");
			//}

			{ // test gamepad input
				if(Input::isGamepadPresent(Input::Gamepad_1))
				{
					Input::GamepadAxes axes = Input::getGamepadAxes(Input::Gamepad_1);

					//std::cout << axes.leftThumbStick << std::endl;

					const f32 lookSensitivityX = 1.5f;
					const f32 lookSensitivityY = 2.0f;

					Vector2 rts = axes.rightThumbStick; // right thumb stick

					g_camera.offsetOrientation(Radian(lookSensitivityX * rts.x * dt), 
											   Radian(lookSensitivityY * -rts.y * dt));

					Vector2 lts = axes.leftThumbStick; // left thumb stick

					Vector3 velocityDirection = { 0, 0, 0 };

					if(length(lts) > 1.0f)
						lts = normalize(lts);

					velocityDirection.x += camVel * lts.x * dt;
					velocityDirection.z -= camVel * lts.y * dt;

					//Input::GamepadButtons buttons = Input::getGamepadButtons(Input::Gamepad_1);
					//
					//if (buttons[(size_t)Input::XboxButton::RightShoulder])
					//	velocityDirection += {0, 1, 0};
					//if (buttons[(size_t)Input::XboxButton::LeftShoulder])
					//	velocityDirection += {0, -1, 0};

					if(length(velocityDirection) > 1.0f)
						velocityDirection = normalize(velocityDirection);

					g_camera.transform.position += velocityDirection * dt;

				}
				else
					std::cout << "Gamepad 1 is Not Connected." << std::endl;

			}



			{ // mouse and keyboard input
				// mouse input
				Vector2 curPos = Input::getCursorPosition();

				const f32 mouseSensitivityX = 0.06f;
				const f32 mouseSensitivityY = 0.09f;

				g_camera.offsetOrientation(Radian(mouseSensitivityX * curPos.x * dt), Radian(mouseSensitivityY * curPos.y * dt));

				Input::setCursorPosition({0, 0});

				// keyboard input
				Vector3& camPos = g_camera.transform.position;

				Vector3 velocityDirection = { 0, 0, 0 };

				if (Input::getKey(GLFW_KEY_UP))
				{
					Vector3 f = g_camera.forward();
					f.y = 0; // prevents y axis pos form being changed
					f = normalize(f);
					velocityDirection += f;
				}
				if (Input::getKey(GLFW_KEY_DOWN))
				{
					Vector3 b = g_camera.backward();
					b.y = 0; // prevents y axis pos form being changed
					b = normalize(b);
					velocityDirection += b;
				}
				if (Input::getKey(GLFW_KEY_LEFT))
				{
					Vector3 l = g_camera.left();
					l.y = 0; // prevents y axis pos form being changed
					l = normalize(l);
					velocityDirection += l;
				}
				if (Input::getKey(GLFW_KEY_RIGHT))
				{
					Vector3 r = g_camera.right();
					r.y = 0; // prevents y axis pos form being changed
					r = normalize(r);
					velocityDirection += r;
				}

				if (Input::getKey(GLFW_KEY_RIGHT_SHIFT))
					velocityDirection += {0, 1, 0};
				if (Input::getKey(GLFW_KEY_RIGHT_CONTROL))
					velocityDirection += {0, -1, 0};

				if (length(velocityDirection) > 0)
					velocityDirection = normalize(velocityDirection);

				camPos += camVel * velocityDirection * dt;

				g_camera.viewportAspectRatio = getWindowSize().x / getWindowSize().y;
			}
		}

		// shader info
		INTERNAL void renderInstance(const ModelInstance& inst)
		{
			ModelAsset* asset = inst.asset;
			Dunjun::ShaderProgram* shaders = asset->shaders;

			shaders->setUniform("u_camera", g_camera.getMatrix()); // set u_camera to apply view functions
																   //shaders->setUniform("u_model", inst.transform); // set u_model to apply matrix transform functions
			shaders->setUniform("u_transform", inst.transform); // set u_model to apply matrix transform functions
			shaders->setUniform("u_tex", (Dunjun::u32)0); // set texture position

			asset->texture->bind(0);
			glBindBuffer(GL_ARRAY_BUFFER, g_sprite.vbo); // bind the buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_sprite.ibo); // bind the buffer

																 // Speicify the layout of the vertex data
			glEnableVertexAttribArray(0); // enables attribute array[0] a_position
			glEnableVertexAttribArray(1); // enables attribute array[1] a_color ''
			glEnableVertexAttribArray(2); // enable attribute [2] a_texCoord

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0); // pointer for attribute position (att position, size of vertices x/y/z, int type, normalized?, stride, pointer)
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const GLvoid*)sizeof(Dunjun::Vector3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(Dunjun::Vector3) + sizeof(Dunjun::Color)));
			// stride says how many floats there are per vertex
			// const void *pointer says how far offset the information starts

			// get the draw info from ModelAsset asset
			//glDrawArrays(asset->drawType, asset->drawStart, asset->drawCount); // (mode to draw in, first vertex, total vertices)
			glDrawElements(asset->drawType, asset->drawCount, GL_UNSIGNED_INT, nullptr);

			glDisableVertexAttribArray(0); // disables attribute array[0]
			glDisableVertexAttribArray(1); // disables attribute array[1]
			glDisableVertexAttribArray(2); // disables attribute array[2]
		}

		INTERNAL void render()
		{
			// vars used to define the size of the viewport
			glViewport(0, 0, windowWidth, windowHeight);

			glClearColor(0.3f, 0.6f, 0.9f, 1.0f); // set the default color (R,G,B,A)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Dunjun::ShaderProgram* currentShaders = nullptr;

			for (const auto& inst : g_instances)
			{
				if (inst.asset->shaders != currentShaders) // swap to new shaders
				{
					currentShaders = inst.asset->shaders;
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

			// Temporarily disable culling
			//glEnable(GL_CULL_FACE); // enable culling faces
			//glCullFace(GL_BACK); // specify to cull the back face
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			// load internal render functions
			loadShaders();
			loadSpriteAsset();
			loadInstances();

			Input::setCursorPosition({ 0, 0 });
			Input::setInputMode(Input::InputMode::Cursor, GLFW_CURSOR_DISABLED);
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

			Dunjun::TickCounter tc;
			Dunjun::Clock frameClock;

			Dunjun::Clock deltaClock;

			f64 accumulator = 0;
			f64 prevTime = glfwGetTime();

			while (running) // create a loop that works until the window closes
			{
				f64 currentTime = glfwGetTime();
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
					titleStream << "Dunjun - ms/F: " << 1000.0 / tc.getTickRate() << " - Your current speed"; // dynamic window title
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
