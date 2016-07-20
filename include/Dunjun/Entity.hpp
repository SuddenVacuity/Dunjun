#ifndef DUNJUN_ENTITY_HPP
#define DUNJUN_ENTITY_HPP

#include <Dunjun/Math.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/Window/Event.hpp>
#include <Dunjun/RenderSystem.hpp>

namespace Dunjun
{
	struct SceneGraph;

	enum ComponentMask : u32
	{
		ComponentNone			= 0x00000000,
	//	ComponentPosition		= 0x00000001,
		ComponentName			= 0x00000001,
		ComponentRender			= 0x00000002,

	};

	//struct PositionComponent
	//{
	//	Vector3 position;
	//};

	struct NameComponent
	{
		std::string name;
	};


//#define MaxEntities 1024


	struct EntityWorld
	{
	public:
		GLOBAL const size_t MaxEntities = 1024;

		u32 components[MaxEntities];
		NameComponent names[MaxEntities];
		//PositionComponent positions[MaxEntities];

		SceneGraph sceneGraph;
		Camera camera;
		RenderSystem renderSystem;

		EntityId player;

		EntityWorld();
		~EntityWorld() = default;
		//void init();

		EntityId createEntity();

		bool isAlive(EntityId id) const;
		void destroy(EntityId id);

		void handleEvent(const Event& event);
		void update(Time dt);
		void render();
	};


} // end Dunjun

#endif
