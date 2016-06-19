
#include <Dunjun/World.hpp>

namespace Dunjun
{
	World::World(Context context)
		: m_context(context)
	{
	}

	World::~World()
	{
	}

	void World::init()
	{
		m_sceneGraph.init();

		//{ // player scene node
		//	SceneNode::u_ptr player = make_unique<SceneNode>();
		//
		//	player->transform.position = { 2.5f, 1.0f, 2.5f };
		//	player->transform.scale = { 2.0f, 2.0f, 2.0f };
		//	g_sprite.material = &m_context.materialHolder.get("dunjunText"); // apply material
		//	player->name = "player";
		//
		//	player->addComponent<MeshRenderer>(g_sprite);
		//	//player->addComponent<FaceCamera>(g_cameraPlayer);
		//
		//	m_player = player.get();
		//
		//	m_sceneGraph.attachChild(std::move(player));
		//}

		{ // level generation
			auto level = make_unique<Level>();

			level->material = &m_context.materialHolder.get("terrain");
			level->name = "level";

			level->generate();

			m_level = level.get();

			m_sceneGraph.attachChild(std::move(level));
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
			//m_playerCamera.viewportSize = g_window.getSize();
			//m_playerCamera.viewportAspectRatio = g_window.getSize().x / g_window.getSize().y;
			m_playerCamera.viewportSize = {854, 480};
			m_playerCamera.viewportAspectRatio = 854 / 480;

			m_playerCamera.transform.position = m_player->transform.position + Vector3(8 * 3, 8 * 2, 8 * 3);
			//g_cameraPlayer.transform.orientation = angleAxis(Degree(45), {0, 1, 0}) * angleAxis(Degree(-30), {1, 0, 0});
			m_playerCamera.lookAt(m_player->transform.position);

			m_playerCamera.projectionType = ProjectionType::Orthographic;
			m_playerCamera.fieldOfView = Degree(50.0f); // for perspective view
			m_playerCamera.orthoScale = 15.0f; // for perspective view


											   // initialize world camera
			m_mainCamera = m_playerCamera;
			m_mainCamera.projectionType = ProjectionType::Perspective;
			m_mainCamera.lookAt(m_player->transform.position);

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
	}

	void World::update(Time dt)
	{
		m_sceneGraph.update(dt);
	}

	void World::handleEvent(const Event& event)
	{
		m_sceneGraph.handleEvent(event);
	}

	void World::draw()
	{

		Vector2 viewSize = m_context.window.getSize();

		// check whether to update aspectratio and size each cycle
		if (m_context.window.currentSize != viewSize)
		{
			m_context.window.currentSize = viewSize;
			m_context.window.currentAspectRatio = viewSize.x / viewSize.y;

			//g_cameraPlayer.viewportAspectRatio = aspectRatio;
			//g_cameraWorld.viewportAspectRatio = aspectRatio;
		}

		m_renderer.reset();
		m_renderer.clearAll();

		m_renderer.addSceneGraph(m_sceneGraph);

		for (const auto& light : m_directionalLights)
			m_renderer.addDirectionalLight(&light);
		for (const auto& light : m_pointLights)
			m_renderer.addPointLight(&light);
		for (const auto& light : m_spotLights)
			m_renderer.addSpotLight(&light);

		m_renderer.camera = m_currentCamera;

		//g_renderer.quad = g_meshes["quad"];

		m_renderer.gBuffer.create(viewSize.x, viewSize.y);

		m_renderer.deferredGeometryPass();
		m_renderer.deferredLightPass();
		m_renderer.deferredFinalPass();

		//g_materialHolder.get("dunjunText").diffuseMap = &g_renderer.finalTexture.colorTexture;

		glViewport(0, 0, viewSize.x, viewSize.y);
		glClearColor(0.02f, 0.02f, 0.02f, 1.0f); // set the default color (R,G,B,A)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			ShaderProgram& shaders = m_context.shaderHolder.get("texturePass");

			shaders.use();
			shaders.setUniform("u_tex", 0);
			shaders.setUniform("u_scale", Vector3(1.0f));

			Texture::bind(&m_renderer.finalTexture.colorTexture, 0);

			m_renderer.draw(&m_context.meshHolder.get("quad"));

			shaders.stopUsing();
		}


		m_context.window.display(); // switches information between the front buffer and the back buffer
	}

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

