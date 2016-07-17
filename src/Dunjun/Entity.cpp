
#include <Dunjun/Entity.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/Memory/Memory.hpp>

namespace Dunjun
{
	EntityWorld::EntityWorld()
		: sceneGraph(defaultAllocator())
	{
		for(EntityId& id : components)
			id = ComponentMask::ComponentNone;
	}

	EntityId EntityWorld::createEntity()
	{
		EntityId id;
		for(id = 0; id < MaxEntities; id++)
		{
			if(components[id] == ComponentMask::ComponentNone)
				return id;

		}

		std::cerr << stderr << "EntityWorld::createEntity() entities is full\n";
		return MaxEntities;

	}

	bool EntityWorld::alive(EntityId id)
	{
		return components[id] != ComponentMask::ComponentNone;
	}

	void EntityWorld::destroy(EntityId id)
	{
		components[id] = ComponentMask::ComponentNone;
	}

} // end Dunjun
