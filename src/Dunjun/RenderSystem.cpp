
#include <Dunjun/RenderSystem.hpp>

namespace Dunjun
{
	using ComponentId = u32;

	RenderSystem::RenderSystem(Allocator& a, SceneGraph& sg)
		: allocator(a)
		, data()
		, map(a)
		, sceneGraph(sg)
		, gBuffer()
		, lightingBuffer()
		, ambientLightColor({0, 0, 255, 0})
		, ambientLightIntensity(0.002f)
		, directionalLights(a)
		, pointLights(a)
		, spotLights(a)
		, camera(nullptr)
		, currentTexture()
	{
		allocate(16);
	}

	RenderSystem::~RenderSystem()
	{
		allocator.deallocate(data.buffer);

		destroyGBuffer(gBuffer);
		destroyRenderTexture(lightingBuffer);
		destroyRenderTexture(finalTexture);
	}

	void RenderSystem::allocate(u32 capacity)
	{
		if(capacity <= data.length)
			return;

		// add up all the data in void* buffer
		const size_t bytes = capacity * (sizeof(EntityId) +
										 sizeof(RenderComponent));

		// make a copy of data that's the new capacity
		InstanceData newData;
		newData.length = data.length;
		newData.capacity = capacity;
		newData.buffer = allocator.allocate(bytes);

		// assign pointers to buffer data accunting for extra capacity
		newData.entityId	= (EntityId*)(newData.buffer);
		newData.component	= (RenderComponent*)(newData.entityId	+ capacity);

		// copy data from old location to new location
		memcpy(newData.entityId,	data.entityId,	  data.length * sizeof(EntityId));
		memcpy(newData.component,	data.component,	  data.length * sizeof(RenderComponent));

		allocator.deallocate(data.buffer);

		// change pointer to point to new data
		data = newData;
	}

	ComponentId RenderSystem::addComponent(EntityId id, const RenderComponent& component)
	{
		if (data.capacity == data.length || data.capacity == 0)
			allocate(2 * data.length + 1);

		// get last position in data arrays
		const ComponentId last = data.length;

		// assing data to next open position
		data.entityId[last] = id;
		data.component[last] = component;

		// add data to hash map
		set(map, id, last);

		data.length++;

		return last;
	}

	void RenderSystem::removeComponent(ComponentId id)
	{
		const ComponentId last = data.length - 1;
		const EntityId entity = data.entityId[id];
		const EntityId lastEntity = data.entityId[last];

		data.entityId[id] = data.entityId[last];
		data.component[id] = data.component[last];


		set(map, lastEntity, id);
		set(map, entity, EmptyId);

		data.length--;
	}


	ComponentId RenderSystem::getComponentId(EntityId id)
	{
		return get(map, id, EmptyId);
	}

	bool RenderSystem::isValid(ComponentId id) const
	{
		return id != EmptyId;
	}

	void RenderSystem::resetAllPointers()
	{
		camera = nullptr;
		//currentTexture = nullptr;
		for(u32 i = 0; i < 32; i++) // 32 is the number of elements in currentTexture array
			setTexture(nullptr, i);
	}

	void RenderSystem::render()
	{
		//clearAll();
		//addSceneGraph(world->sceneGraph);

		//camera = world->currentCamera;

		if (!camera)
		{
			std::cerr << "RenderSystem:: No camera for rendersystem.\n";
			return;
		}

		createGBuffer(gBuffer, fbSize.x, fbSize.y);

		deferredGeometryPass();
		deferredLightPass();
		deferredFinalPass();
	}


