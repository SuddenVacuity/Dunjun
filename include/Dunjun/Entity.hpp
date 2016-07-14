#ifndef DUNJUN_ENTITY_HPP
#define DUNJUN_ENTITY_HPP

#include <Dunjun/Math.hpp>

namespace Dunjun
{
	using EntityId = u32;

	enum ComponentMask
	{
		ComponentNone			= 0x00000000,
		ComponentPosition		= 0x00000001,
		ComponentName			= 0x00000010,

	};

	struct PositionComponent
	{
		Vector3 position;
	};

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

		PositionComponent positions[MaxEntities];
		NameComponent names[MaxEntities];

		void init();

		EntityId createEntity();

		bool alive(EntityId id);
		void destroy(EntityId id);
	};


} // end Dunjun

#endif
