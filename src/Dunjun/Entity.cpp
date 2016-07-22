
#include <Dunjun/Entity.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/Memory/Memory.hpp>

#include <Dunjun/ResourceHolder_objects.hpp>

namespace Dunjun
{
	EntityWorld::EntityWorld()
		: sceneGraph(defaultAllocator())
		, renderSystem(defaultAllocator(), sceneGraph)
		, components()
		, camera()
	{
		for(u32 id = 0; id < MaxEntities; id++)
			components[id] = ComponentMask::ComponentNone;

		camera.transform.position = {0, 3, 9};
		camera.lookAt({0, 0, 0});

		camera.projectionType = ProjectionType::Perspective;
	}

	EntityId EntityWorld::createEntity(u32 flags)
	{
		EntityId id;
		for(id = 0; id < MaxEntities; id++)
		{
			if(components[id] == ComponentMask::ComponentNone)
			{
				components[id] = flags;
				return id;
			}

		}

		std::cerr << stderr << "EntityWorld::createEntity() entities is full\n";
		return MaxEntities;

	}

	bool EntityWorld::isAlive(EntityId id) const
	{
		return components[id] != ComponentMask::ComponentNone;
	}

	void EntityWorld::destroy(EntityId id)
	{
		components[id] = ComponentMask::ComponentNone;
	}

	void EntityWorld::handleEvent(const Event& event)
	{

	}

	void EntityWorld::update(Time dt)
	{

	}

	void EntityWorld::render()
	{
		renderSystem.resetAllPointers();
		renderSystem.fbSize = { 854, 480 };
		renderSystem.camera = &camera;
		camera.viewportAspectRatio = renderSystem.fbSize.x / renderSystem.fbSize.y;
		renderSystem.render();

		glViewport(0, 0, renderSystem.fbSize.x, renderSystem.fbSize.y);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			ShaderProgram& shaders = g_shaderHolder.get("texturePass");
			shaders.use();

			shaders.setUniform("u_scale", Vector3{ 1, 1, 1 });
			shaders.setUniform("u_tex", 0);

			Texture::bind(&renderSystem.finalTexture.colorTexture, 0);

			drawMesh(g_meshHolder.get("quad"));

			shaders.stopUsing();
		}
	}

} // end Dunjun
