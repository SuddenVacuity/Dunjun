#ifndef DUNJUN_WORLD_HPP
#define DUNJUN_WORLD_HPP

#include <Dunjun/Level.hpp>

namespace Dunjun
{
	class World : private NonCopyable
	{
	public:
		World();
		virtual ~World();

		void init(Context context);

		void update(Time dt);
		void handleEvent(const Event& event);
		void render();

		SceneNode& getSceneGraph();
		const SceneNode& getSceneGraph() const;

		SceneNode* getPlayer();
		const SceneNode* getPlayer() const;

		Level* getLevel();
		const Level* getLevel() const;

		Camera& getCurrentCamera();
		const Camera& getCurrentCamera() const;

	private:
		Context m_context;

		SceneNode m_sceneGraph;
		SceneRenderer m_renderer;

		Camera m_mainCamera;
		Camera m_playerCamera;
		Camera* m_currentCamera = nullptr;

		SceneNode * m_player = nullptr;
		Level* m_level = nullptr;

		std::vector<DirectionalLight> m_directionalLights;
		std::vector<PointLight> m_pointLights;
		std::vector<SpotLight> m_spotLights;



	};

} // end Dunjun

#endif
