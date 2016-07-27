#ifndef DUNJUN_ENTITY_HPP
#define DUNJUN_ENTITY_HPP

#include <Dunjun/Context.hpp>


namespace Dunjun
{
	struct SceneGraph;

	enum ComponentMask : u32
	{
		ComponentNone			= 0x00000000,
	//	ComponentPosition		= 0x00000001,
		ComponentName			= 0x00000001,
		ComponentTransform		= 0x00000002,
		ComponentRender			= 0x00000004 | ComponentTransform,

	};

	//struct PositionComponent
	//{
	//	Vector3 position;
	//};

	struct NameComponent
	{
		String name;
	};

	struct EntitySystem
	{
	public:
		GLOBAL const size_t MaxEntities = 1024;
		u32 components[MaxEntities];

		EntitySystem() = default;

		EntityId addEntity(u32 componentMask);
		void removeEntity(u32 id);

		bool isAlive(EntityId id) const;
	}; // end EntitySystem

	struct World
	{
	public:
		EntitySystem entitySystem;

		SceneGraph sceneGraph;
		RenderSystem renderSystem;

		Camera camera;

		NameComponent names[EntitySystem::MaxEntities];
		//PositionComponent positions[MaxEntities];

		// temporary handles to track entities
		EntityId player;
		EntityId crate;

		World();
		~World() = default;
		//void init();
		//World(const EntityWorld&) = delete;
		//World& operator=(const EntityWorld&) = delete;

		//EntityId addEntity(u32 flags);
		//void removeEntity(EntityId id);

		//bool isAlive(EntityId id) const;

		void handleEvent(const Event& event);
		void update(Time dt);
		void render();

	private:
		World(const World&) = delete;
		World& operator=(const World&) = delete;
	};


} // end Dunjun

#endif