	void RenderSystem::deferredGeometryPass()
	{
		//std::sort(begin(modelInstances), end(modelInstances),
		//	[](const ModelInstance& a, const ModelInstance& b) -> bool
		//{
		//	const auto* A = a.asset->material;
		//	const auto* B = b.asset->material;
		//
		//	// if same shaders sort by texture else sort by shader
		//	if (A != B && A && B)
		//	{
		//		if (A->shaders == B->shaders)
		//			return A->diffuseMap < B->diffuseMap;
		//		else
		//			return A->shaders < B->shaders;
		//	}
		//	return false;
		//});

		ShaderProgram& shaders = g_shaderHolder.get("deferredGeometryPass");

		bindGBuffer(&gBuffer);
		defer(bindGBuffer(nullptr));
		{
			glViewport(0, 0, gBuffer.width, gBuffer.height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaders.use();
			defer(shaders.stopUsing());

			shaders.setUniform("u_camera", camera->getMatrix()); // shaderprogram.cpp
			shaders.setUniform("u_cameraPosition", camera->transform.position); // shaderprogram.cpp

			for(u32 i = 0; i < data.length; i++)
			{
				EntityId entityId = data.entityId[i];
				SceneGraph::NodeId nodeId = sceneGraph.getNodeId(entityId);


				const RenderComponent& component = data.component[i];
				const Material& material = component.material;
				const Transform& transform = sceneGraph.getGlobalTransform(nodeId);

				shaders.setUniform("u_material.diffuseMap", (u32)0); // shaderprogram.cpp
				shaders.setUniform("u_material.normalMap", (u32)1); // shaderprogram.cpp

				shaders.setUniform("u_material.diffuseColor", material.diffuseColor); // shaderprogram.cpp
				shaders.setUniform("u_material.specularColor", material.specularColor); // shaderprogram.cpp
				shaders.setUniform("u_material.specularExponent", material.specularExponent); // shaderprogram.cpp

				setTexture(material.diffuseMap, 0);
				setTexture(material.normalMap, 1);
				shaders.setUniform("u_transform", transform); // shaderprogram.cpp

				drawMesh(component.mesh);
			}
			//for (const auto& inst : modelInstances)
			//{
			//	if (!inst.asset->mesh)
			//		continue;
			//
			//	const Material& material = *inst.asset->material;
			//
			//	shaders.setUniform("u_material.diffuseMap", (u32)0); // shaderprogram.cpp
			//	shaders.setUniform("u_material.diffuseColor", material.diffuseColor); // shaderprogram.cpp
			//	shaders.setUniform("u_material.specularColor", material.specularColor); // shaderprogram.cpp
			//	shaders.setUniform("u_material.specularExponent", material.specularExponent); // shaderprogram.cpp
			//
			//	setTexture(inst.asset->material->diffuseMap, 0);
			//
			//	shaders.setUniform("u_transform", inst.transform); // shaderprogram.cpp
			//
			//	draw(inst.asset->mesh);
			//}

			glFlush();
		}
	}
	
	void RenderSystem::deferredLightPass()
	{
		createRenderTexture(lightingBuffer, gBuffer.width, gBuffer.height, RenderTexture::Light);

		bindTexture(&gBuffer.textures[GBuffer::Diffuse], 0);
		bindTexture(&gBuffer.textures[GBuffer::Specular], 1);
		bindTexture(&gBuffer.textures[GBuffer::Normal], 2);
		bindTexture(&gBuffer.textures[GBuffer::Depth], 3);

		bindRenderTexture(&lightingBuffer);
		defer(bindRenderTexture(nullptr));
		{
			glClearColor(0, 0, 0, 0);
			glViewport(0, 0, lightingBuffer.width, lightingBuffer.height);
			glClear(GL_COLOR_BUFFER_BIT);

			glDepthMask(false);
			defer(glDepthMask(true));

			glEnable(GL_BLEND);
			defer(glDisable(GL_BLEND));

			glBlendFunc(GL_ONE, GL_ONE);

			renderAmbientLight();
			renderDirectionalLights();
			renderPointLights();
			renderSpotLights();

		}
	}
	
	void RenderSystem::renderAmbientLight()
	{
		auto& shaders = g_shaderHolder.get("deferredAmbientLight");

		shaders.use();
		defer(shaders.stopUsing());

		Vector3 colorIntensity = calculateLightIntensities(ambientLightColor, ambientLightIntensity);

		shaders.setUniform("u_light.intensities", colorIntensity);

		drawMesh(g_meshHolder.get("quad"));
	}

	void RenderSystem::renderDirectionalLights()
	{
		auto& shaders = g_shaderHolder.get("deferredDirectionalLight");

		shaders.use();
		defer(shaders.stopUsing());

		shaders.setUniform("u_specular", 1);
		shaders.setUniform("u_normal", 2);

		for (const DirectionalLight& light : directionalLights)
		{
			//Vector3 colorIntensity = calculateLightIntensities(light.color, light.intensity);

			shaders.setUniform("u_light.base.intensities", light.colorIntensity); // shaderprogram.cpp

			shaders.setUniform("u_light.direction", normalize(light.direction));

			drawMesh(g_meshHolder.get("quad"));
		}
	}

	void RenderSystem::renderPointLights()
	{
		auto& shaders = g_shaderHolder.get("deferredPointLight");

		shaders.use();
		defer(shaders.stopUsing());

		shaders.setUniform("u_diffuse", 0);
		shaders.setUniform("u_specular", 1);
		shaders.setUniform("u_normal", 2);
		shaders.setUniform("u_depth", 3);

		shaders.setUniform("u_cameraInverse", inverse(camera->getMatrix()));

		for (const PointLight& light : pointLights)
		{
			//light.range = calculateLightRange(light.intensity, light.color, light.attenuation);

			Vector3 colorIntensity = calculateLightIntensities(light.color, light.intensity);
			f32 range = calculateLightRange(light.intensity, light.color, light.attenuation);

			shaders.setUniform("u_light.base.intensities", colorIntensity); // shaderprogram.cpp

			shaders.setUniform("u_light.position", light.position); // shaderprogram.cpp
			shaders.setUniform("u_light.range", range); // shaderprogram.cpp

			shaders.setUniform("u_light.attenuation.constant", light.attenuation.constant); // shaderprogram.cpp
			shaders.setUniform("u_light.attenuation.linear", light.attenuation.linear); // shaderprogram.cpp
			shaders.setUniform("u_light.attenuation.quadratic", light.attenuation.quadratic); // shaderprogram.cpp

			drawMesh(g_meshHolder.get("quad"));

		}
	}

	void RenderSystem::renderSpotLights()
	{
		auto& shaders = g_shaderHolder.get("deferredSpotLight");

		shaders.use();
		defer(shaders.stopUsing());

		shaders.setUniform("u_diffuse", 0);
		shaders.setUniform("u_specular", 1);
		shaders.setUniform("u_normal", 2);
		shaders.setUniform("u_depth", 3);

		shaders.setUniform("u_cameraInverse", inverse(camera->getMatrix()));

		for (const SpotLight& light : spotLights)
		{
			Vector3 colorIntensity = calculateLightIntensities(light.color, light.intensity);
			f32 range = calculateLightRange(light.intensity, light.color, light.attenuation);


			shaders.setUniform("u_light.coneAngle", static_cast<f32>(light.coneAngle)); // shaderprogram.cpp
			shaders.setUniform("u_light.direction", light.direction); // shaderprogram.cpp

			shaders.setUniform("u_light.pointLight.base.intensities", colorIntensity); // shaderprogram.cpp

			shaders.setUniform("u_light.pointLight.position", light.position); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.range", range); // shaderprogram.cpp

			shaders.setUniform("u_light.pointLight.attenuation.constant", light.attenuation.constant); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.attenuation.linear", light.attenuation.linear); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.attenuation.quadratic", light.attenuation.quadratic); // shaderprogram.cpp

			drawMesh(g_meshHolder.get("quad"));

		}
	}
	// */
	void RenderSystem::deferredFinalPass()
	{

		createRenderTexture(finalTexture, gBuffer.width, gBuffer.height, RenderTexture::Color);
		
		bindTexture(&gBuffer.textures[GBuffer::Diffuse], 0);
		bindTexture(&lightingBuffer.colorTexture, 1);
		
		bindRenderTexture(&finalTexture);
		defer(bindRenderTexture(nullptr));
		{
			glClearColor(0, 0, 0, 0);
			glViewport(0, 0, finalTexture.width, finalTexture.height);
			glClear(GL_COLOR_BUFFER_BIT);
		
			auto& shaders = g_shaderHolder.get("deferredFinalPass");
		
			shaders.use();
			defer(shaders.stopUsing());
		
			shaders.setUniform("u_diffuse", 0);
			shaders.setUniform("u_lighting", 1);
		
			drawMesh(g_meshHolder.get("quad"));
		
		}
	}


	//bool RenderSystem::isCurrentShaders(const ShaderProgram* shaders)
	//{
	//
	//}

	//bool RenderSystem::isCurrentTexture(const Texture* texture)
	//{
	//	if (texture == currentTexture[position])
	//	{
	//		return true;
	//	}
	//	return false;
	//}

	//void RenderSystem::setShaders(const ShaderProgram* shaders)
	//{
	//
	//}

	bool RenderSystem::setTexture(const Texture* texture, u32 position)
	{
		if (texture != currentTexture[position])
		{
			currentTexture[position] = texture;
			bindTexture(currentTexture[position], position);
			return true;
		}
		return false;
	}







} // end Dunjun
