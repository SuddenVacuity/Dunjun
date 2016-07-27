
#include <Dunjun/Entity.hpp>

namespace Dunjun
{
	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					ENTITY SYSTEM
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	EntityId EntitySystem::addEntity(u32 componentMask)
	{
		EntityId id;
		for (id = 0; id < MaxEntities; id++)
		{
			if (components[id] == ComponentMask::ComponentNone)
			{
				components[id] = componentMask;
				return id;
			}

		}

		std::cerr << stderr << "EntityWorld::createEntity() entities is full\n";
		return MaxEntities;
	}

	bool EntitySystem::isAlive(EntityId id) const
	{
		return components[id] != ComponentMask::ComponentNone;
	}

	void EntitySystem::removeEntity(EntityId id)
	{
		components[id] = ComponentMask::ComponentNone;
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					WORLD
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	World::World()
		: entitySystem()
		, sceneGraph(defaultAllocator())
		, renderSystem(defaultAllocator(), sceneGraph)
		, camera()
		, names()
	{
		//for(u32 id = 0; id < MaxEntities; id++)
		//	components[id] = ComponentMask::ComponentNone;

		camera.transform.position = {0, 3, 9};
		camera.cameraLookAt({0, 0, 0});
		camera.fieldOfView = Degree(90);
		camera.projectionType = ProjectionType::Perspective;
	}

	void World::handleEvent(const Event& event)
	{

	}

	void World::update(Time dt)
	{

	}

	void World::render()
	{
		renderSystem.resetAllPointers();
		renderSystem.camera = &camera;
		camera.viewportAspectRatio = renderSystem.fbSize.x / renderSystem.fbSize.y;
		renderSystem.render();

		//glViewport(0, 0, renderSystem.fbSize.x, renderSystem.fbSize.y);
		//glClearColor(0, 0, 0, 0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			ShaderProgram& shaders = g_shaderHolder.get("texturePass");

			shaders.use();
			defer(shaders.stopUsing());

			shaders.setUniform("u_scale", Vector3{ 1, 1, 1 });
			shaders.setUniform("u_tex", 0);

			bindTexture(&renderSystem.finalTexture.colorTexture, 0);

			drawMesh(g_meshHolder.get("quad"));
		}
	}

} // end Dunjun
