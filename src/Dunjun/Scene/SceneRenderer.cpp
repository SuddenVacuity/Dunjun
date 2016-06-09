
#include <Dunjun/Scene/SceneRenderer.hpp>
#include <Dunjun/Scene/MeshRenderer.hpp>

namespace Dunjun
{
	SceneRenderer::SceneRenderer()
	{
	}

	SceneRenderer::~SceneRenderer()
	{
	}

	void SceneRenderer::reset()
	{
		if (m_currentShaders)
			m_currentShaders->stopUsing();

		m_currentShaders = nullptr;
		m_currentTexture = nullptr;
		Texture::bind(nullptr, 0);
		m_currentMaterial = nullptr; // commenting this out makes the top right corner of the window white

	}

	void SceneRenderer::clearAll()
	{
		m_modelInstances.clear();
		m_directionalLights.clear();
		m_pointLights.clear();
		m_spotLights.clear();
	}

	void SceneRenderer::addSceneGraph(const SceneNode& node, const Transform& t)
	{
		node.draw(*this, t);
	}

	void SceneRenderer::draw(const Mesh* mesh) const
	{
		if(mesh)
			mesh->draw();
	}
	
	void SceneRenderer::addModelInstance(const MeshRenderer& meshRenderer, Transform t)
	{
		if(meshRenderer.getParent()->visible == true)
			m_modelInstances.emplace_back(&meshRenderer, t);
	}

	void SceneRenderer::addDirectionalLight(const DirectionalLight* light)
	{
		m_directionalLights.emplace_back(light);
	}

	void SceneRenderer::addPointLight(const PointLight* light)
	{
		m_pointLights.emplace_back(light);
	}

	void SceneRenderer::addSpotLight(const SpotLight* light)
	{
		m_spotLights.emplace_back(light);
	}

