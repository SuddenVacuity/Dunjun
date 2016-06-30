#ifndef DUNJUN_WORLD_HPP
#define DUNJUN_WORLD_HPP

#include <Dunjun/Level.hpp>

namespace Dunjun
{
	class World : private NonCopyable
	{
	public:
		Context context;

		SceneNode sceneGraph;
		SceneRenderer renderer;

		Camera mainCamera;
		Camera playerCamera;
		Camera* currentCamera = nullptr;

		SceneNode * player = nullptr;
		Level* level = nullptr;

		BaseLight ambientLight = BaseLight();
		std::vector<DirectionalLight> directionalLights;
		std::vector<PointLight> pointLights;
		std::vector<SpotLight> spotLights;

		bool toggleCulling = true;

		std::string consoleText = "";
		std::string consoleBuffer = "";
		bool acceptInput = true;
		bool checkForCommand = false;
		bool useConsole = false;

		World();
		virtual ~World();

		void init(Context context);

		void update(Time dt);
		void handleEvent(const Event& event);
		void render();

		//SceneNode& getSceneGraph();
		//const SceneNode& getSceneGraph() const;

		//SceneNode* getPlayer();
		//const SceneNode* getPlayer() const;
		//
		//Level* getLevel();
		//const Level* getLevel() const;
		//
		//Camera& getCurrentCamera();
		//const Camera& getCurrentCamera() const;

	};

} // end Dunjun

#endif
