
#include <Dunjun/Scene/SceneRenderer.hpp>
#include <Dunjun/Scene/MeshRenderer.hpp>
#include <Dunjun/World.hpp>

namespace Dunjun
{
	SceneRenderer::SceneRenderer()
		: modelInstances(defaultAllocator())
	{
	}

	SceneRenderer::~SceneRenderer()
	{
		gBuffer.destroy();
	}

	SceneRenderer& SceneRenderer::reset()
	{
		if (currentShaders)
			currentShaders->stopUsing();

		currentShaders = nullptr;
		currentTexture = nullptr;
		Texture::bind(nullptr, 0);
		currentMaterial = nullptr; // commenting this out makes the top right corner of the window white

		return *this;
	}

	SceneRenderer& SceneRenderer::clearAll()
	{
		clear(modelInstances);
		//m_directionalLights.clear();
		//m_pointLights.clear();
		//m_spotLights.clear();

		return *this;
	}

	SceneRenderer& SceneRenderer::addSceneGraph(const SceneNode& node, const Transform& t)
	{
		node.draw(*this, t);

		return *this;
	}

	void SceneRenderer::draw(const Mesh* mesh) const
	{
		if(mesh)
			mesh->draw();
	}
	
	void SceneRenderer::addModelInstance(const MeshRenderer& meshRenderer, Transform t)
	{
		if(meshRenderer.parent->enabled == true)
		{
			ModelInstance m;
			m.asset = &meshRenderer;
			m.transform = t;

			append(modelInstances, m);
		}
	}

	//void SceneRenderer::addDirectionalLight(const DirectionalLight* light)
	//{
	//	m_directionalLights.emplace_back(light);
	//}
	//
	//void SceneRenderer::addPointLight(const PointLight* light)
	//{
	//	m_pointLights.emplace_back(light);
	//}
	//
	//void SceneRenderer::addSpotLight(const SpotLight* light)
	//{
	//	m_spotLights.emplace_back(light);
	//}

	SceneRenderer& SceneRenderer::setFrameBufferSize(Vector2 size)
	{
		world->context.window->setSize(size);

		return *this;
	}

	void SceneRenderer::render()
	{
		reset();
		clearAll();
		addSceneGraph(world->sceneGraph);

		camera = world->currentCamera;

		gBuffer.create(world->context.window->currentSize.x,
						 world->context.window->currentSize.y);

		deferredGeometryPass();
		deferredLightPass();
		deferredFinalPass();
	}

