#ifndef DUNJUN_ENTITY_HPP
#define DUNJUN_ENTITY_HPP

#include <Dunjun/Math.hpp>
#include <Dunjun/SceneGraph.hpp>

namespace Dunjun
{
	struct SceneGraph;

	enum ComponentMask : u32
	{
		ComponentNone			= 0x00000000,
	//	ComponentPosition		= 0x00000001,
		ComponentName			= 0x00000001,

	};

	//struct PositionComponent
	//{
	//	Vector3 position;
	//};

	struct NameComponent
	{
		std::string name;
	};


#define MaxEntities 1024


	struct EntityWorld
	{
	public:
		//GLOBAL const size_t MaxEntities;
		u32 components[MaxEntities];
		NameComponent names[MaxEntities];
		//PositionComponent positions[MaxEntities];

		SceneGraph sceneGraph;

		EntityWorld();
		~EntityWorld() = default;
		//void init();

		EntityId createEntity();

		bool alive(EntityId id);
		void destroy(EntityId id);
	};


} // end Dunjun

#endif