	void SceneRenderer::deferredGeometryPass()
	{
		std::sort(std::begin(m_modelInstances), std::end(m_modelInstances),
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
			glViewport(0, 0, gBuffer.getWidth(), gBuffer.getHeight());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaders.use();

			shaders.setUniform("u_camera", camera->getMatrix()); // shaderprogram.cpp
			shaders.setUniform("u_cameraPosition", camera->transform.position); // shaderprogram.cpp

			for (const auto& inst : m_modelInstances)
			{
				if (!inst.asset->mesh)
					continue;

					//setShaders(inst.asset->material->shaders);

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
	} // end deferredGeometryPass()

	void SceneRenderer::deferredLightPass()
	{
		//if(lightingTexture == nullptr)
		//	lightingTexture = make_unique<RenderTexture>();

		lightingTexture.create(gBuffer.getWidth(), gBuffer.getHeight(), RenderTexture::Light);

		Texture::bind(&gBuffer.diffuse,  0);
		Texture::bind(&gBuffer.specular, 1);
		Texture::bind(&gBuffer.normal,   2);
		Texture::bind(&gBuffer.depth,    3);

		RenderTexture::bind(&lightingTexture);
		{
			glClearColor(0, 0, 0, 0);
			glViewport(0, 0, lightingTexture.getWidth(), lightingTexture.getHeight());
			glClear(GL_COLOR_BUFFER_BIT);

			glDepthMask(false);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			renderAmbientLight();
			renderDirectionalLights();
			renderPointLights();
			renderSpotLights();

			glDisable(GL_BLEND);
			glDepthMask(true);
		}
		RenderTexture::bind(nullptr);
	} // end deferredLightPass()

	void SceneRenderer::deferredFinalPass()
	{
		finalTexture.create(gBuffer.getWidth(), gBuffer.getHeight(), RenderTexture::Color);

		Texture::bind(&gBuffer.diffuse, 0);
		Texture::bind(&lightingTexture.colorTexture, 1);

		RenderTexture::bind(&finalTexture);
		{
			glClearColor(1, 0, 1, 1);
			glViewport(0, 0, finalTexture.getWidth(), finalTexture.getHeight());
			glClear(GL_COLOR_BUFFER_BIT);

			auto& shaders = g_shaderHolder.get("deferredFinalPass");

			shaders.use();

			shaders.setUniform("u_diffuse", 0);
			shaders.setUniform("u_lighting", 1);

			draw(&g_meshHolder.get("quad"));

			shaders.stopUsing();
		}
		RenderTexture::bind(nullptr);

	}

	void SceneRenderer::renderAmbientLight()
	{
		auto& shaders = g_shaderHolder.get("deferredAmbientLight");

		shaders.use();

		shaders.setUniform("u_light.intensities", Vector3(0, 0, 0.002f));	
		
		draw(&g_meshHolder.get("quad"));

		shaders.stopUsing();
	}

	void SceneRenderer::renderDirectionalLights()
	{
		auto& shaders = g_shaderHolder.get("deferredDirectionalLight");

		shaders.use();

		shaders.setUniform("u_specular", 1);
		shaders.setUniform("u_normal", 2);

		for (const DirectionalLight* light : m_directionalLights)
		{
			shaders.setUniform("u_light.base.intensities", light->colorIntensity); // shaderprogram.cpp

			shaders.setUniform("u_light.direction", normalize(light->direction));

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

		for (const PointLight* light : m_pointLights)
		{
			light->calculateRange();

			//Vector3 colorIntensities;
			//
			//colorIntensities.r = light->color.r / 255.0f;
			//colorIntensities.g = light->color.g / 255.0f;
			//colorIntensities.b = light->color.b / 255.0f;
			//colorIntensities *= light->intensity;

			shaders.setUniform("u_light.base.intensities", light->colorIntensity); // shaderprogram.cpp

			shaders.setUniform("u_light.position", light->position); // shaderprogram.cpp
			shaders.setUniform("u_light.range", light->range); // shaderprogram.cpp

			shaders.setUniform("u_light.attenuation.constant", light->attenuation.constant); // shaderprogram.cpp
			shaders.setUniform("u_light.attenuation.linear", light->attenuation.linear); // shaderprogram.cpp
			shaders.setUniform("u_light.attenuation.quadratic", light->attenuation.quadratic); // shaderprogram.cpp

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

		for (const SpotLight* light : m_spotLights)
		{
			light->calculateRange();

			//Vector3 colorIntensities;
			//
			//colorIntensities.r = light->color.r / 255.0f;
			//colorIntensities.g = light->color.g / 255.0f;
			//colorIntensities.b = light->color.b / 255.0f;
			//colorIntensities *= light->intensity;


			shaders.setUniform("u_light.coneAngle", static_cast<f32>(light->coneAngle)); // shaderprogram.cpp
			shaders.setUniform("u_light.direction", light->direction); // shaderprogram.cpp

			shaders.setUniform("u_light.pointLight.base.intensities", light->colorIntensity); // shaderprogram.cpp
									
			shaders.setUniform("u_light.pointLight.position", light->position); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.range", light->range); // shaderprogram.cpp
									
			shaders.setUniform("u_light.pointLight.attenuation.constant", light->attenuation.constant); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.attenuation.linear", light->attenuation.linear); // shaderprogram.cpp
			shaders.setUniform("u_light.pointLight.attenuation.quadratic", light->attenuation.quadratic); // shaderprogram.cpp

			draw(&g_meshHolder.get("quad"));

		}
		shaders.stopUsing();
	}

	bool SceneRenderer::isCurrentShaders(const ShaderProgram* shaders)
	{
		if(!m_currentShaders)
			return false;

		if(shaders == m_currentShaders)
		{
			return true;
		}

		return false;
	}

	bool SceneRenderer::isCurrentTexture(const Texture* texture)
	{
		if (texture == m_currentTexture)
		{
			return true;
		}
		return false;
	}

	void SceneRenderer::setShaders(const ShaderProgram* shaders)
	{
		//if(shaders != m_currentShaders && m_currentShaders)
		//	m_currentShaders->stopUsing();
		if (shaders != m_currentShaders)
		{
			if (m_currentShaders)
				m_currentShaders->stopUsing();

			m_currentShaders = shaders;
			m_currentShaders->use();

		}
	}

	void SceneRenderer::setTexture(const Texture* texture, u32 position)
	{
		//assert(m_currentTexture);

		if (texture != m_currentTexture || !m_currentTexture)
		{
			m_currentTexture = texture;
			Texture::bind(m_currentTexture, position);
		}
	}

	void SceneRenderer::setCamera(const Camera& c)
	{
		camera = &c;
	}
	
	//void SceneRenderer::setUniforms(const Transform& t)
	//{
	//	if(!m_currentShaders)
	//		return;
	//	else if(!m_currentShaders->isInUse())
	//		m_currentShaders->use();
	//
	//	m_currentShaders->setUniform("u_camera", m_currentCamera->getMatrix()); // shaderprogram.cpp
	//	m_currentShaders->setUniform("u_transform", t); // shaderprogram.cpp
	//	m_currentShaders->setUniform("u_tex", (Dunjun::u32)0); // shaderprogram.cpp
	//}
} // end Dunjun
