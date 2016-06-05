
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
		modelInstances.clear();
		pointLights.clear();
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
		if(meshRenderer.parent->visible == true)
			modelInstances.push_back({&meshRenderer, t});
	}

	void SceneRenderer::addPointLight(const PointLight* light)
	{
		pointLights.push_back(light);
	}

	//
	//void SceneRenderer::renderAll()
	//{
	//	std::sort(std::begin(modelInstances), std::end(modelInstances),
	//		[](const ModelInstance& a, const ModelInstance& b) -> bool
	//	{
	//		const auto& A = a.asset->material;
	//		const auto& B = b.asset->material;
	//
	//		// if same shaders sort by texture else sort by shader
	//		if(A->shaders == B->shaders)
	//			return A->diffuseMap < B->diffuseMap;
	//		else
	//			return A->shaders < B->shaders;
	//	});
	//
	//
	//	for(const auto& inst : modelInstances)
	//	{
	//		if(!inst.asset->mesh)
	//			continue;
	//
	//		if (!isCurrentShaders(inst.asset->material->shaders))
	//		{
	//			setShaders(inst.asset->material->shaders);
	//
	//			const Material& material = *inst.asset->material;
	//
	//			m_currentShaders->setUniform("u_camera", camera->getMatrix()); // shaderprogram.cpp
	//			m_currentShaders->setUniform("u_cameraPosition", camera->transform.position); // shaderprogram.cpp
	//
	//			m_currentShaders->setUniform("u_material.diffuseMap", (u32)0); // shaderprogram.cpp
	//			m_currentShaders->setUniform("u_material.diffuseColor", material.diffuseColor); // shaderprogram.cpp
	//			m_currentShaders->setUniform("u_material.specularColor", material.specularColor); // shaderprogram.cpp
	//			m_currentShaders->setUniform("u_material.specularExponent", material.specularExponent); // shaderprogram.cpp
	//
	//			const PointLight* light = pointLights[0];
	//			light->calculateRange();
	//
	//			Vector3 lightIntensities;
	//			lightIntensities.r = light->color.r / 255.0f;
	//			lightIntensities.g = light->color.g / 255.0f;
	//			lightIntensities.b = light->color.b / 255.0f;
	//			lightIntensities *= light->brightness;
	//
	//			m_currentShaders->setUniform("u_light.position", light->position); // shaderprogram.cpp
	//			m_currentShaders->setUniform("u_light.intensities", lightIntensities); // shaderprogram.cpp
	//
	//			m_currentShaders->setUniform("u_light.attenuation.constant", light->attenuation.constant); // shaderprogram.cpp
	//			m_currentShaders->setUniform("u_light.attenuation.linear", light->attenuation.linear); // shaderprogram.cpp
	//			m_currentShaders->setUniform("u_light.attenuation.quadratic", light->attenuation.quadratic); // shaderprogram.cpp
	//
	//			m_currentShaders->setUniform("u_light.range", light->range); // shaderprogram.cpp
	//
	//		}
	//
	//		setTexture(inst.asset->material->diffuseMap, 0);
	//
	//		m_currentShaders->setUniform("u_transform", inst.transform); // shaderprogram.cpp
	//
	//		draw(inst.asset->mesh);
	//	}
	//}
	//
	void SceneRenderer::deferredGeometryPass()
	{
		std::sort(std::begin(modelInstances), std::end(modelInstances),
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

					//setShaders(inst.asset->material->shaders);

				const Material& material = *inst.asset->material;
				//const PointLight* light = pointLights[0];

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

		lightingTexture.create(gBuffer.width, gBuffer.height, RenderTexture::Light);

		auto& shaders = g_shaderHolder.get("deferredPointLight");

		Texture::bind(&gBuffer.diffuse,  0);
		Texture::bind(&gBuffer.specular, 1);
		Texture::bind(&gBuffer.normal,   2);
		Texture::bind(&gBuffer.depth,    3);

		RenderTexture::bind(&lightingTexture);
		{
			glClearColor(0, 0, 0, 0);
			glViewport(0, 0, lightingTexture.width, lightingTexture.height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaders.use();

			shaders.setUniform("u_diffuse",  0);
			shaders.setUniform("u_specular", 1);
			shaders.setUniform("u_normal",   2);
			shaders.setUniform("u_depth",    3);

			shaders.setUniform("u_cameraInverse", inverse(camera->getMatrix()));

			for(const PointLight* light : pointLights)
			{
				light->calculateRange();
				Vector3 lightIntensities;

				lightIntensities.r = light->color.r / 255.0f;
				lightIntensities.g = light->color.g / 255.0f;
				lightIntensities.b = light->color.b / 255.0f;
				lightIntensities *= light->brightness;

				shaders.setUniform("u_light.position", light->position); // shaderprogram.cpp
				shaders.setUniform("u_light.intensities", lightIntensities); // shaderprogram.cpp
			
				shaders.setUniform("u_light.attenuation.constant", light->attenuation.constant); // shaderprogram.cpp
				shaders.setUniform("u_light.attenuation.linear", light->attenuation.linear); // shaderprogram.cpp
				shaders.setUniform("u_light.attenuation.quadratic", light->attenuation.quadratic); // shaderprogram.cpp
		
				shaders.setUniform("u_light.range", light->range); // shaderprogram.cpp

				glDepthMask(GL_FALSE);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);

				draw(&g_meshHolder.get("quad"));

				glDisable(GL_BLEND);
				glDepthMask(GL_TRUE);
			}
			shaders.stopUsing();
		}
		RenderTexture::bind(nullptr);
	} // end deferredLightPass()

	//void SceneRenderer::setMaterial(const Material* material)
	//{
	//	if(material != m_currentMaterial)
	//	{
	//		m_currentMaterial = material;
	//
	//		setShaders(m_currentMaterial->shaders);
	//		setTexture(m_currentMaterial->texture);
	//	}
	//}

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

	void SceneRenderer::setTexture(const Texture* texture, GLuint position)
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
