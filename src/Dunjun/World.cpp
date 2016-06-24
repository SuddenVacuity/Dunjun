
#include <Dunjun/World.hpp>

namespace Dunjun
{
	World::World()
		: m_context(Context())
		, m_renderer(*this)
	{
	}

	World::~World()
	{
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

	void World::init(Context context)
	{
		m_context = context;

		{ // player scene node
			SceneNode::u_ptr player = make_unique<SceneNode>();
		
			player->transform.position = { 2.5f, 1.0f, 2.5f };
			player->transform.scale = { 2.0f, 2.0f, 2.0f };
			player->name = "player";
		
			player->addComponent<MeshRenderer>(m_context.meshHolder->get("player"),
											   &m_context.materialHolder->get("dunjunText"));

			//player->addComponent<FaceCamera>(m_mainCamera);
		
			m_player = player.get();
		
			m_sceneGraph.attachChild(std::move(player));
		}

		{ // level generation
			auto level = make_unique<Level>();

			level->material = &m_context.materialHolder->get("terrain");
			level->name = "level";

			level->generate();

			m_level = level.get();

			m_sceneGraph.attachChild(std::move(level));
		}

		// add ambient light
		{
			m_ambientLight.setIntensities(ColorLib::Blue, 0.002f);
		}


		// add point lights
		{
			PointLight light;

			light.setIntensities(ColorLib::White, 20.0f);
			light.position = { 0.0f, -300.0f, 0.0f };
			m_pointLights.emplace_back(light);

			light.setIntensities(ColorLib::Red, 5.0f);
			light.position = { 3.0f, 0.5f, 3.0f };
			m_pointLights.emplace_back(light);

			light.setIntensities(ColorLib::Blue, 5.0f);
			light.position = { 2.5f, 0.5f, 2.0f };
			m_pointLights.emplace_back(light);

			light.setIntensities(ColorLib::Green, 5.0f);
			light.position = { 2.0f, 0.5f, 3.0f };
			m_pointLights.emplace_back(light);
		}

		// add directional lights
		{
			DirectionalLight light;

			light.setIntensities(ColorLib::Orange, 0.02f);
			light.direction = Vector3(-0.8, -1.0, -0.2);
			m_directionalLights.emplace_back(light);
		}

		// add spot lights
		{
			SpotLight light;

			light.position = { 2.5f, 2.0f, 2.5f };
			light.setIntensities(ColorLib::White, 50.0f);
			m_spotLights.emplace_back(light);
		}

		{
			//Initialize camera
			// aspect ratio
			m_playerCamera.viewportSize = m_context.window->getSize();
			m_playerCamera.viewportAspectRatio = m_context.window->getSize().x / m_context.window->getSize().y;
			//m_playerCamera.viewportSize = {854, 480};
			//m_playerCamera.viewportAspectRatio = 854 / 480;

			m_playerCamera.transform.position = m_player->transform.position + Vector3(8 * 3, 8 * 2, 8 * 3);
			//g_cameraPlayer.transform.orientation = angleAxis(Degree(45), {0, 1, 0}) * angleAxis(Degree(-30), {1, 0, 0});
			m_playerCamera.lookAt(m_player->transform.position);

			m_playerCamera.projectionType = ProjectionType::Orthographic;
			m_playerCamera.fieldOfView = Degree(50.0f); // for perspective view
			m_playerCamera.orthoScale = 15.0f; // for perspective view


											   // initialize world camera
			m_mainCamera = m_playerCamera;
			m_mainCamera.projectionType = ProjectionType::Perspective;
			m_mainCamera.lookAt({0, 0, 0});//m_player->transform.position);

			m_currentCamera = &m_mainCamera;

			m_player->transform.orientation = -m_currentCamera->transform.orientation;

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

		m_sceneGraph.init();
	} // end init()

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					UPDATE
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	void World::update(Time dt)
	{
		m_sceneGraph.update(dt);

		f32 camVel = 20.0f; // multiplier for camera speed
		f32 playerVelX = 5.5f;
		f32 playerVelY = 3.5f;
		f32 playerVelZ = 5.5f;

		//
		//if (checkForCommand == true)
		//{
		//	std::cout << "\n";
		//	if (consoleText == "HELP")
		//	{
		//		std::cout << "\n" << std::endl;
		//		std::cout << "Commands suck for now.\n--------------------" << std::endl;
		//
		//		std::cout << "GamePad::Left Stick = Move camera" << std::endl;
		//		std::cout << "GamePad::Right Stick = Turn camera" << std::endl;
		//		std::cout << "GamePad::D-Pad Move = sprite" << std::endl;
		//		std::cout << "GamePad::Shoulder Buttons = Move camera up/down" << std::endl;
		//		std::cout << "GamePad::X = Test vibration" << std::endl;
		//		//std::cout << "GamePad::B = Render Texture on sprite" << std::endl;
		//		std::cout << "GamePad::A = Move Light to current Camera location" << std::endl;
		//		std::cout << "\n";
		//		std::cout << "Keyboard::ArrowKeys = Move sprite" << std::endl;
		//		std::cout << "Keyboard::L/R Ctrl = Move sprite up/down" << std::endl;
		//		std::cout << "\n";
		//		std::cout << "[SYSTEM] = Shwo system information" << std::endl;
		//		std::cout << "[REGEN] = Regenerate level with culling" << std::endl;
		//		std::cout << "[REGENNC] = Regenerate level without culling" << std::endl;
		//		std::cout << "[DIR] = Return views cardinal direction and vertical angle" << std::endl;
		//		std::cout << "[ROOMS] = Return number of rooms currently rendering" << std::endl;
		//		std::cout << "[HELP] = Show help" << std::endl;
		//		std::cout << "[QUIT] = Close Program" << std::endl;
		//		//std::cout << "Keyboard::T = Test multiply transforms" << std::endl;
		//		//std::cout << "Keyboard::R = Reset sprite position, orientation and scale" << std::endl;
		//		//std::cout << "Keyboard::L = Change to orthographic camera" << std::endl;
		//		//std::cout << "Keyboard::K = Change to perspective camera" << std::endl;
		//
		//
		//	}
		//	else if (consoleText == "TODO")
		//	{
		//		std::cout << "stuff" << std::endl;
		//	}
		//	// room visibility test
		//	else if (consoleText == "ROOMS")
		//	{
		//		std::cout << "Rendering " << g_level->roomsRendered << " Rooms" << std::endl;
		//	}
		//	// room visibility test
		//	else if (consoleText == "CHAR")
		//	{
		//		for (int i = 0; i < 255; i++)
		//		{
		//			// skip the beep
		//			if (i == 7)
		//				continue;
		//
		//			std::cout << "char# " << i << " - " << (char)i << std::endl;
		//		}
		//
		//	}
		//	// room visibility test
		//	else if (consoleText == "SYSTEM")
		//	{
		//		std::cout << "Using Grapics Card:\n-------------------" << std::endl;
		//		std::cout << glGetString(GL_VENDOR) << std::endl;
		//		std::cout << glGetString(GL_RENDERER) << std::endl;
		//		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		//
		//		std::cout << "\n\n";
		//	}
		//	// return direction
		//	else if (consoleText == "DIR")
		//	{
		//		Vector3 f = g_cameraWorld.forward();
		//
		//		s32 angle = f.y * 90;
		//
		//		std::string direction;
		//
		//		if (f.x > 0) // right side
		//		{
		//			if (f.z > 0) // right-bottom quadrant
		//			{
		//				if (f.x > f.z)
		//					direction = "East";
		//				else
		//					direction = "South";
		//			}
		//			else // right-top quadrant
		//			{
		//				if (f.x > -f.z)
		//					direction = "East";
		//				else
		//					direction = "North";
		//			}
		//		}
		//		else // left side
		//		{
		//			if (f.z > 0) // left-bottom quadrant
		//			{
		//				if (-f.x > f.z)
		//					direction = "West";
		//				else
		//					direction = "South";
		//			}
		//			else // left-top quadrant
		//			{
		//				if (f.z > f.x)
		//					direction = "West";
		//				else
		//					direction = "North";
		//			}
		//		}
		//
		//		std::cout << "You are facing " << direction << " and looking " << angle << " degrees vertically." << std::endl;
		//
		//	}
		//	// cout test iterator
		//	else if (consoleText == "ITERATOR")
		//	{
		//		std::cout << "Test Iterator 0: " << testIterator_5[0] << std::endl;
		//		std::cout << "Test Iterator 1: " << testIterator_5[1] << std::endl;
		//		std::cout << "Test Iterator 2: " << testIterator_5[2] << std::endl;
		//		std::cout << "Test Iterator 3: " << testIterator_5[3] << std::endl;
		//		std::cout << "Test Iterator 4: " << testIterator_5[4] << std::endl;
		//	}
		//	// regenerate world without culling
		//	else if (consoleText == "REGENNC")
		//	{
		//		toggleCulling = false;
		//
		//		SceneNode* level = g_rootNode.findChildByName("level");
		//		g_world.getSceneGraph().detachChild(*level);
		//
		//		{ // test level generation
		//			auto level = make_unique<Level>();
		//
		//			level->material = &g_materialHolder.get("terrain");
		//			level->name = "level";
		//			level->generate();
		//
		//			g_level = level.get();
		//
		//			g_world.getSceneGraph().attachChild(std::move(level));
		//		}
		//	}
		//	// level regeneration
		//	else if (consoleText == "REGEN")
		//	{
		//		toggleCulling = true;
		//		SceneNode* level = g_world.getSceneGraph().findChildByName("level");
		//		g_world.getSceneGraph().detachChild(*level);
		//
		//		{ // test level generation
		//			auto level = make_unique<Level>();
		//
		//			level->material = &g_materialHolder.get("terrain");
		//			level->name = "level";
		//			level->generate();
		//
		//			g_level = level.get();
		//
		//			g_world.getSceneGraph().attachChild(std::move(level));
		//		}
		//	}
		//
		//	else if (consoleText == "QUIT")
		//	{
		//		m_context.window->close();
		//		g_running = false;
		//	}
		//	else
		//	{
		//		std::cout << "Invalid command." << std::endl;
		//	}
		//	consoleText.clear();
		//	checkForCommand = false;
		//}
		//

		Vector3 velocityDirection = { 0, 0, 0 };

		// game pad input
		if (Input::isGamepadPresent(0))
		{
			const f32 lookSensitivityX = 2.0f;
			const f32 lookSensitivityY = 1.5f;
			const f32 deadZone = 0.21f;

			//// gamepad camera rotation
			//Vector2 rts = axes.rightThumbStick;

			Vector2 rts = { -Input::getGamepadAxis(0, Input::GamepadAxis::RightX),
				-Input::getGamepadAxis(0, Input::GamepadAxis::RightY) };

			if (Math::abs(rts.x) < deadZone) // ignore anything in the deadZone
				rts.x = 0;
			if (Math::abs(rts.y) < deadZone)
				rts.y = 0;

			m_currentCamera->offsetOrientation(lookSensitivityX * Radian(rts.x * dt.asSeconds()),
				lookSensitivityY * Radian(rts.y * dt.asSeconds()));

			// gamepad camera translation
			//Vector2 lts = axes.leftThumbStick;

			Vector2 lts = { Input::getGamepadAxis(0, Input::GamepadAxis::LeftX),
				-Input::getGamepadAxis(0, Input::GamepadAxis::LeftY) };

			if (Math::abs(lts.x) < deadZone) // ignore anything in the deadZone
				lts.x = 0;
			if (Math::abs(lts.y) < deadZone)
				lts.y = 0;

			if (length(Vector2(lts.x, lts.y)) > 1.0f) // keep diagonals from being faster then straight x, y or z
				lts = normalize(lts);

			Vector3 camVelocityDirection = { 0, 0, 0 };

			Vector3 forward = m_currentCamera->forward();
			forward = normalize(forward);

			camVelocityDirection += lts.x * m_currentCamera->right();
			camVelocityDirection += lts.y * forward;

			//Input::GamepadButtons buttons = Input::getGamepadButtons(Input::Gamepad_1);

			if (Input::isGamepadButtonPressed(0, Input::GamepadButton::RightShoulder))
				camVelocityDirection.y += 1;
			if (Input::isGamepadButtonPressed(0, Input::GamepadButton::LeftShoulder))
				camVelocityDirection.y -= 1;

			if (length(camVelocityDirection) > 1.0f)
				camVelocityDirection = normalize(camVelocityDirection);

			m_currentCamera->transform.position += camVel * camVelocityDirection * dt.asSeconds();

			// gamepad player movement
			if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadUp))
			{
				Vector3 f = m_currentCamera->forward();
				f.y = 0;
				f = normalize(f);
				velocityDirection += f;
			}
			if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadDown))
			{
				Vector3 b = m_currentCamera->backward();
				b.y = 0;
				b = normalize(b);
				velocityDirection += b;
			}
			if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadLeft))
			{
				Vector3 l = m_currentCamera->left();
				l.y = 0;
				l = normalize(l);
				velocityDirection += l;
			}
			if (Input::isGamepadButtonPressed(0, Input::GamepadButton::DpadRight))
			{
				Vector3 r = m_currentCamera->right();
				r.y = 0;
				r = normalize(r);
				velocityDirection += r;
			}

			// vibration test
			if (Input::isGamepadButtonPressed(0, Input::GamepadButton::X))
			{
				std::cout << "X" << std::endl;
				Input::setGamepadVibration(0, 1.0f);
			}
			else
				Input::setGamepadVibration(0, 0.0f);
			
			//// temp variables used in lerp()
			////g_cameraWorld.projectionType = ProjectionType::Perspective;
			////const Matrix4 pp = g_cameraWorld.getProjection(); // perspective projection
			////
			////g_cameraWorld.projectionType = ProjectionType::Orthographic;
			////const Matrix4 op = g_cameraWorld.getProjection(); // perspective projection
			//
			
			//// 
			//if (Input::isGamepadButtonPressed(Input::Gamepad_1, Input::XboxButton::B))
			//{
			//	std::cout << "un-mapped button." << std::endl;
			//}
			
			if (Input::isGamepadButtonPressed(0, Input::GamepadButton::A))
			{
				m_spotLights[0].position.x = m_currentCamera->transform.position.x;
				m_spotLights[0].position.y = m_currentCamera->transform.position.y - 0.5f;
				m_spotLights[0].position.z = m_currentCamera->transform.position.z;
				m_spotLights[0].direction =  m_currentCamera->forward();
			}

		} // end Gamepad Input

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
		{
			Vector3 d = m_currentCamera->forward();
			d.y = 0;
			d = normalize(d);
			velocityDirection += d;
		}
		if (Input::isKeyPressed(Input::Key::Down))
		{
			Vector3 d = m_currentCamera->backward();
			d.y = 0;
			d = normalize(d);
			velocityDirection += d;
		}
		if (Input::isKeyPressed(Input::Key::Left))
		{
			Vector3 d = m_currentCamera->left();
			d.y = 0;
			d = normalize(d);
			velocityDirection += d;
		}
		if (Input::isKeyPressed(Input::Key::Right))
		{
			Vector3 d = m_currentCamera->right();
			d.y = 0;
			d = normalize(d);
			velocityDirection += d;
		}

		if (Input::isKeyPressed(Input::Key::RShift))
			velocityDirection += {0, 1, 0};
		if (Input::isKeyPressed(Input::Key::RControl))
			velocityDirection += {0, -1, 0};

		if (length(velocityDirection) > 0.0f)
		{
			velocityDirection = normalize(velocityDirection);

			// update player position
			m_player->transform.position += Vector3{ playerVelX, playerVelY, playerVelZ } * velocityDirection * dt.asSeconds();

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

			// camera follow movement
			m_playerCamera.transform.position.x = Math::lerp(m_playerCamera.transform.position.x, m_player->transform.position.x + 8.0f * 3.0f, 12.0f * dt.asSeconds());
			m_playerCamera.transform.position.y = Math::lerp(m_playerCamera.transform.position.y, m_player->transform.position.y + 8.0f * 2.0f, 12.0f * dt.asSeconds());
			m_playerCamera.transform.position.z = Math::lerp(m_playerCamera.transform.position.z, m_player->transform.position.z + 8.0f * 3.0f, 12.0f * dt.asSeconds());

			m_playerCamera.lookAt({ m_player->transform.position });
		}
		// make player face the camera
		m_player->transform.orientation = -m_currentCamera->transform.orientation;

	} // end update()

	void World::handleEvent(const Event& event)
	{
		m_sceneGraph.handleEvent(event);
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					RENDER
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


	void World::render()
	{

		Vector2 fbSize = m_context.window->getSize();

		// check whether to update aspectratio and size each cycle
		if (m_context.window->currentSize != fbSize)
		{
			m_renderer.setFrameBufferSize(fbSize);
		}

		//m_renderer.reset();
		//m_renderer.clearAll();

		//m_renderer.addSceneGraph(m_sceneGraph);

		//m_renderer.setCamera(*m_currentCamera);

		//m_renderer.getGBuffer().create(viewSize.x, viewSize.y);

		//m_renderer.deferredGeometryPass();
		//m_renderer.deferredLightPass();
		//m_renderer.deferredFinalPass();

		//m_renderer.setFrameBufferSize(fbSize);
		m_renderer.render();

		glViewport(0, 0, fbSize.x, fbSize.y);
		glClearColor(0.02f, 0.02f, 0.02f, 1.0f); // set the default color (R,G,B,A)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			ShaderProgram& shaders = m_context.shaderHolder->get("texturePass");

			shaders.use();
			shaders.setUniform("u_tex", 0);
			shaders.setUniform("u_scale", Vector3(1.0f));

			Texture::bind(&m_renderer.getFinalTexture(), 0);

			m_renderer.draw(&m_context.meshHolder->get("quad"));

			shaders.stopUsing();
		}


		m_context.window->display(); // switches information between the front buffer and the back buffer
	} // end render()

	SceneNode& World::getSceneGraph()
	{
		return m_sceneGraph;
	}

	const SceneNode& World::getSceneGraph() const
	{
		return m_sceneGraph;
	}

	SceneNode* World::getPlayer()
	{
		return m_player;
	}

	const SceneNode* World::getPlayer() const
	{
		return m_player;
	}

	Level* World::getLevel()
	{
		return m_level;
	}

	const Level* World::getLevel() const
	{
		return m_level;
	}

	Camera& World::getCurrentCamera()
	{
		return *m_currentCamera;
	}

	const Camera& World::getCurrentCamera() const
	{
		return *m_currentCamera;
	}

} // end Dunjun