	SceneRenderer& SceneRenderer::deferredGeometryPass()
	{
		std::sort(begin(modelInstances), end(modelInstances),
			[](const ModelInstance& a, const ModelInstance& b) -> bool
		{
			const auto* A = a.asset->material;
			const auto* B = b.asset->material;

			// if same shaders sort by texture else sort by shader
			if( A != B && A && B)
			{
				if (A->shaders == B->shaders)
					return A->diffuseMap < B->diffuseMap;
				else
				return A->shaders < B->shaders;
			}
			return false;
		});

		auto& shaders = g_shaderHolder.get("deferredGeometryPass");

		GBuffer::bind(&gBuffer);
		{
			glViewport(0, 0, gBuffer.width, gBuffer.height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaders.use();

			shaders.setUniform("u_camera", camera->getMatrix()); // shaderprogram.cpp
			shaders.setUniform("u_cameraPosition", camera->transform.position); // shaderprogram.cpp

			for (const auto& inst : modelInstances)
			{
				if (!inst.asset->mesh)
					continue;

				const Material& material = *inst.asset->material;

				shaders.setUniform("u_material.diffuseMap", (u32)0); // shaderprogram.cpp
				shaders.setUniform("u_material.diffuseColor", material.diffuseColor); // shaderprogram.cpp
				shaders.setUniform("u_material.specularColor", material.specularColor); // shaderprogram.cpp
				shaders.setUniform("u_material.specularExponent", material.specularExponent); // shaderprogram.cpp

				setTexture(inst.asset->material->diffuseMap, 0);

				shaders.setUniform("u_transform", inst.transform); // shaderprogram.cpp

				draw(inst.asset->mesh);
			}
			glFlush();
		}
		GBuffer::bind(nullptr);

		return *this;
	} // end deferredGeometryPass()

	SceneRenderer& SceneRenderer::deferredLightPass()
	{
		lightingBuffer.create(gBuffer.width, gBuffer.height, RenderTexture::Light);

		Texture::bind(&gBuffer.textures[GBuffer::Diffuse],  0);
		Texture::bind(&gBuffer.textures[GBuffer::Specular], 1);
		Texture::bind(&gBuffer.textures[GBuffer::Normal],   2);
		Texture::bind(&gBuffer.textures[GBuffer::Depth],    3);

		RenderTexture::bind(&lightingBuffer);
		{
			glClearColor(0, 0, 0, 0);
			glViewport(0, 0, lightingBuffer.width, lightingBuffer.height);
			glClear(GL_COLOR_BUFFER_BIT);

			glDepthMask(false);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			// TODO: make lights overwrite other lights
			renderAmbientLight();
			renderDirectionalLights();
			renderPointLights();
			renderSpotLights();

			glDisable(GL_BLEND);
			glDepthMask(true);
		}
		RenderTexture::bind(nullptr);

		return *this;
	} // end deferredLightPass()

	SceneRenderer& SceneRenderer::deferredFinalPass()
	{
		finalTexture.create(gBuffer.width, gBuffer.height, RenderTexture::Color);

		Texture::bind(&gBuffer.textures[GBuffer::Diffuse], 0);
		Texture::bind(&lightingBuffer.colorTexture, 1);

		RenderTexture::bind(&finalTexture);
		{
			glClearColor(1, 0, 1, 1);
			glViewport(0, 0, finalTexture.width, finalTexture.height);
			glClear(GL_COLOR_BUFFER_BIT);

			auto& shaders = g_shaderHolder.get("deferredFinalPass");

			shaders.use();

			shaders.setUniform("u_diffuse", 0);
			shaders.setUniform("u_lighting", 1);

			draw(&g_meshHolder.get("quad"));

			shaders.stopUsing();
		}
		RenderTexture::bind(nullptr);

		return *this;
	}

	void SceneRenderer::renderAmbientLight()
	{
		auto& shaders = g_shaderHolder.get("deferredAmbientLight");

		shaders.use();

		shaders.setUniform("u_light.intensities", world->ambientLight.colorIntensity);
		
		draw(&g_meshHolder.get("quad"));

		shaders.stopUsing();
	}

	void SceneRenderer::renderDirectionalLights()
	{
		auto& shaders = g_shaderHolder.get("deferredDirectionalLight");

		shaders.use();

		shaders.setUniform("u_specular", 1);
		shaders.setUniform("u_normal", 2);

		for (const DirectionalLight& light : world->directionalLights)
		{
			shaders.setUniform("u_light.base.intensities", light.colorIntensity); // shaderprogram.cpp

			shaders.setUniform("u_light.direction", normalize(light.direction));

			draw(&g_meshHolder.get("quad"));
		}
		shaders.stopUsing();
	}

	void SceneRenderer::renderPointLights()
	{
		auto& shaders = g_shaderHolder.get("deferredPointLight");

		shaders.use();

		shaders.setUniform("u_diffuse", 0);
		shaders.setUniform("u_specular", 1);
		shaders.setUniform("u_normal", 2);
		shaders.setUniform("u_depth", 3);

		shaders.setUniform("u_cameraInverse", inverse(camera->getMatrix()));

		for (const PointLight& light : world->pointLights)
		{
			//light.range = calculateLightRange(light.intensity, light.color, light.attenuation);

			shaders.setUniform("u_light.base.intensities", light.colorIntensity); // shaderprogram.cpp

			shaders.setUniform("u_light.position", light.position); // shaderprogram.cpp
			shaders.setUniform("u_light.range", light.range); // shaderprogram.cpp

			shaders.setUniform("u_light.attenuation.constant", light.attenuation.constant); // shaderprogram.cpp
			shaders.setUniform("u_light.attenuation.linear", light.attenuation.linear); // shaderprogram.cpp
			shaders.setUniform("u_light.attenuation.quadratic", light.attenuation.quadratic); // shaderprogram.cpp

			draw(&g_meshHolder.get("quad"));

		}
		shaders.stopUsing();
	}

	void SceneRenderer::renderSpotLights()
	{
		auto& shaders = g_shaderHolder.get("deferredSpotLight");

		shaders.use();

		shaders.setUniform("u_diffuse", 0);
		shaders.setUniform("u_specular", 1);
		shaders.setUniform("u_normal", 2);
		shaders.setUniform("u_depth", 3);

		shaders.setUniform("u_cameraInverse", inverse(camera->getMatrix()));

		for (const SpotLight& light : world->spotLights)
		{
			//light.range = calculateLightRange(light.intensity, light.color, light.attenuation);

			shaders.setUniform("u_light.coneAngle", static_cast<f32>(light.coneAngle)); // shaderprogram.cpp
			shaders.setUniform("u_light.direction", light.direction); // shaderprogram.cpp

			shaders.setUniform("u_light.pointLight.base.intensities", light.colorIntensity); // shaderprogram.cpp
									
			shaders.setUniform("u_light.pointLight.position", light.position); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.range", light.range); // shaderprogram.cpp
									
			shaders.setUniform("u_light.pointLight.attenuation.constant", light.attenuation.constant); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.attenuation.linear", light.attenuation.linear); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.attenuation.quadratic", light.attenuation.quadratic); // shaderprogram.cpp

			draw(&g_meshHolder.get("quad"));

		}
		shaders.stopUsing();
	}

	bool SceneRenderer::isCurrentShaders(const ShaderProgram* shaders)
	{
		if(shaders == currentShaders)
		{
			return true;
		}

		return false;
	}

	bool SceneRenderer::isCurrentTexture(const Texture* texture)
	{
		if (texture == currentTexture)
		{
			return true;
		}
		return false;
	}

	void SceneRenderer::setShaders(const ShaderProgram* shaders)
	{
		//if(shaders != m_currentShaders && m_currentShaders)
		//	m_currentShaders->stopUsing();
		if (shaders != currentShaders)
		{
			if (currentShaders)
				currentShaders->stopUsing();

			currentShaders = shaders;
			currentShaders->use();

		}
	}

	void SceneRenderer::setTexture(const Texture* texture, u32 position)
	{
		//assert(m_currentTexture);

		if (texture != currentTexture)
		{
			currentTexture = texture;
			Texture::bind(currentTexture, position);
		}
	}

} // end Dunjun
